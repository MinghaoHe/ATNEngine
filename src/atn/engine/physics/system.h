/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_PHYSICS_SYSTEM
#define INCLUDE_ATN_ENGINE_PHYSICS_SYSTEM

#include "atn/engine/ecs/system.h"

namespace atn::engine::physics {

class System : public ecs::System {
 public:
  System();
  virtual ~System();

 public:
  virtual void PrevTick(::std::size_t delta) override;
  virtual void EntityTick(::std::size_t delta, ::std::shared_ptr<ecs::Component> component) override;
  virtual void PostTick(::std::size_t delta) override;
};

}  // namespace atn::engine::physics

#endif  // !INCLUDE_ATN_ENGINE_PHYSICS_SYSTEM
