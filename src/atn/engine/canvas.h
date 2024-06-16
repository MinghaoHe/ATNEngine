/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_CANVAS
#define INCLUDE_ATN_ENGINE_CANVAS

#include <vulkan/vulkan.hpp>

#include "atn/engine/canvas/define.h"

namespace atn::engine {

class Canvas {
 public:
  Canvas(const canvas::CreateParam& param);
  virtual ~Canvas();

 public:
  canvas::Size Size() const;
  ::std::string Name() const;

 public:
  virtual ::vk::SurfaceKHR CreateSurface(const ::vk::Instance& instance) = 0;

 public:
  virtual void PollEvent() = 0;
  virtual void Destroy() = 0;
  virtual bool WindowShouldClose() = 0;

 private:
  canvas::Size size_;
  ::std::string name_;
};

}  // namespace atn::engine

#endif  // !INCLUDE_ATN_ENGINE_CANVAS
