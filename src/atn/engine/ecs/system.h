/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_ECS_SYSTEM
#define INCLUDE_ATN_ENGINE_ECS_SYSTEM

#include <memory>

#include "atn/engine/ecs/component.h"

namespace atn::engine::ecs {

class System {
 public:
  System();
  virtual ~System();

 public:
  virtual void PrevTick(::std::size_t delta) = 0;
  virtual void EntityTick(::std::size_t delta, ::std::shared_ptr<Component> component) = 0;
  virtual void PostTick(::std::size_t delta) = 0;
};

}  // namespace atn::engine::ecs

#endif  // !INCLUDE_ATN_ENGINE_ECS_SYSTEM
