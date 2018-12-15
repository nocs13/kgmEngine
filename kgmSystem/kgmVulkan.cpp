#include "kgmVulkan.h"
#include "../kgmBase/kgmLog.h"
#include "kgmSystem.h"

#ifdef VULKAN

#include "inc/vk/vulkan.h"

//https://gist.github.com/Overv/7ac07356037592a121225172d7d78f2d

u32           kgmVulkan::g_vulkans = 0;
kgmLib        kgmVulkan::vk_lib;
kgmVulkan::vk kgmVulkan::m_vk      = {0};

kgmVulkan::kgmVulkan(kgmWindow* wnd)
{
  m_instance = 0;

  if (wnd == nullptr)
    return;

  if (g_vulkans < 1) {
    if (vkInit() != 1 || m_vk.vkCreateInstance == nullptr)
      return;
  }

  g_vulkans++;

  s32 wrc[4];

  wnd->getRect(wrc[0], wrc[1], wrc[2], wrc[3]);

  u32 vk_res;

  VkApplicationInfo appInfo;

  memset(&appInfo, 0, sizeof(VkApplicationInfo));

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "kTest";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  u32 ecount = 0;

  vk_res = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &ecount, nullptr);

  if (ecount == 0)
  {
    kgm_log() << "Vulkan error: No extensions supported!\n";
    return;
  }

  VkExtensionProperties eprops[ecount];
  const char* enames[ecount];

  vk_res = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &ecount, eprops);

  u32 iname = 0;

  for (const auto& ep: eprops)
  {
    kgm_log() << "Vulkan info: Extension name " << ep.extensionName << "\n";

    enames[iname++] = ep.extensionName;
  }

  VkInstanceCreateInfo createInfo;

  memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));

  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = ecount;
  createInfo.ppEnabledExtensionNames = enames;

  vk_res = m_vk.vkCreateInstance(&createInfo, null, &m_instance);

  if (vk_res != VK_SUCCESS)
  {
    switch(vk_res)
    {
    case VK_ERROR_LAYER_NOT_PRESENT:
    kgm_log() << "kgmVulkan error: vkCreateInstance no layers\n";
      break;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
      kgm_log() << "kgmVulkan error: vkCreateInstance no extentions\n";
      break;
    default:
      kgm_log() << "kgmVulkan error: vkCreateInstance failed " << vk_res << "\n";
    }

    return;
  }

#ifdef DEBUG

  VkDebugReportCallbackEXT debugReportCallback;
  VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;

  debugReportCallbackCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  debugReportCallbackCreateInfo.pNext = nullptr;
  debugReportCallbackCreateInfo.flags = VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT;
  debugReportCallbackCreateInfo.pfnCallback = [](
          VkDebugReportFlagsEXT flags,
          VkDebugReportObjectTypeEXT objectType,
          uint64_t object,
          size_t location,
          int32_t messageCode,
          const char* pLayerPrefix,
          const char* pMessage,
          void* pUserData) -> VkBool32
      {
        kgm_log() << "Vulkan DEBUG: (";

        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0) kgm_log() << "INFO";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0) kgm_log() << "WARNING";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0) kgm_log() << "PERFORMANCE";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0) kgm_log() << "DEBUG";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0) kgm_log() << "ERROR";
        kgm_log() << ")";
        kgm_log() << "{" << pLayerPrefix << "} " << pMessage << "\n";
        return VK_FALSE;
      };
  debugReportCallbackCreateInfo.pUserData = nullptr;

  if (m_vk.vkCreateDebugReportCallbackEXT && m_vk.vkCreateDebugReportCallbackEXT(m_instance,
      &debugReportCallbackCreateInfo, nullptr, &debugReportCallback)
      != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to create debug collback.\n";

    return;
  }

#endif

  VkSurfaceKHR surface;

#ifdef WIN32
  VkWin32SurfaceCreateInfoKHR sfCreateInfo;
  sfCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  sfCreateInfo.hwnd = wnd->m_wnd;
  sfCreateInfo.hinstance = GetModuleHandle(nullptr);

  vk_res = m_vk.vkCreateWin32SurfaceKHR(m_instance, &sfCreateInfo, nullptr, &surface);
