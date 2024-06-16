/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "canvas.h"

namespace atn::engine {

Canvas::Canvas(const canvas::CreateParam &param) : size_(param.size), name_(param.name) {}

Canvas::~Canvas() {}

canvas::Size Canvas::Size() const { return size_; }

::std::string Canvas::Name() const { return name_; }

}  // namespace atn::engine
