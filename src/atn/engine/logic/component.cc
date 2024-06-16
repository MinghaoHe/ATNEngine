/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "component.h"

namespace atn::engine::logic {

Component::Component() : ecs::Component() {}

Component::~Component() {}

void Component::Tick(::std::size_t delta) {}

}  // namespace atn::engine::logic