#else
  VkXlibSurfaceCreateInfoKHR sfCreateInfo;
  sfCreateInfo.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
  sfCreateInfo.window = wnd->m_wnd;
  sfCreateInfo.dpy    = wnd->m_dpy;

  vk_res = m_vk.vkCreateXlibSurfaceKHR(m_instance, &sfCreateInfo, nullptr, &surface);
#endif

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: vkCreate**SurfaceKHR " << vk_res << "\n";

    return;
  }

  m_surface = surface;

  u32 dcount = 0;

  vk_res = m_vk.vkEnumeratePhysicalDevices(m_instance, &dcount, nullptr);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: vkEnumeratePhysicalDevices " << vk_res << "\n";

    return;
  }

  VkPhysicalDevice devs[dcount];

  vk_res = m_vk.vkEnumeratePhysicalDevices(m_instance, &dcount, devs);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: vkEnumeratePhysicalDevices no devs\n";

    return;
  }

  u32 decount = 0;

  vk_res = m_vk.vkEnumerateDeviceExtensionProperties(devs[0], nullptr, &decount, nullptr);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: vkEnumerateDeviceExtensionProperties " << vk_res << "\n";

    return;
  }

  VkPhysicalDevice physicalDevice = devs[0];

  m_physicalDevice = physicalDevice;

  VkExtensionProperties dexsts[decount];

  vk_res = m_vk.vkEnumerateDeviceExtensionProperties(devs[0], nullptr, &decount, dexsts);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: vkEnumerateDeviceExtensionProperties no extentions\n";

    return;
  }

  bool isswapchain = false;

  for (const auto dexst: dexsts)
  {
    if (strcmp(dexst.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
    {
      isswapchain = true;

      break;
    }

    kgm_log() << "kgmVulkan error: vkEnumerateDeviceExtensionProperties " << dexst.extensionName << "\n";
  }

  if (!isswapchain)
  {
    kgm_log() << "kgmVulkan error: vkEnumerateDeviceExtensionProperties no swap chain!\n";

    return;
  }

  VkSurfaceCapabilitiesKHR surfaceCapabilities;

  if(m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilities) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: failed to get surface capabilities.\n";

    return;
  }

  u32 queueFamilyCount = 0;

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(devs[0], &queueFamilyCount, nullptr);

  if (queueFamilyCount < 1)
  {
    kgm_log() << "kgmVulkan error: vkGetPhysicalDeviceQueueFamilyProperties no count\n";

    return;
  }

  VkQueueFamilyProperties queueFamilies[queueFamilyCount];

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(devs[0], &queueFamilyCount, queueFamilies);

  if (queueFamilyCount < 1)
  {
    kgm_log() << "kgmVulkan error: vkGetPhysicalDeviceQueueFamilyProperties no families\n";

    return;
  }

  bool hasGraphicsQueueFamily = false;
  bool hasPresentQueueFamily = false;

  u32 graphicsQueueFamily = -1, presentQueueFamily = -1;

  for (u32 i = 0; i < queueFamilyCount; i++)
  {
    VkBool32 presentSupport = false;
    m_vk.vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

    if (queueFamilies[i].queueCount > 0
        && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      graphicsQueueFamily = i;
      hasGraphicsQueueFamily = true;

      if (presentSupport)
      {
        presentQueueFamily = i;
        hasPresentQueueFamily = true;
        break;
      }
    }

    if (!hasPresentQueueFamily && presentSupport)
    {
      presentQueueFamily = i;
      hasPresentQueueFamily = true;
    }
  }

  if (hasGraphicsQueueFamily)
  {
    kgm_log() << "Vulkan: queue family #" << graphicsQueueFamily << " supports graphics.\n";

    if (hasPresentQueueFamily)
    {
      kgm_log() << "Vulkan queue family #" << presentQueueFamily
          << " supports presentation.\n";
    }
    else
    {
      kgm_log() << "Vulkan error: could not find a valid queue family with present support.\n";

      return;
    }
  }
  else
  {
    kgm_log() << "Vulkan error: could not find a valid queue family with graphics support.\n";

    return;
  }

  u32 surfaceFormatsCount = 0;

  if(m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, nullptr) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get surface formats count.\n";

    return;
  }

  VkSurfaceFormatKHR surfaceFormats[surfaceFormatsCount];

  if(m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, surfaceFormats) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get surface formats.\n";

    return;
  }

  f32 queuePriority = 1.0f;

  VkDeviceQueueCreateInfo queueCreateInfo[2] = {};

  queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo[0].queueFamilyIndex = graphicsQueueFamily;
  queueCreateInfo[0].queueCount = 1;
  queueCreateInfo[0].pQueuePriorities = &queuePriority;

  queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo[0].queueFamilyIndex = presentQueueFamily;
  queueCreateInfo[0].queueCount = 1;
  queueCreateInfo[0].pQueuePriorities = &queuePriority;

  VkDeviceCreateInfo deviceCreateInfo = {};

  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;

  if (graphicsQueueFamily == presentQueueFamily)
  {
    deviceCreateInfo.queueCreateInfoCount = 1;
  }
  else
  {
    deviceCreateInfo.queueCreateInfoCount = 2;
  }

  // Necessary for shader (for some reason)
  VkPhysicalDeviceFeatures enabledFeatures = {0};

  enabledFeatures.shaderClipDistance = VK_TRUE;
  enabledFeatures.shaderCullDistance = VK_TRUE;

  const char* deviceExtensions = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

  deviceCreateInfo.enabledExtensionCount = 1;
  deviceCreateInfo.ppEnabledExtensionNames = &deviceExtensions;
  deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

  VkDevice device;

  if (m_vk.vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan: failed to create logical device\n";

    return;
  }

  kgm_log() << "Vulkan: created logical device.\n";

  VkQueue graphicsQueue, presentQueue;

  // Get graphics and presentation queues (which may be the same)
  m_vk.vkGetDeviceQueue(device, graphicsQueueFamily, 0, &graphicsQueue);
  m_vk.vkGetDeviceQueue(device, presentQueueFamily, 0, &presentQueue);

  kgm_log() << "Vulkan: acquired graphics and presentation queues.\n";

  m_device = device;

  u32 ddim[2];

  kgmSystem::getDesktopDimension(ddim[0], ddim[1]);

  VkSwapchainKHR swapChain;

  VkSwapchainCreateInfoKHR swapChainCreateInfo;

  swapChainCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapChainCreateInfo.pNext = nullptr;
  swapChainCreateInfo.flags = 0;
  swapChainCreateInfo.surface = surface;
  swapChainCreateInfo.minImageCount = 2;
  swapChainCreateInfo.imageFormat = surfaceFormats[0].format;
  swapChainCreateInfo.imageColorSpace = surfaceFormats[0].colorSpace;
  swapChainCreateInfo.imageExtent = VkExtent2D{ddim[0], ddim[1]};
  swapChainCreateInfo.imageArrayLayers = 1;
  swapChainCreateInfo.imageUsage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapChainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
  swapChainCreateInfo.queueFamilyIndexCount = 0;
  swapChainCreateInfo.pQueueFamilyIndices = nullptr;
  swapChainCreateInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  swapChainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapChainCreateInfo.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
  swapChainCreateInfo.clipped = VK_FALSE;
  swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

  if(m_vk.vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapChain) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to create swapchain.\n";

    return;
  }

  m_swapChain = swapChain;

  kgm_log() << "Vulkan: created swap chain.\n";

  u32 swapChainImagesCount;

  if(m_vk.vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, nullptr) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get swapchain images count.\n";

    return;
  }

  m_swapChainImages.alloc(swapChainImagesCount);

  if(m_vk.vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, m_swapChainImages.data()) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get swapchain images.\n";

    return;
  }

  VkRenderPass renderPass;

  VkAttachmentDescription attachmentDescription;
  attachmentDescription.flags = 0;
  attachmentDescription.format = surfaceFormats[0].format;
  attachmentDescription.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
  attachmentDescription.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachmentDescription.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
  attachmentDescription.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachmentDescription.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachmentDescription.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
  attachmentDescription.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference attachmentReference;
  attachmentReference.attachment = 0;
  attachmentReference.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpassDescription;
  subpassDescription.flags = 0;
  subpassDescription.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpassDescription.inputAttachmentCount = 0;
  subpassDescription.pInputAttachments = nullptr;
  subpassDescription.colorAttachmentCount = 1;
  subpassDescription.pColorAttachments = &attachmentReference;
  subpassDescription.pResolveAttachments = nullptr;
  subpassDescription.pDepthStencilAttachment = nullptr;
  subpassDescription.preserveAttachmentCount = 0;
  subpassDescription.pPreserveAttachments = nullptr;

  VkRenderPassCreateInfo renderPassCreateInfo;
  renderPassCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassCreateInfo.pNext = nullptr;
  renderPassCreateInfo.flags = 0;
  renderPassCreateInfo.attachmentCount = 1;
  renderPassCreateInfo.pAttachments = &attachmentDescription;
  renderPassCreateInfo.subpassCount = 1;
  renderPassCreateInfo.pSubpasses = &subpassDescription;
  renderPassCreateInfo.dependencyCount = 0;
  renderPassCreateInfo.pDependencies = nullptr;

  if(m_vk.vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass) != VkResult::VK_SUCCESS)
  {
      kgm_log() << "Vulkan error: failed to create render pass.\n";

      return;
  }

  kgm_log() << "Vulkan: created render pass.\n";

  m_renderPass = renderPass;

  m_framebuffers.alloc(m_swapChainImages.length());

  m_imageViews.alloc(m_swapChainImages.length());

  for (u32 i = 0; i < m_swapChainImages.length(); i++)
  {
    auto& imageView = m_imageViews[i];

    VkImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = m_swapChainImages[i];
    imageViewCreateInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = surfaceFormats[0].format;

    imageViewCreateInfo.components = {
      VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
      VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
      VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
      VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A
    };

    imageViewCreateInfo.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;

    if(m_vk.vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &imageView) != VkResult::VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: failed to create image view\n";

      return;
    }

    auto &framebuffer = m_framebuffers[i];

    VkFramebufferCreateInfo framebufferCreateInfo;

    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.pNext = nullptr;
    framebufferCreateInfo.flags = 0;
    framebufferCreateInfo.renderPass = renderPass;
    framebufferCreateInfo.attachmentCount = 1;
    framebufferCreateInfo.pAttachments = &imageView;
    framebufferCreateInfo.width = surfaceCapabilities.currentExtent.width;
    framebufferCreateInfo.height = surfaceCapabilities.currentExtent.height;
    framebufferCreateInfo.layers = 1;


    if(m_vk.vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr, &framebuffer) != VkResult::VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: failed to create framebuffer\n";

      return;
    }
  }

  kgm_log() << "Vulkan: created framebuffer and image views.\n";

  VkQueue queue;

  m_vk.vkGetDeviceQueue(m_device, 0, 0, &queue);

  if(m_vk.vkQueueWaitIdle(queue) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for queue.\n";

    return;
  }

  m_queue = queue;

  VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

  m_vk.vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

  /*VkSemaphore imageAvailableSemaphore, renderingFinishedSemaphore;
  VkSemaphoreCreateInfo semaphoreCreateInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if (m_vk.vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
      m_vk.vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderingFinishedSemaphore) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan: failed to create semaphores.\n";

    return;
  }

  kgm_log() << "Vulkan: created semaphores.\n";*/

  VkCommandPool commandPool;

  VkCommandPoolCreateInfo poolCreateInfo = {};
  poolCreateInfo.flags = VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolCreateInfo.pNext = null;
  poolCreateInfo.queueFamilyIndex = graphicsQueueFamily;

  if (m_vk.vkCreateCommandPool(device, &poolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to create command queue for graphics queue family.\n";

    return;
  }

  if(m_vk.vkResetCommandPool(m_device, commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to reset command pool.\n";

    return;
  }

  kgm_log() << "Vulkan: created/reseted command pool for graphics queue family.\n";

  m_commandPool = commandPool;

  m_commandBuffers.alloc(m_swapChainImages.length());

  VkCommandBufferAllocateInfo commandBufferAllocateInfo;
  commandBufferAllocateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocateInfo.pNext = nullptr;
  commandBufferAllocateInfo.commandPool = commandPool;
  commandBufferAllocateInfo.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocateInfo.commandBufferCount = m_swapChainImages.length();

  if(m_vk.vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, m_commandBuffers) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to allocate command buffers\n";

    return;
  }

  for(u32 i = 0; i < m_swapChainImages.length(); ++i)
  {
    auto &commandBuffer = m_commandBuffers[i];
    auto &image = m_swapChainImages[i];
    auto &framebuffer = m_framebuffers[i];

    if(m_vk.vkResetCommandBuffer(commandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VkResult::VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: failed to reset command buffer\n";

      return;
    }

    VkCommandBufferInheritanceInfo commandBufferInheritanceInfo;

    commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    commandBufferInheritanceInfo.pNext = nullptr;
    commandBufferInheritanceInfo.renderPass = renderPass;
    commandBufferInheritanceInfo.subpass = 0;
    commandBufferInheritanceInfo.framebuffer = framebuffer;
    commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
    commandBufferInheritanceInfo.queryFlags = 0;
    commandBufferInheritanceInfo.pipelineStatistics = 0;

    VkCommandBufferBeginInfo commandBufferBeginInfo;

    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = nullptr;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = &commandBufferInheritanceInfo;

    m_vk.vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

    VkClearValue clearValue;

    clearValue.color.float32[0] = 1.0f;
    clearValue.color.float32[1] = 0.0f;
    clearValue.color.float32[2] = 0.0f;
    clearValue.color.float32[3] = 1.0f;

    VkRenderPassBeginInfo renderPassBeginInfo;

    renderPassBeginInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = m_framebuffers[i];
    renderPassBeginInfo.renderArea = VkRect2D {
      VkOffset2D  {0, 0},
      VkExtent2D  {
        (u32) wrc[2], (u32) wrc[3]
        //surfaceCapabilities.currentExtent.width,
        //surfaceCapabilities.currentExtent.height
      }
    };
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;

    m_vk.vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
    m_vk.vkCmdEndRenderPass(commandBuffer);

    VkImageMemoryBarrier imageMemoryBarrier;
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = nullptr;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask =
        VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
        VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT;
    imageMemoryBarrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.srcQueueFamilyIndex = 0;
    imageMemoryBarrier.dstQueueFamilyIndex = 0;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = {
      VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1
    };

    m_vk.vkCmdPipelineBarrier(commandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, null, 0, null,
                              1, &imageMemoryBarrier);

    m_vk.vkEndCommandBuffer(commandBuffer);
  }

  kgm_log() << "Vulkan: Filled command buffers.\n";

  VkFence fence;

  VkFenceCreateInfo fenceCreateInfo;
  fenceCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.pNext = nullptr;
  fenceCreateInfo.flags = 0;

  if (m_vk.vkCreateFence(m_device, &fenceCreateInfo, nullptr, &fence)
      != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to create fence.\n";

    return;
  }

  m_fence = fence;

  /* u32 swapChainImage;

  if(m_vk.vkAcquireNextImageKHR(m_device, swapChain, UINT64_MAX, VK_NULL_HANDLE, fence, &swapChainImage) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get next swapchain image.\n";

    return;
  }

  m_swapChainImage = swapChainImage;

  kgm_log() << "Vulkan: got next swap chain image.\n";

  if(m_vk.vkWaitForFences(m_device, 1, &fence, VK_TRUE, UINT64_MAX) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for fence.\n";

    return;
  }

  if(m_vk.vkResetFences(m_device, 1, &fence) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to reset fence.\n";

    return;
  }

  auto &commandBuffer = m_commandBuffers[swapChainImage];

  VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  VkSubmitInfo submitInfo;

  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = nullptr;
  submitInfo.waitSemaphoreCount = 0;
  submitInfo.pWaitSemaphores = nullptr;
  submitInfo.pWaitDstStageMask = &waitMask;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.pSignalSemaphores = nullptr;

  if(m_vk.vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to submit command buffer.\n";

    return;
  }

  if(m_vk.vkQueueWaitIdle(queue) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for queue.\n";

    return;
  }

  VkResult result;
  VkPresentInfoKHR presentInfo;

  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;
  presentInfo.waitSemaphoreCount = 0;
  presentInfo.pWaitSemaphores = nullptr;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &swapChain;
  presentInfo.pImageIndices = &swapChainImage;
  presentInfo.pResults = &result;


  if(m_vk.vkQueuePresentKHR(queue, &presentInfo) != VkResult::VK_SUCCESS || result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to present swapchain.\n";

    return;
  } */

  kgm_log() << "Vulkan: Successfully prepared.\n";
}

kgmVulkan::~kgmVulkan()
{
  if (m_instance)
    m_vk.vkDestroyInstance(m_instance, null);

  g_vulkans--;

  if (g_vulkans < 1)
    vkFree();
}

int kgmVulkan::vkInit()
{
  if (vk_lib.active())
    return -1;

#ifdef WIN32
  if (!vk_lib.open((char*) "vulkan.dll"))
    if (!vk_lib.open((char*) "vulkan-1.dll"))
      return -2;
#else
  if (!vk_lib.open((char*) "libvulkan.so"))
    if (!vk_lib.open((char*) "libvulkan.so.1"))
      return -2;
#endif

  #pragma GCC diagnostic push
  //#pragma GCC diagnostic ignored "-fpermissive"

  m_vk.vkCreateInstance = (typeof m_vk.vkCreateInstance) vk_lib.get((char*) "vkCreateInstance");
  m_vk.vkDestroyInstance = (typeof m_vk.vkDestroyInstance) vk_lib.get((char*) "vkDestroyInstance");
  m_vk.vkEnumerateInstanceExtensionProperties = (typeof m_vk.vkEnumerateInstanceExtensionProperties) vk_lib.get((char*) "vkEnumerateInstanceExtensionProperties");
  m_vk.vkEnumeratePhysicalDevices = (typeof m_vk.vkEnumeratePhysicalDevices) vk_lib.get((char*) "vkEnumeratePhysicalDevices");
  m_vk.vkEnumerateDeviceExtensionProperties = (typeof m_vk.vkEnumerateDeviceExtensionProperties) vk_lib.get((char*) "vkEnumerateDeviceExtensionProperties");
  m_vk.vkGetPhysicalDeviceQueueFamilyProperties = (typeof m_vk.vkGetPhysicalDeviceQueueFamilyProperties) vk_lib.get((char*) "vkGetPhysicalDeviceQueueFamilyProperties");
  m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
  m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceFormatsKHR");
  m_vk.vkGetPhysicalDeviceSurfaceSupportKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceSupportKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceSupportKHR");
  m_vk.vkCreateDevice = (typeof m_vk.vkCreateDevice) vk_lib.get((char*) "vkCreateDevice");
  m_vk.vkGetDeviceQueue = (typeof m_vk.vkGetDeviceQueue) vk_lib.get((char*) "vkGetDeviceQueue");
  m_vk.vkQueueWaitIdle = (typeof m_vk.vkQueueWaitIdle) vk_lib.get((char*) "vkQueueWaitIdle");
  m_vk.vkGetPhysicalDeviceMemoryProperties = (typeof m_vk.vkGetPhysicalDeviceMemoryProperties) vk_lib.get((char*) "vkGetPhysicalDeviceMemoryProperties");
  m_vk.vkCreateSwapchainKHR = (typeof m_vk.vkCreateSwapchainKHR) vk_lib.get((char*) "vkCreateSwapchainKHR");
  m_vk.vkGetSwapchainImagesKHR = (typeof m_vk.vkGetSwapchainImagesKHR) vk_lib.get((char*) "vkGetSwapchainImagesKHR");
  m_vk.vkCreateSemaphore = (typeof m_vk.vkCreateSemaphore) vk_lib.get((char*) "vkCreateSemaphore");
  m_vk.vkCreateCommandPool = (typeof m_vk.vkCreateCommandPool) vk_lib.get((char*) "vkCreateCommandPool");
  m_vk.vkResetCommandPool = (typeof m_vk.vkResetCommandPool) vk_lib.get((char*) "vkResetCommandPool");
  m_vk.vkCreateRenderPass = (typeof m_vk.vkCreateRenderPass) vk_lib.get((char*) "vkCreateRenderPass");
  m_vk.vkCmdBeginRenderPass = (typeof m_vk.vkCmdBeginRenderPass) vk_lib.get((char*) "vkCmdBeginRenderPass");
  m_vk.vkCmdEndRenderPass = (typeof m_vk.vkCmdEndRenderPass) vk_lib.get((char*) "vkCmdEndRenderPass");
  m_vk.vkCreateImageView = (typeof m_vk.vkCreateImageView) vk_lib.get((char*) "vkCreateImageView");
  m_vk.vkCreateFramebuffer = (typeof m_vk.vkCreateFramebuffer) vk_lib.get((char*) "vkCreateFramebuffer");
  m_vk.vkAllocateCommandBuffers = (typeof m_vk.vkAllocateCommandBuffers) vk_lib.get((char*) "vkAllocateCommandBuffers");
  m_vk.vkBeginCommandBuffer = (typeof m_vk.vkBeginCommandBuffer) vk_lib.get((char*) "vkBeginCommandBuffer");
  m_vk.vkResetCommandBuffer = (typeof m_vk.vkResetCommandBuffer) vk_lib.get((char*) "vkResetCommandBuffer");
  m_vk.vkEndCommandBuffer = (typeof m_vk.vkEndCommandBuffer) vk_lib.get((char*) "vkEndCommandBuffer");
  m_vk.vkCmdPipelineBarrier = (typeof m_vk.vkCmdPipelineBarrier) vk_lib.get((char*) "vkCmdPipelineBarrier");
  m_vk.vkCreateFence = (typeof m_vk.vkCreateFence) vk_lib.get((char*) "vkCreateFence");
  m_vk.vkResetFences = (typeof m_vk.vkResetFences) vk_lib.get((char*) "vkResetFences");
  m_vk.vkWaitForFences = (typeof m_vk.vkWaitForFences) vk_lib.get((char*) "vkWaitForFences");
  m_vk.vkAcquireNextImageKHR = (typeof m_vk.vkAcquireNextImageKHR) vk_lib.get((char*) "vkAcquireNextImageKHR");
  m_vk.vkQueueSubmit = (typeof m_vk.vkQueueSubmit) vk_lib.get((char*) "vkQueueSubmit");
  m_vk.vkQueuePresentKHR = (typeof m_vk.vkQueuePresentKHR) vk_lib.get((char*) "vkQueuePresentKHR");
  m_vk.vkDestroyPipeline = (typeof m_vk.vkDestroyPipeline) vk_lib.get((char*) "vkDestroyPipeline");
  m_vk.vkDestroyRenderPass = (typeof m_vk.vkDestroyRenderPass) vk_lib.get((char*) "vkDestroyRenderPass");
  m_vk.vkDestroyFramebuffer = (typeof m_vk.vkDestroyFramebuffer) vk_lib.get((char*) "vkDestroyFramebuffer");
  m_vk.vkDestroyImageView = (typeof m_vk.vkDestroyImageView) vk_lib.get((char*) "vkDestroyImageView");
  m_vk.vkDestroyDescriptorSetLayout = (typeof m_vk.vkDestroyDescriptorSetLayout) vk_lib.get((char*) "vkDestroyDescriptorSetLayout");
  m_vk.vkFreeCommandBuffers = (typeof m_vk.vkFreeCommandBuffers) vk_lib.get((char*) "vkFreeCommandBuffers");

  m_vk.vkCreateDebugReportCallbackEXT = (typeof m_vk.vkCreateDebugReportCallbackEXT) vk_lib.get((char*) "vkCreateDebugReportCallbackEXT");

#ifdef WIN32
  m_vk.vkCreateWin32SurfaceKHR = (typeof m_vk.vkCreateWin32SurfaceKHR) vk_lib.get((char*) "vkCreateWin32SurfaceKHR");
#else
  m_vk.vkCreateXlibSurfaceKHR = (typeof m_vk.vkCreateXlibSurfaceKHR) vk_lib.get((char*) "vkCreateXlibSurfaceKHR");
#endif

  #pragma GCC diagnostic pop

  return 1;
}

void kgmVulkan::vkFree()
{
  if (vk_lib.active())
    vk_lib.close();
}

u32 kgmVulkan::gcError()
{
  return 0;
}

void  kgmVulkan::gcSet(u32 param, void* value) {}
void  kgmVulkan::gcGet(u32 param, void* value) {}
void  kgmVulkan::gcClear(u32 flag, u32 col, float depth, u32 sten) {}
void  kgmVulkan::gcBegin() {}
void  kgmVulkan::gcEnd() {}

void  kgmVulkan::gcRender()
{
  VkResult result;

  u32 swapChainImage = m_swapChainImage;

  result = m_vk.vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, VK_NULL_HANDLE, m_fence, &swapChainImage);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get next swapchain image.\n";
    fprintf(stderr, "Vulkan error code %X\n", result);

    return;
  }

  result = m_vk.vkQueueWaitIdle(m_queue);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for queue.\n";

    return;
  }

  result = m_vk.vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, 1000);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for fence.\n";

    return;
  }

  result = m_vk.vkResetFences(m_device, 1, &m_fence);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to reset fence.\n";

    return;
  }

  auto &commandBuffer = m_commandBuffers[m_swapChainImage];

  VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  VkSubmitInfo submitInfo;

  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = nullptr;
  submitInfo.waitSemaphoreCount = 0;
  submitInfo.pWaitSemaphores = nullptr;
  submitInfo.pWaitDstStageMask = &waitMask;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.pSignalSemaphores = nullptr;

  result = m_vk.vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to submit command buffer.\n";

    return;
  }

  result = m_vk.vkQueueWaitIdle(m_queue);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for queue.\n";

    return;
  }

  VkPresentInfoKHR presentInfo;

  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;
  presentInfo.waitSemaphoreCount = 0;
  presentInfo.pWaitSemaphores = nullptr;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &m_swapChain;
  presentInfo.pImageIndices = &swapChainImage;
  presentInfo.pResults = &result;


  if(m_vk.vkQueuePresentKHR(m_queue, &presentInfo) != VkResult::VK_SUCCESS || result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to present swapchain.\n";

    return;
  }

  m_swapChainImage = swapChainImage;
}

