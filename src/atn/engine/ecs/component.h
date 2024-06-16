/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_ECS_COMPONENT
#define INCLUDE_ATN_ENGINE_ECS_COMPONENT

#include <cstddef>

namespace atn::engine::ecs {

class Component {
 public:
  Component();
  virtual ~Component();

 public:
  virtual void Tick(::std::size_t delta) = 0;
};

}  // namespace atn::engine::ecs

#endif  // !INCLUDE_ATN_ENGINE_ECS_COMPONENT
