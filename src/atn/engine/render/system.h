/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_RENDER_SYSTEM
#define INCLUDE_ATN_ENGINE_RENDER_SYSTEM

#include <filesystem>

#include <vulkan/vulkan.hpp>

#include "atn/engine/canvas.h"
#include "atn/engine/ecs/system.h"

namespace atn::engine::render {

class System : public ecs::System {
 public:
  System(::std::shared_ptr<Canvas> canvas);
  virtual ~System();

 public:
  virtual void PrevTick(::std::size_t delta) override;
  virtual void EntityTick(::std::size_t delta, ::std::shared_ptr<ecs::Component> component) override;
  virtual void PostTick(::std::size_t delta) override;

 private:
  ::std::vector<const char *> GetLayersList();
  ::std::vector<const char *> GetExtensionsList();
  void CreateDebugMessenger();
  void DestroyDebugMessenger();

  ::vk::PhysicalDevice ChoosePhysicsDevice(const ::vk::Instance &instance);
  ::std::uint32_t GetGraphicsQueueFamilyIndex(const ::vk::PhysicalDevice &physics_device);
  ::std::uint32_t GetPresentQueueFamilyIndex(const ::vk::PhysicalDevice &physics_device,
                                             const ::vk::SurfaceKHR &surface);
  ::vk::Device CreateLogicDevice(const ::vk::PhysicalDevice &physics_device, ::std::uint32_t graphics_queue_index,
                                 ::std::uint32_t present_queue_index);
  bool IsDeviceExtensionsSupport(const ::vk::PhysicalDevice &physics_device);

  ::vk::Extent2D ChooseSwapExtent(const ::vk::SurfaceCapabilitiesKHR &capabilities);
  ::vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const ::std::vector<::vk::SurfaceFormatKHR> &available_formats);
  ::vk::PresentModeKHR ChooseSwapPresentMode(const ::std::vector<::vk::PresentModeKHR> &available_modes);
  ::vk::SwapchainKHR CreateSwapchain(const ::vk::Device &device, const ::vk::SurfaceKHR &surface,
                                     const ::vk::SurfaceCapabilitiesKHR &capabilities, const ::vk::Extent2D &extent,
                                     const ::vk::SurfaceFormatKHR &surface_format, ::std::uint32_t graphics_queue_index,
                                     ::std::uint32_t present_queue_index);
  ::std::vector<::vk::ImageView> CreateImageViews(const ::vk::Device &device, const ::std::vector<::vk::Image> &images,
                                                  const ::vk::SurfaceFormatKHR &surface_format);

  ::vk::ShaderModule CreateShaderModule(const ::vk::Device &device, const ::std::filesystem::path &sprv_path);

 private:
  static VKAPI_ATTR ::vk::Bool32 VKAPI_CALL DebugCallback(::vk::DebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                          ::vk::DebugUtilsMessageTypeFlagBitsEXT message_type,
                                                          const ::vk::DebugUtilsMessengerCallbackDataEXT *callback_data,
                                                          void *user_data);

 private:
  ::vk::Instance instance_;
  ::vk::SurfaceKHR surface_;
  ::vk::PhysicalDevice physics_device_;
  ::vk::Device logic_device_;
  ::vk::Queue graphics_queue_;
  ::vk::Queue present_queue_;
  ::vk::SurfaceCapabilitiesKHR capabilities_;
  ::vk::Extent2D extent_;
  ::vk::SurfaceFormatKHR surface_format_;

  ::vk::SwapchainKHR swapchain_;
  ::std::vector<::vk::Image> images_;
  ::std::vector<::vk::ImageView> image_views_;

  ::vk::PipelineLayout pipeline_layout_;

  ::vk::DispatchLoaderDynamic dispatcher_;
  ::vk::DebugUtilsMessengerEXT messenger_;

  bool enable_validation_layers_ = false;

  ::std::shared_ptr<Canvas> canvas_;
};

}  // namespace atn::engine::render

#endif  // !INCLUDE_ATN_ENGINE_RENDER_SYSTEM