void  kgmVulkan::gcSetTarget(void*  rt) {}

// DRAWING
void  kgmVulkan::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt) {}

// TEXTURE
void* kgmVulkan::gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type) {}
void  kgmVulkan::gcFreeTexture(void *t) {}
void  kgmVulkan::gcSetTexture(u32 stage, void *t) {}

// TARGET
gchandle kgmVulkan::gcGenTarget(u32 w, u32 h, bool depth, bool stencil){}
bool     kgmVulkan::gcTexTarget(gchandle tar, gchandle tex, u32 type){}
void     kgmVulkan::gcFreeTarget(gchandle t){}

// MATRIX
void  kgmVulkan::gcSetMatrix(u32 mode, float* mtx) {}
void  kgmVulkan::gcGetMatrix(u32 mode, float* mtx) {}

void  kgmVulkan::gcSetViewport(int x, int y, int w, int h, float n, float f)
{
  m_vk.vkDeviceWaitIdle(m_device);

  m_vk.vkFreeCommandBuffers(m_device, m_commandPool, (u32) m_commandBuffers.length(), m_commandBuffers.data());

  //m_vk.vkDestroyPipeline(m_device, graphicsPipeline, nullptr);
  m_vk.vkDestroyRenderPass(m_device, m_renderPass, nullptr);

  for (size_t i = 0; i < m_swapChainImages.length(); i++)
  {
    m_vk.vkDestroyFramebuffer(m_device, m_framebuffers[i], nullptr);
    m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
  }

  //m_vk.vkDestroyDescriptorSetLayout(m_device, descriptorSetLayout, nullptr);

}

