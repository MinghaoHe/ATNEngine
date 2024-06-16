/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_WORLD_PRIME_PLAYER
#define INCLUDE_ATN_ENGINE_WORLD_PRIME_PLAYER

#include <atomic>
#include <thread>
#include <vector>

#include "atn/engine/ecs/entity.h"
#include "atn/engine/world.h"
#include "atn/engine/world/tick_lock.h"

namespace atn::engine::world {

class PrimePlayer final {
 public:
  PrimePlayer(::std::shared_ptr<World> world, ::std::shared_ptr<TickLock> tick_lock);
  ~PrimePlayer();

 public:
  void CreateWorld();
  void CreateEntity(std::shared_ptr<ecs::EntityCreateParam> create_param);
  void DestroyWorld();

 private:
  void Create();
  void CreateEntities();
  void DestroyEntities();

 private:
  ::std::atomic_bool continue_{true};
  ::std::thread creator_thread_;

  ::std::vector<::std::shared_ptr<ecs::EntityCreateParam>> to_create_;

  ::std::shared_ptr<World> world_;
  ::std::shared_ptr<TickLock> tick_lock_;
};

}  // namespace atn::engine::world

#endif  // !INCLUDE_ATN_ENGINE_WORLD_PRIME_PLAYER
