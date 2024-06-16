/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "system.h"

namespace atn::engine::logic {

System::System() : ecs::System() {}

System::~System() {}

void System::PrevTick(::std::size_t delta) {}

void System::EntityTick(::std::size_t delta, ::std::shared_ptr<ecs::Component> component) {}

void System::PostTick(::std::size_t delta) {}

}  // namespace atn::engine::logic
