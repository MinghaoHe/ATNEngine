/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_WORLD
#define INCLUDE_ATN_ENGINE_WORLD

#include <list>

#include "atn/engine/ecs/entity.h"

namespace atn::engine {

class World final {
 public:
  World();
  ~World();

 public:
  void Seed(::std::list<::std::unique_ptr<ecs::Entity>> entities);

  ::std::list<::std::unique_ptr<ecs::Entity>> &Entities() { return entities_; }

 private:
  ::std::list<::std::unique_ptr<ecs::Entity>> entities_;
};

}  // namespace atn::engine

#endif  // !INCLUDE_ATN_ENGINE_WORLD
