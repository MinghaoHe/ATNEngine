/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "system.h"

#include <fstream>
#include <iostream>
#include <unordered_set>

#include <GLFW/glfw3.h>

#include "atn/engine/define.h"
#include "atn/engine/render/config.h"

namespace atn::engine::render {

System::System(::std::shared_ptr<Canvas> canvas) : ecs::System(), canvas_(canvas) {
  ::vk::ApplicationInfo app_info(kAppName, kAppVersion, kEngineName, kEngineVersion, VK_API_VERSION_1_0);

  ::std::vector<const char*> layers_list = GetLayersList();
  ::std::vector<const char*> extensions_list = GetExtensionsList();

  ::vk::InstanceCreateFlagBits instance_flag = {};
#ifdef __APPLE__
  instance_flag = ::vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
#endif
  ::vk::InstanceCreateInfo create_info(instance_flag, &app_info, static_cast<::std::uint32_t>(layers_list.size()),
                                       layers_list.data(), static_cast<::std::uint32_t>(extensions_list.size()),
                                       extensions_list.data());
  instance_ = ::vk::createInstance(create_info);
  dispatcher_ = ::vk::DispatchLoaderDynamic(instance_, ::vkGetInstanceProcAddr);
  if (enable_validation_layers_) {
    CreateDebugMessenger();
  }

  physics_device_ = ChoosePhysicsDevice(instance_);
  surface_ = canvas_->CreateSurface(instance_);
  ::std::uint32_t graphics_queue_index = GetGraphicsQueueFamilyIndex(physics_device_);
  ::std::uint32_t present_queue_index = GetPresentQueueFamilyIndex(physics_device_, surface_);
  logic_device_ = CreateLogicDevice(physics_device_, graphics_queue_index, present_queue_index);
  graphics_queue_ = logic_device_.getQueue(graphics_queue_index, 0);
  present_queue_ = logic_device_.getQueue(present_queue_index, 0);

  capabilities_ = physics_device_.getSurfaceCapabilitiesKHR(surface_);
  extent_ = ChooseSwapExtent(capabilities_);
  surface_format_ = ChooseSwapSurfaceFormat(physics_device_.getSurfaceFormatsKHR(surface_));
  swapchain_ = CreateSwapchain(logic_device_, surface_, capabilities_, extent_, surface_format_, graphics_queue_index,
                               present_queue_index);
  images_ = logic_device_.getSwapchainImagesKHR(swapchain_);
  image_views_ = CreateImageViews(logic_device_, images_, surface_format_);

  pipeline_layout_ = CreatePipelineLayout(logic_device_);
  render_pass_ = CreateRenderPass(logic_device_, surface_format_.format);

  pipeline_ = CreatePipeline(logic_device_, "shader/deemo.vert.spv", "shader/deemo.frag.spv", extent_, pipeline_layout_,
                             render_pass_);
}

System::~System() {
  logic_device_.destroyPipeline(pipeline_);
  logic_device_.destroyRenderPass(render_pass_);
  logic_device_.destroyPipelineLayout(pipeline_layout_);
  std::ranges::for_each(image_views_, [this](const ::vk::ImageView& view) { logic_device_.destroyImageView(view); });
  image_views_.clear();
  images_.clear();
  logic_device_.destroySwapchainKHR(swapchain_);
  instance_.destroySurfaceKHR(surface_);
  logic_device_.destroy();
  if (enable_validation_layers_) {
    DestroyDebugMessenger();
  }
  instance_.destroy();
}

void System::PrevTick(::std::size_t delta) {}

void System::EntityTick(::std::size_t delta, ::std::shared_ptr<ecs::Component> component) {}

void System::PostTick(::std::size_t delta) {}

::std::vector<const char*> System::GetLayersList() {
  const ::std::vector<::vk::LayerProperties> support_layers_list = ::vk::enumerateInstanceLayerProperties();
  ::std::unordered_set<::std::string> support_layers_set;
  for (const ::vk::LayerProperties& properties : support_layers_list) {
    support_layers_set.insert(properties.layerName);
  }

  if (::std::ranges::all_of(config::kRequiredLayers, [&support_layers_set](const char* layer_name) {
        return support_layers_set.contains(layer_name);
      })) {
    enable_validation_layers_ = true;
    return config::kRequiredLayers;
  }
  return ::std::vector<const char*>{};
}

::std::vector<const char*> System::GetExtensionsList() {
  uint32_t extensions_count = 0;
  const char** extensions = ::glfwGetRequiredInstanceExtensions(&extensions_count);

  ::std::vector<const char*> extensions_list = config::kRequiredExtensions;
  for (::std::size_t i = 0; i < extensions_count; ++i) {
    extensions_list.push_back(extensions[i]);
  }

  if (enable_validation_layers_) {
    extensions_list.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

#ifdef __APPLE__
  extensions_list.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  extensions_list.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif

  return extensions_list;
}

::std::vector<const char*> System::GetDeviceExtensionsList() {
  ::std::vector<const char*> extensions_list = config::kRequiredDeviceExtensions;
#ifdef __APPLE__
  extensions_list.push_back("VK_KHR_portability_subset");
#endif

  return extensions_list;
}

void System::CreateDebugMessenger() {
  ::vk::DebugUtilsMessengerCreateInfoEXT create_info(
      {},
      ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
          ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
      ::vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | ::vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
          ::vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
      reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(&System::DebugCallback));
  messenger_ = instance_.createDebugUtilsMessengerEXT(create_info, nullptr, dispatcher_);
}

void System::DestroyDebugMessenger() { instance_.destroyDebugUtilsMessengerEXT(messenger_, nullptr, dispatcher_); }

VKAPI_ATTR ::vk::Bool32 VKAPI_CALL System::DebugCallback(::vk::DebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                         ::vk::DebugUtilsMessageTypeFlagBitsEXT message_type,
                                                         const ::vk::DebugUtilsMessengerCallbackDataEXT* callback_data,
                                                         void* user_data) {
  if (message_severity >= ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning) [[unlikely]] {
    ::std::cerr << callback_data->pMessage << ::std::endl;
  }
  return ::vk::False;
}

::vk::PhysicalDevice System::ChoosePhysicsDevice(const ::vk::Instance& instance) {
  ::std::vector<::vk::PhysicalDevice> device_list = instance.enumeratePhysicalDevices();

  ::vk::PhysicalDevice integrated_gpu;
  for (const ::vk::PhysicalDevice& device : device_list) {
    ::std::vector<::vk::QueueFamilyProperties> queue_family_properties_list = device.getQueueFamilyProperties();
    if (::std::ranges::all_of(queue_family_properties_list, [](const ::vk::QueueFamilyProperties& properties) {
          return static_cast<uint32_t>(properties.queueFlags & ::vk::QueueFlagBits::eGraphics) == 0;
        })) {
      continue;
    }
    if (!IsDeviceExtensionsSupport(device)) {
      continue;
    }
    ::vk::PhysicalDeviceProperties properties = device.getProperties();
    if (properties.deviceType == ::vk::PhysicalDeviceType::eDiscreteGpu) {
      return device;  // prioritize discrete gpu
    }
    if (properties.deviceType == ::vk::PhysicalDeviceType::eIntegratedGpu) {
      integrated_gpu = device;
    }
  }

  return integrated_gpu;
}

::std::uint32_t System::GetGraphicsQueueFamilyIndex(const ::vk::PhysicalDevice& physics_device) {
  ::std::vector<::vk::QueueFamilyProperties> properties_list = physics_device_.getQueueFamilyProperties();
  return static_cast<::std::uint32_t>(std::distance(
      properties_list.begin(),
      ::std::ranges::find_if(properties_list, [&properties_list](const ::vk::QueueFamilyProperties& properties) {
        return static_cast<uint32_t>(properties.queueFlags & ::vk::QueueFlagBits::eGraphics) != 0;
      })));
}

::std::uint32_t System::GetPresentQueueFamilyIndex(const ::vk::PhysicalDevice& physics_device,
                                                   const ::vk::SurfaceKHR& surface) {
  ::std::vector<::vk::QueueFamilyProperties> properties_list = physics_device_.getQueueFamilyProperties();
  ::std::uint32_t index = 0;
  for (; index < static_cast<::std::uint32_t>(properties_list.size()); ++index) {
    if (physics_device.getSurfaceSupportKHR(index, surface) == ::vk::True) {
      break;
    }
  }
  return index;
}

::vk::Device System::CreateLogicDevice(const ::vk::PhysicalDevice& physics_device, ::std::uint32_t graphics_queue_index,
                                       ::std::uint32_t present_queue_index) {
  float queue_priorities = 1.0f;
  ::std::vector<::vk::DeviceQueueCreateInfo> queue_create_infos;
  if (graphics_queue_index == present_queue_index) {
    queue_create_infos.emplace_back(::vk::DeviceQueueCreateInfo({}, graphics_queue_index, 1, &queue_priorities));
  } else {
    queue_create_infos.emplace_back(::vk::DeviceQueueCreateInfo({}, graphics_queue_index, 1, &queue_priorities));
    queue_create_infos.emplace_back(::vk::DeviceQueueCreateInfo({}, present_queue_index, 1, &queue_priorities));
  }
  ::vk::PhysicalDeviceFeatures device_features{};
  ::std::vector<const char*> layers_list = GetLayersList();
  ::std::vector<const char*> extensions_list = GetDeviceExtensionsList();
  ::vk::DeviceCreateInfo device_create_info({}, static_cast<::std::uint32_t>(queue_create_infos.size()),
                                            queue_create_infos.data(), static_cast<::std::uint32_t>(layers_list.size()),
                                            layers_list.data(), static_cast<::std::uint32_t>(extensions_list.size()),
                                            extensions_list.data(), &device_features);
  return physics_device.createDevice(device_create_info);
}

bool System::IsDeviceExtensionsSupport(const ::vk::PhysicalDevice& physics_device) {
  ::std::unordered_set<::std::string_view> require_list(config::kRequiredDeviceExtensions.begin(),
                                                        config::kRequiredDeviceExtensions.end());
  for (const ::vk::ExtensionProperties& properties : physics_device.enumerateDeviceExtensionProperties()) {
    if (require_list.empty()) break;
    require_list.erase(properties.extensionName);
  }
  return require_list.empty();
}

::vk::Extent2D System::ChooseSwapExtent(const ::vk::SurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != ::std::numeric_limits<::std::uint32_t>::max()) {
    return capabilities.currentExtent;
  }
  return ::vk::Extent2D(::std::clamp(capabilities.minImageExtent.width, capabilities.maxImageExtent.width,
                                     static_cast<::std::uint32_t>(canvas_->Size().width)),
                        ::std::clamp(capabilities.minImageExtent.height, capabilities.maxImageExtent.height,
                                     static_cast<::std::uint32_t>(canvas_->Size().height)));
}

::vk::SurfaceFormatKHR System::ChooseSwapSurfaceFormat(const ::std::vector<::vk::SurfaceFormatKHR>& available_formats) {
  if (available_formats.size() == 1 && available_formats[0].format == ::vk::Format::eUndefined) [[unlikely]] {
    return ::vk::SurfaceFormatKHR(::vk::Format::eB8G8R8A8Unorm, ::vk::ColorSpaceKHR::eSrgbNonlinear);
  }

  // Prefer using eB8G8R8A8Unorm, eSrgbNonlinear format
  if (std::ranges::find_if(available_formats, [](const ::vk::SurfaceFormatKHR format) {
        return format.format == ::vk::Format::eB8G8R8A8Unorm &&
               format.colorSpace == ::vk::ColorSpaceKHR::eSrgbNonlinear;
      }) != available_formats.end()) [[likely]] {
    return ::vk::SurfaceFormatKHR(::vk::Format::eB8G8R8A8Unorm, ::vk::ColorSpaceKHR::eSrgbNonlinear);
  }

  return available_formats[0];
}

::vk::PresentModeKHR System::ChooseSwapPresentMode(const ::std::vector<::vk::PresentModeKHR>& available_modes) {
  ::vk::PresentModeKHR best_mode = ::vk::PresentModeKHR::eFifo;
  for (::vk::PresentModeKHR mode : available_modes) {
    if (mode == ::vk::PresentModeKHR::eMailbox) {
      return ::vk::PresentModeKHR::eMailbox;
    } else if (mode == ::vk::PresentModeKHR::eImmediate) {
      return ::vk::PresentModeKHR::eImmediate;
    }
  }
  return best_mode;
}

::vk::SwapchainKHR System::CreateSwapchain(const ::vk::Device& device, const ::vk::SurfaceKHR& surface,
                                           const ::vk::SurfaceCapabilitiesKHR& capabilities,
                                           const ::vk::Extent2D& extent, const ::vk::SurfaceFormatKHR& surface_format,
                                           ::std::uint32_t graphics_queue_index, ::std::uint32_t present_queue_index) {
  ::vk::PresentModeKHR present_mode = ChooseSwapPresentMode(physics_device_.getSurfacePresentModesKHR(surface_));
  ::std::uint32_t image_count = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount) {
    image_count = capabilities.maxImageCount;
  }

