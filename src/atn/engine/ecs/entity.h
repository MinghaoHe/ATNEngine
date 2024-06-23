/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_ECS_ENTITY
#define INCLUDE_ATN_ENGINE_ECS_ENTITY

#include <memory>
#include <optional>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "atn/engine/ecs/component.h"
#include "atn/engine/ecs/define.h"

namespace atn::engine::ecs {

struct EntityCreateParam {
  const ::std::string tag;
  explicit EntityCreateParam(const ::std::string &tag) : tag(tag) {}
};

struct EntitySeed {
  bool existence{true};
  ::std::uintptr_t id{};
  ::std::string tag;
  ::glm::mat4 model{1.0f};

  ::std::shared_mutex mutex;
};

class Entity final {
 public:
  Entity();
  ~Entity();

 public:
  EntitySeed &Seed();

  void Attach(Category category, ::std::shared_ptr<Component> component);
  ::std::optional<::std::shared_ptr<Component>> Appear(Category category) const;

 private:
  EntitySeed seed_;
  ::std::unordered_map<Category, ::std::shared_ptr<Component>> appearance_;
};

}  // namespace atn::engine::ecs

#endif  // !INCLUDE_ATN_ENGINE_ECS_ENTITY
