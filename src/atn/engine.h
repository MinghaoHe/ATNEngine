/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE
#define INCLUDE_ATN_ENGINE

#include <memory>
#include <unordered_map>

#include "atn/engine/canvas.h"
#include "atn/engine/ecs/define.h"
#include "atn/engine/ecs/system.h"
#include "atn/engine/world.h"
#include "atn/engine/world/prime_player.h"
#include "atn/engine/world/tick_lock.h"

namespace atn {

using namespace engine;

class Engine final {
 public:
  Engine(int argc, char *argv[]);
  ~Engine();

 public:
  void Initialize();
  void CreateWorld();
  int Exec();

 private:
  void MakeCanvas(int width, int height);
  bool Continue();
  void Tick();
  void PollEvent();
  void LoadWorld();

 private:
  size_t delta_{0};
  volatile bool continue_{true};

  ::std::shared_ptr<Canvas> canvas_;
  ::std::shared_ptr<World> world_;
  ::std::shared_ptr<world::TickLock> tick_lock_;
  ::std::shared_ptr<world::PrimePlayer> primer_;

  ::std::unordered_map<ecs::Category, ::std::shared_ptr<ecs::System>> systems_;
};

}  // namespace atn

#endif  // !INCLUDE_ATN_ENGINE