//BLEND
void  kgmVulkan::gcBlend(bool, u32, u32, u32) {}

//ALPHA
void  kgmVulkan::gcAlpha(bool, u32, float) {}

//CULL
void  kgmVulkan::gcCull(u32 mode) {}

//DEPTH
void  kgmVulkan::gcDepth(bool en, bool mask, u32 mode) {}

//LIGHT
void kgmVulkan::gcSetLight(int i, float* pos, float forse, float* col, float* dir, float angle) {}

//VERTEX & INDEX BUFFERS
void* kgmVulkan::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize) {}
void  kgmVulkan::gcFreeVertexBuffer(void*) {}
void  kgmVulkan::gcDrawVertexBuffer(void* buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff) {}

// SHADER
void* kgmVulkan::gcGenShader(const char*, const char*) {}
void  kgmVulkan::gcFreeShader(void* s) {}
void  kgmVulkan::gcSetShader(void* s) {}
void  kgmVulkan::gcBindAttribute(void* s, int, const char*) {}
void  kgmVulkan::gcUniform(void* s, u32, u32, const char*, void*) {}
void  kgmVulkan::gcUniformMatrix(void* s, u32, u32, u32, const char*, void*) {}
void  kgmVulkan::gcUniformSampler(void* s, const char*, void*) {}

#ifdef DEBUG
void  kgmVulkan::gcGetUniform(void* s, const char*, void*) {}
#endif

#endif
