/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "prime_player.h"

namespace atn::engine::world {

PrimePlayer::PrimePlayer(::std::shared_ptr<World> world, ::std::shared_ptr<TickLock> tick_lock)
    : world_(world), tick_lock_(tick_lock) {}

PrimePlayer::~PrimePlayer() {}

void PrimePlayer::CreateWorld() {
  // world_->Seed(std::move(game::OrigSeed()));
  creator_thread_ = std::thread(&PrimePlayer::Create, this);
}

void PrimePlayer::CreateEntity(std::shared_ptr<ecs::EntityCreateParam> create_param) {
  to_create_.push_back(create_param);
}

void PrimePlayer::DestroyWorld() {
  continue_.store(false);
  tick_lock_->WakePrimer();
  creator_thread_.join();
  world_->Entities().clear();
}

void PrimePlayer::Create() {
  while (continue_.load()) {
    tick_lock_->PrimerSleep();

    DestroyEntities();
    CreateEntities();

    tick_lock_->WakeWorld();
  }
}

void PrimePlayer::CreateEntities() {
  for (auto create_param : to_create_) {
    // world_->Entities().emplace_back(game::CreateEntity(create_param));
  }
  to_create_.clear();
}

void PrimePlayer::DestroyEntities() {
  world_->Entities().remove_if([](auto &entity) { return !entity->Seed().existence; });
}

}  // namespace atn::engine::world
