/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_RENDER_CONFIG
#define INCLUDE_ATN_ENGINE_RENDER_CONFIG

#include <string_view>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace atn::engine::render::config {

inline const ::std::vector<const char*> kRequiredExtensions{};

inline const ::std::vector<const char*> kRequiredLayers{"VK_LAYER_KHRONOS_validation"};

inline const ::std::vector<const char*> kRequiredDeviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

}  // namespace atn::engine::render::config

#endif  // !INCLUDE_ATN_ENGINE_RENDER_VULKAN_CONFIG
