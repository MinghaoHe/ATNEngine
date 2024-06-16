/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "engine.h"

#include <chrono>
#include <filesystem>
#include <thread>

#include "atn/engine/canvas/glfw.h"
#include "atn/engine/define.h"
#include "atn/engine/logic/system.h"
#include "atn/engine/physics/system.h"
#include "atn/engine/render/system.h"

namespace atn {

std::weak_ptr<world::PrimePlayer> gprimer_;

Engine::Engine(int argc, char *argv[]) {
  std::filesystem::current_path(std::filesystem::canonical(argv[0]).parent_path());
}

Engine::~Engine() {}

void Engine::Initialize() {
  MakeCanvas(kWidth, kHeight);
  systems_[ecs::Category::kLogic] = std::make_shared<logic::System>();
  systems_[ecs::Category::kPhysics] = std::make_shared<physics::System>();
  systems_[ecs::Category::kRender] = std::make_shared<render::System>(canvas_);
}

void Engine::CreateWorld() {
  world_ = std::make_shared<World>();
  tick_lock_ = std::make_shared<world::TickLock>();

  primer_ = std::make_shared<world::PrimePlayer>(world_, tick_lock_);
  gprimer_ = primer_;

  primer_->CreateWorld();
}

int Engine::Exec() {
  while (Continue()) {
    Tick();
  }
  primer_->DestroyWorld();
  canvas_->Destroy();
  return 0;
}

void Engine::Tick() {
  PollEvent();

  for (auto &[category, system] : systems_) {
    system->PrevTick(delta_);
    for (auto &entity : world_->Entities()) {
      auto component{entity->Appear(category)};
      if (!component.has_value()) continue;

      system->EntityTick(delta_, component.value());
    }
    system->PostTick(delta_);
  }
}

void Engine::MakeCanvas(int width, int height) {
  canvas::CreateParam canvas_param{.size{width, height}, .name{kAppName}};
  canvas_ = std::make_shared<canvas::Glfw>(canvas_param);
}

bool Engine::Continue() {
  const static size_t kExpectedDelta{1000 / kFps};
  static auto now{std::chrono::steady_clock::now()};

  delta_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - now).count();

  tick_lock_->WakePrimer();

  if (delta_ < kExpectedDelta) {
    std::this_thread::sleep_for(std::chrono::milliseconds(kExpectedDelta - delta_));
    delta_ = kExpectedDelta;
  }

  tick_lock_->WorldSleep();

  now = std::chrono::steady_clock::now();

  return continue_;
}

void Engine::PollEvent() {
  canvas_->PollEvent();
  continue_ = !canvas_->WindowShouldClose();
}

void Engine::LoadWorld() {}

}  // namespace atn
