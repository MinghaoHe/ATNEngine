/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_CANVAS_GLFW
#define INCLUDE_ATN_ENGINE_CANVAS_GLFW

#include <GLFW/glfw3.h>

#include "atn/engine/canvas.h"

namespace atn::engine::canvas {

class Glfw : public Canvas {
 public:
  Glfw(const canvas::CreateParam &param);
  virtual ~Glfw();

 public:
  virtual ::vk::SurfaceKHR CreateSurface(const ::vk::Instance &instance) override;

 public:
  virtual void PollEvent() override;
  virtual void Destroy() override;
  virtual bool WindowShouldClose() override;

 private:
  GLFWwindow *window_{nullptr};
};

}  // namespace atn::engine::canvas

#endif  // !INCLUDE_ATN_ENGINE_CANVAS_GLFW