  ::vk::SharingMode sharing_mode = ::vk::SharingMode::eExclusive;
  ::std::vector<::std::uint32_t> indices{graphics_queue_index};
  if (graphics_queue_index != present_queue_index) {
    sharing_mode = ::vk::SharingMode::eConcurrent;
    indices.push_back(present_queue_index);
  }
  ::vk::SwapchainCreateInfoKHR create_info({}, surface, image_count, surface_format.format, surface_format.colorSpace,
                                           extent, 1, ::vk::ImageUsageFlagBits::eColorAttachment, sharing_mode, indices,
                                           capabilities.currentTransform, ::vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                           present_mode, ::vk::True);
  return device.createSwapchainKHR(create_info);
}

::std::vector<::vk::ImageView> System::CreateImageViews(const ::vk::Device& device,
                                                        const ::std::vector<::vk::Image>& images,
                                                        const ::vk::SurfaceFormatKHR& surface_format) {
  ::std::vector<::vk::ImageView> image_views(images.size());
  for (::std::size_t i = 0; i < images.size(); ++i) {
    ::vk::ComponentMapping components(::vk::ComponentSwizzle::eIdentity, ::vk::ComponentSwizzle::eIdentity,
                                      ::vk::ComponentSwizzle::eIdentity, ::vk::ComponentSwizzle::eIdentity);
    ::vk::ImageSubresourceRange subresource_range(::vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    ::vk::ImageViewCreateInfo create_info({}, images[i], ::vk::ImageViewType::e2D, surface_format.format, components,
                                          subresource_range);
    image_views.emplace_back(device.createImageView(create_info));
  }
  return image_views;
}

::vk::ShaderModule System::CreateShaderModule(const ::vk::Device& device, const ::std::filesystem::path& sprv_path) {
  std::ifstream sprv_file(sprv_path, ::std::ios::binary);
  std::vector<char> sprv_data(std::istreambuf_iterator<char>(sprv_file), {});

  ::vk::ShaderModuleCreateInfo create_info({}, sprv_data.size(),
                                           reinterpret_cast<const ::std::uint32_t*>(sprv_data.data()));
  return device.createShaderModule(create_info);
}

::vk::PipelineLayout System::CreatePipelineLayout(const ::vk::Device& device) {
  ::vk::PipelineLayoutCreateInfo pipeline_layout_info({}, 0, nullptr, 0, nullptr);
  return device.createPipelineLayout(pipeline_layout_info);
}

::vk::RenderPass System::CreateRenderPass(const ::vk::Device& device, const ::vk::Format& format) {
  ::vk::AttachmentDescription attachment_description(
      {}, format, ::vk::SampleCountFlagBits::e1, ::vk::AttachmentLoadOp::eClear, ::vk::AttachmentStoreOp::eStore,
      ::vk::AttachmentLoadOp::eDontCare, ::vk::AttachmentStoreOp::eDontCare, ::vk::ImageLayout::eUndefined,
      ::vk::ImageLayout::ePresentSrcKHR);
  ::vk::AttachmentReference attachment_reference(0, ::vk::ImageLayout::eColorAttachmentOptimal);

  ::vk::SubpassDescription subpass_description({}, ::vk::PipelineBindPoint::eGraphics, 0, nullptr, 1,
                                               &attachment_reference);

  ::vk::RenderPassCreateInfo render_pass_info({}, 1, &attachment_description, 1, &subpass_description);
  return device.createRenderPass(render_pass_info);
}

::vk::Pipeline System::CreatePipeline(const ::vk::Device& device, const ::std::filesystem::path& vert_sprv_path,
                                      const ::std::filesystem::path& frag_sprv_path, const ::vk::Extent2D& extent,
                                      const ::vk::PipelineLayout& pipeline_layout,
                                      const ::vk::RenderPass& render_pass) {
  ::vk::ShaderModule vert_shader = CreateShaderModule(device, vert_sprv_path);
  ::vk::ShaderModule frag_shader = CreateShaderModule(device, frag_sprv_path);
  std::vector<::vk::PipelineShaderStageCreateInfo> shader_stage_infos{
      ::vk::PipelineShaderStageCreateInfo({}, ::vk::ShaderStageFlagBits::eVertex, vert_shader, "main"),
      ::vk::PipelineShaderStageCreateInfo({}, ::vk::ShaderStageFlagBits::eFragment, frag_shader, "main")};

  ::vk::PipelineVertexInputStateCreateInfo vert_input_info({}, 0, nullptr, 0, nullptr);
  ::vk::PipelineInputAssemblyStateCreateInfo input_assembly_info({}, ::vk::PrimitiveTopology::eTriangleList,
                                                                 ::vk::False);
  ::vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(extent.width), static_cast<float>(extent.height), 0.0, 1.0f);
  ::vk::Rect2D scissor(::vk::Offset2D(0, 0), extent);
  ::vk::PipelineViewportStateCreateInfo viewport_state_info({}, 1, &viewport, 1, &scissor);
  ::vk::PipelineRasterizationStateCreateInfo rasterization_state_info(
      {}, ::vk::False, ::vk::False, ::vk::PolygonMode::eFill, ::vk::CullModeFlagBits::eBack,
      ::vk::FrontFace::eClockwise, ::vk::False, 0.0f, 0.0f, 0.0f, 1.0f);
  ::vk::PipelineMultisampleStateCreateInfo multisample_state_info({}, ::vk::SampleCountFlagBits::e1, ::vk::False, 1.0f,
                                                                  nullptr, ::vk::False, ::vk::False);
  ::vk::PipelineColorBlendAttachmentState blend_attachment_state(
      ::vk::False, ::vk::BlendFactor::eOne, ::vk::BlendFactor::eZero, ::vk::BlendOp::eAdd, ::vk::BlendFactor::eOne,
      ::vk::BlendFactor::eZero, ::vk::BlendOp::eAdd,
      ::vk::ColorComponentFlagBits::eR | ::vk::ColorComponentFlagBits::eG | ::vk::ColorComponentFlagBits::eB |
          ::vk::ColorComponentFlagBits::eA);
  ::vk::PipelineColorBlendStateCreateInfo blend_state_info({}, ::vk::False, ::vk::LogicOp::eCopy, 1,
                                                           &blend_attachment_state, {0.0f, 0.0f, 0.0f, 0.0f});

  ::std::vector<::vk::DynamicState> dynamic_state{::vk::DynamicState::eViewport, ::vk::DynamicState::eLineWidth};
  ::vk::PipelineDynamicStateCreateInfo dynamic_state_info({}, static_cast<::std::uint32_t>(dynamic_state.size()),
                                                          dynamic_state.data());

  ::vk::GraphicsPipelineCreateInfo pipeline_info({}, shader_stage_infos, &vert_input_info, &input_assembly_info,
                                                 nullptr, &viewport_state_info, &rasterization_state_info,
                                                 &multisample_state_info, nullptr, &blend_state_info,
                                                 &dynamic_state_info, pipeline_layout, render_pass);

  ::vk::Result result;
  ::vk::Pipeline pipeline;
  ::std::tie(result, pipeline) = device.createGraphicsPipeline({}, pipeline_info);
  device.destroyShaderModule(vert_shader);
  device.destroyShaderModule(frag_shader);
  if (result != ::vk::Result::eSuccess) {
    ::std::cerr << "createGraphicsPipeline fail, result code" << static_cast<::std::uint32_t>(result) << ::std::endl;
  }

  return pipeline;
}

}  // namespace atn::engine::render
