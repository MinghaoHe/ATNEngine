/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_RENDER_COMPONENT
#define INCLUDE_ATN_ENGINE_RENDER_COMPONENT

#include "atn/engine/ecs/component.h"

namespace atn::engine::render {

class Component : public ecs::Component {
 public:
  Component();
  virtual ~Component();

 public:
  virtual void Tick(::std::size_t delta) override;
};

}  // namespace atn::engine::render

#endif  // !INCLUDE_ATN_ENGINE_RENDER_COMPONENT
