/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "entity.h"

namespace atn::engine::ecs {

Entity::Entity() {}

Entity::~Entity() {}

EntitySeed &Entity::Seed() { return seed_; }

void Entity::Attach(Category category, ::std::shared_ptr<Component> component) { appearance_[category] = component; }

std::optional<std::shared_ptr<Component>> Entity::Appear(Category category) const {
  auto component = appearance_.find(category);
  if (component == appearance_.end()) {
    return std::nullopt;
  }
  return component->second;
}

}  // namespace atn::engine::ecs
