/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_CANVAS_DEFINE
#define INCLUDE_ATN_ENGINE_CANVAS_DEFINE

#include <string>

namespace atn::engine::canvas {

struct Size {
  int width{};
  int height{};
};

struct CreateParam {
  Size size{};
  ::std::string name;
};

}  // namespace atn::engine::canvas

#endif  // !INCLUDE_ATN_ENGINE_CANVAS_GLFW
