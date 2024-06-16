/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "glfw.h"

namespace atn::engine::canvas {

Glfw::Glfw(const canvas::CreateParam &param) : Canvas(param) {
  ::glfwInit();
  ::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  ::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window_ = ::glfwCreateWindow(param.size.width, param.size.height, param.name.c_str(), nullptr, nullptr);
}

Glfw::~Glfw() { ::glfwTerminate(); }

::vk::SurfaceKHR Glfw::CreateSurface(const ::vk::Instance &instance) {
  VkSurfaceKHR surface;
  ::glfwCreateWindowSurface(instance, window_, nullptr, &surface);
  return ::vk::SurfaceKHR(surface);
}

void Glfw::PollEvent() { ::glfwPollEvents(); }

void Glfw::Destroy() { ::glfwDestroyWindow(window_); }

bool Glfw::WindowShouldClose() { return ::glfwWindowShouldClose(window_); }

}  // namespace atn::engine::canvas
