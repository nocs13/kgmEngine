#include "kgmVulkan.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmUtils/kgmBase64.h"
#include "kgmSystem.h"
#include "kgmThread.h"

#ifdef VULKAN

#include "inc/vk/vulkan.h"

#define ZeroObject(o) memset(&o, 0, sizeof(typeof o))

//https://gist.github.com/Overv/7ac07356037592a121225172d7d78f2d

u32           kgmVulkan::g_vulkans = 0;
kgmLib        kgmVulkan::vk_lib;
kgmVulkan::vk kgmVulkan::m_vk      = {0};

kgmVulkan::kgmVulkan(kgmWindow* wnd)
{
  m_instance = 0;

  if (wnd == nullptr)
    return;

  m_window = wnd;

  if (g_vulkans < 1)
  {
    if (vkInit() != 1 || m_vk.vkCreateInstance == nullptr)
    {
      m_error = 1;

      return;
    }
  }
  else
  {
    m_error = -1;

    return;
  }

  g_vulkans++;

  wnd->getRect(m_rect[0], m_rect[1], m_rect[2], m_rect[3]);

  if (!initInstance())
  {
    m_error = 1;

    return;
  }

  initDebug();

  if (!listDevices() || !initDevice())
  {
    m_error = 1;

    return;
  }

  if (!initSurface() || !initSwapchain())
  {
    m_error = 1;

    return;
  }

  if (!initRenderPass() || !initImageViews() || !initFrameBuffers())
  {
    m_error = 1;

    return;
  }

  if (!initSemaphores() || !initCommands())
  {
    m_error = 1;

    return;
  }

  /*
  VkResult vk_res;

  VkApplicationInfo appInfo;

  memset(&appInfo, 0, sizeof(VkApplicationInfo));

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "kTest";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  u32 extensionsCount = 0;

  vk_res = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

  if (extensionsCount == 0)
  {
    kgm_log() << "Vulkan error: No extensions supported!\n";

    return;
  }

  VkExtensionProperties extensionProperties[extensionsCount];
  const char* extensionNames[extensionsCount];

  vk_res = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensionProperties);

  u32 iname = 0;

  for (const auto& ep: extensionProperties)
  {
    kgm_log() << "Vulkan info: Extension name " << ep.extensionName << "\n";

    extensionNames[iname++] = ep.extensionName;
  }

  VkInstanceCreateInfo createInfo;

  memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));

  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = extensionsCount;
  createInfo.ppEnabledExtensionNames = extensionNames;

  vk_res = m_vk.vkCreateInstance(&createInfo, null, &m_instance);

  if (vk_res != VK_SUCCESS)
  {
    switch(vk_res)
    {
    case VK_ERROR_LAYER_NOT_PRESENT:
    kgm_log() << "Vulkan error: vkCreateInstance no layers\n";
      break;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
      kgm_log() << "Vulkan error: vkCreateInstance no extentions\n";
      break;
    default:
      kgm_log() << "Vulkan error: vkCreateInstance failed.\n";

      printResult(vk_res);
    }

    return;
  }

  kgm_log() << "Vulkan: Instance created.\n";

#ifdef DEBUG

  m_vk.vkCreateDebugReportCallbackEXT = (typeof m_vk.vkCreateDebugReportCallbackEXT) m_vk.vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");;

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
        kgm_log() << "{" << pLayerPrefix << "} ";
        kgm_log() << pMessage << "\n";
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

  kgm_log() << "Vulkan: Debug callback created.\n";

#endif

  u32 deviceCount = 0;

  vk_res = m_vk.vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: vkEnumeratePhysicalDevices.\n";

    printResult(vk_res);

    return;
  }

  VkPhysicalDevice physicalDevices[deviceCount];

  vk_res = m_vk.vkEnumeratePhysicalDevices(m_instance, &deviceCount, physicalDevices);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: vkEnumeratePhysicalDevices no devs\n";

    return;
  }

  kgm_log() << "Vulkan: [" << deviceCount << "] Physical devices enumerated.\n";

  VkPhysicalDevice physicalDevice = physicalDevices[0];

  m_physicalDevice = physicalDevice;

  extensionsCount = 0;

  vk_res = m_vk.vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties.\n";

    printResult(vk_res);

    return;
  }

  VkExtensionProperties deviceExtensionProperties[extensionsCount];

  vk_res = m_vk.vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, deviceExtensionProperties);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties no extentions\n";

    return;
  }

  kgm_log() << "Vulkan: Device extentions enumerated.\n";

  bool isswapchain = false;

  for (const auto e: deviceExtensionProperties)
  {
    if (strcmp(e.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
    {
      isswapchain = true;

      break;
    }

    kgm_log() << "Vulkan: vkEnumerateDeviceExtensionProperties " << e.extensionName << "\n";
  }

  if (!isswapchain)
  {
    kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties no swap chain!\n";

    return;
  }

  kgm_log() << "Vulkan: Has swap chain.\n";

  VkSurfaceKHR surface;

#ifdef WIN32
  VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
  surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surfaceCreateInfo.hwnd = wnd->m_wnd;
  surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);

  vk_res = m_vk.vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &surface);
#else
  VkXlibSurfaceCreateInfoKHR surfaceCreateInfo;
  surfaceCreateInfo.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
  surfaceCreateInfo.window = wnd->m_wnd;
  surfaceCreateInfo.dpy    = wnd->m_dpy;

  vk_res = m_vk.vkCreateXlibSurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &surface);
#endif

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: vkCreate**SurfaceKHR " << vk_res << "\n";

    return;
  }

  kgm_log() << "Vulkan: Surface created.\n";

  m_surface = surface;

  VkSurfaceCapabilitiesKHR surfaceCapabilities;

  if(m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilities) != VkResult::VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: failed to get surface capabilities.\n";

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
  for (auto &sf: surfaceFormats)
  {
    kgm_log() << "Vulkan: Surface formats > colorSpace " << sf.colorSpace << ", format " << sf.format << ".\n";
  }

  kgm_log() << "Vulkan: Got surface formats.\n";

  u32 queueFamilyCount = 0;

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

  if (queueFamilyCount < 1)
  {
    kgm_log() << "kgmVulkan error: vkGetPhysicalDeviceQueueFamilyProperties no count\n";

    return;
  }

  VkQueueFamilyProperties queueFamilies[queueFamilyCount];

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

  if (queueFamilyCount < 1)
  {
    kgm_log() << "kgmVulkan error: vkGetPhysicalDeviceQueueFamilyProperties no families\n";

    return;
  }

  kgm_log() << "Vulkan: Physical devices queue family properties checked.\n";

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

  kgm_log() << "Vulkan: Found valid queue family graphics support.\n";

  f32 queuePriority = 1.0f;

  VkDeviceQueueCreateInfo queueCreateInfo[2] = {};

  memset(queueCreateInfo, 0, 2 * sizeof(VkDeviceQueueCreateInfo));

  queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo[0].queueFamilyIndex = graphicsQueueFamily;
  queueCreateInfo[0].queueCount = 1;
  queueCreateInfo[0].pQueuePriorities = &queuePriority;

  queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo[0].queueFamilyIndex = presentQueueFamily;
  queueCreateInfo[0].queueCount = 1;
  queueCreateInfo[0].pQueuePriorities = &queuePriority;

  VkDeviceCreateInfo deviceCreateInfo;

  memset(&deviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));

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
  VkPhysicalDeviceFeatures enabledFeatures;

  memset(&enabledFeatures, 0, sizeof(VkPhysicalDeviceFeatures));

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

  wnd->getRect(m_rect[0], m_rect[1], m_rect[2], m_rect[3]);

  // Find supported present modes
  u32 presentModeCount;

  vk_res = m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);

  if (vk_res != VK_SUCCESS || presentModeCount == 0)
  {
    kgm_log() << "Vulkan error: Failed to get number of supported presentation modes.\n";

    return;
  }

  VkPresentModeKHR presentModes[presentModeCount];

  vk_res = m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes);

  if (vk_res != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get supported presentation modes.\n";

    return;
  }

  VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

  for (const auto &i: presentModes)
  {
    if (i == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      presentMode == i;

      break;
    }
  }

  VkSwapchainKHR swapChain = null;

  VkSwapchainCreateInfoKHR swapChainCreateInfo;

  ZeroObject(swapChainCreateInfo);

  swapChainCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapChainCreateInfo.pNext = nullptr;
  swapChainCreateInfo.flags = 0;
  swapChainCreateInfo.surface = surface;
  swapChainCreateInfo.minImageCount = 1;
  swapChainCreateInfo.imageFormat = surfaceFormats[0].format;
  swapChainCreateInfo.imageColorSpace = surfaceFormats[0].colorSpace;
  swapChainCreateInfo.imageExtent = VkExtent2D{(u32) m_rect[2], (u32) m_rect[3]};
  swapChainCreateInfo.imageArrayLayers = 1;
  swapChainCreateInfo.imageUsage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapChainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
  swapChainCreateInfo.queueFamilyIndexCount = 0;
  swapChainCreateInfo.pQueueFamilyIndices = nullptr;
  swapChainCreateInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  swapChainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapChainCreateInfo.presentMode = presentMode;
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

  memset(&attachmentDescription, 0, sizeof(VkAttachmentDescription));

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

  memset(&attachmentReference, 0, sizeof(VkAttachmentReference));

  attachmentReference.attachment = 0;
  attachmentReference.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpassDescription;

  memset(&subpassDescription, 0, sizeof(VkSubpassDescription));

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

  memset(&renderPassCreateInfo, 0, sizeof(VkRenderPassCreateInfo));

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

    memset(&imageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

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

    memset(&framebufferCreateInfo, 0, sizeof(VkFramebufferCreateInfo));

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
  */
  /*VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

  m_vk.vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

  VkSemaphore imageAvailableSemaphore, renderingFinishedSemaphore;
  VkSemaphoreCreateInfo semaphoreCreateInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if (m_vk.vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
      m_vk.vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderingFinishedSemaphore) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan: failed to create semaphores.\n";

    return;
  }

  kgm_log() << "Vulkan: created semaphores.\n";*/
  /*
  VkCommandPool commandPool;

  VkCommandPoolCreateInfo poolCreateInfo;

  memset(&poolCreateInfo, 0, sizeof(VkCommandPoolCreateInfo));

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

  ZeroObject(commandBufferAllocateInfo);

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

    ZeroObject(commandBufferInheritanceInfo);

    commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    commandBufferInheritanceInfo.pNext = nullptr;
    commandBufferInheritanceInfo.renderPass = renderPass;
    commandBufferInheritanceInfo.subpass = 0;
    commandBufferInheritanceInfo.framebuffer = framebuffer;
    commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
    commandBufferInheritanceInfo.queryFlags = 0;
    commandBufferInheritanceInfo.pipelineStatistics = 0;

    VkCommandBufferBeginInfo commandBufferBeginInfo;

    ZeroObject(commandBufferBeginInfo);

    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = nullptr;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = &commandBufferInheritanceInfo;

    m_vk.vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

    VkClearValue clearValue;

    ZeroObject(clearValue);

    clearValue.color.float32[0] = 1.0f;
    clearValue.color.float32[1] = 0.0f;
    clearValue.color.float32[2] = 0.0f;
    clearValue.color.float32[3] = 1.0f;

    VkRenderPassBeginInfo renderPassBeginInfo;

    ZeroObject(renderPassBeginInfo);

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

    ZeroObject(imageMemoryBarrier);

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

  ZeroObject(fenceCreateInfo);

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
  }
  */

  kgm_log() << "Vulkan: Successfully prepared.\n";
}

kgmVulkan::~kgmVulkan()
{
  if (m_device)
  {
    m_vk.vkDeviceWaitIdle(m_device);

    if (m_commandPool)
    {
      m_vk.vkFreeCommandBuffers(m_device, m_commandPool, (u32) m_commandBuffers.length(), m_commandBuffers.data());

      m_commandBuffers.clear();

      kgm_log() << "Vulkan: Command buffer destroyed.\n";

      m_vk.vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    }


    //m_vk.vkDestroyPipeline(m_device, graphicsPipeline, nullptr);
    if (m_renderPass)
    {
      m_vk.vkDestroyRenderPass(m_device, m_renderPass, nullptr);

      kgm_log() << "Vulkan: Render pass destroyed.\n";
    }

    for (size_t i = 0; i < m_swapChainImages.length(); i++)
    {
      m_vk.vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr);
      m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
    }

    m_swapChainImages.clear();
    m_imageViews.clear();
    m_frameBuffers.clear();


    if (m_swapChain != VK_NULL_HANDLE)
    {
      m_vk.vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);

      kgm_log() << "Vulkan: SwapChain destroyed.\n";
    }

    m_vk.vkDestroyDevice(m_device, nullptr);

    kgm_log() << "Vulkan: Device destroyed.\n";
  }

  if (m_instance)
  {
    if (m_surface)
      m_vk.vkDestroySurfaceKHR(m_instance, m_surface, null);

#ifdef DEBUG
    m_vk.vkDestroyDebugReportCallbackEXT = (typeof m_vk.vkDestroyDebugReportCallbackEXT) m_vk.vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");;

    if (m_vk.vkDestroyDebugReportCallbackEXT)
      m_vk.vkDestroyDebugReportCallbackEXT(m_instance, m_debugReportCallback, nullptr);
#endif

    m_vk.vkDestroyInstance(m_instance, null);
  }

  g_vulkans--;

  if (g_vulkans < 1)
    vkFree();
}

#if !defined(VK_IMPORT_FUNCTION)
#define VK_IMPORT_FUNCTION( fn ) m_vk.fn = (typeof m_vk.fn) vk_lib.get((s8*) #fn)
#endif

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

  m_vk.vkGetInstanceProcAddr = (typeof m_vk.vkGetInstanceProcAddr) vk_lib.get((char*) "vkGetInstanceProcAddr");
  m_vk.vkCreateInstance = (typeof m_vk.vkCreateInstance) vk_lib.get((char*) "vkCreateInstance");
  m_vk.vkDestroyInstance = (typeof m_vk.vkDestroyInstance) vk_lib.get((char*) "vkDestroyInstance");
  m_vk.vkEnumerateInstanceExtensionProperties = (typeof m_vk.vkEnumerateInstanceExtensionProperties) vk_lib.get((char*) "vkEnumerateInstanceExtensionProperties");
  m_vk.vkEnumeratePhysicalDevices = (typeof m_vk.vkEnumeratePhysicalDevices) vk_lib.get((char*) "vkEnumeratePhysicalDevices");
  m_vk.vkEnumerateDeviceExtensionProperties = (typeof m_vk.vkEnumerateDeviceExtensionProperties) vk_lib.get((char*) "vkEnumerateDeviceExtensionProperties");
  m_vk.vkGetPhysicalDeviceFormatProperties = (typeof m_vk.vkGetPhysicalDeviceFormatProperties) vk_lib.get((char*) "vkGetPhysicalDeviceFormatProperties");
  m_vk.vkGetPhysicalDeviceQueueFamilyProperties = (typeof m_vk.vkGetPhysicalDeviceQueueFamilyProperties) vk_lib.get((char*) "vkGetPhysicalDeviceQueueFamilyProperties");
  m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
  m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceFormatsKHR");
  m_vk.vkGetPhysicalDeviceSurfaceSupportKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceSupportKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceSupportKHR");
  m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR = (typeof m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfacePresentModesKHR");
  m_vk.vkGetImageMemoryRequirements = (typeof m_vk.vkGetImageMemoryRequirements) vk_lib.get((char*) "vkGetImageMemoryRequirements");
  m_vk.vkCreateDevice = (typeof m_vk.vkCreateDevice) vk_lib.get((char*) "vkCreateDevice");
  m_vk.vkDestroyDevice = (typeof m_vk.vkDestroyDevice) vk_lib.get((char*) "vkDestroyDevice");
  m_vk.vkGetDeviceQueue = (typeof m_vk.vkGetDeviceQueue) vk_lib.get((char*) "vkGetDeviceQueue");
  m_vk.vkQueueWaitIdle = (typeof m_vk.vkQueueWaitIdle) vk_lib.get((char*) "vkQueueWaitIdle");
  m_vk.vkGetPhysicalDeviceMemoryProperties = (typeof m_vk.vkGetPhysicalDeviceMemoryProperties) vk_lib.get((char*) "vkGetPhysicalDeviceMemoryProperties");
  m_vk.vkCreateSwapchainKHR = (typeof m_vk.vkCreateSwapchainKHR) vk_lib.get((char*) "vkCreateSwapchainKHR");
  m_vk.vkDestroySwapchainKHR = (typeof m_vk.vkDestroySwapchainKHR) vk_lib.get((char*) "vkDestroySwapchainKHR");
  m_vk.vkGetSwapchainImagesKHR = (typeof m_vk.vkGetSwapchainImagesKHR) vk_lib.get((char*) "vkGetSwapchainImagesKHR");
  m_vk.vkCreateSemaphore = (typeof m_vk.vkCreateSemaphore) vk_lib.get((char*) "vkCreateSemaphore");
  m_vk.vkCreateCommandPool = (typeof m_vk.vkCreateCommandPool) vk_lib.get((char*) "vkCreateCommandPool");
  m_vk.vkResetCommandPool = (typeof m_vk.vkResetCommandPool) vk_lib.get((char*) "vkResetCommandPool");
  m_vk.vkDestroyCommandPool = (typeof m_vk.vkDestroyCommandPool) vk_lib.get((char*) "vkDestroyCommandPool");
  m_vk.vkCreateRenderPass = (typeof m_vk.vkCreateRenderPass) vk_lib.get((char*) "vkCreateRenderPass");
  m_vk.vkCmdBeginRenderPass = (typeof m_vk.vkCmdBeginRenderPass) vk_lib.get((char*) "vkCmdBeginRenderPass");
  m_vk.vkCmdEndRenderPass = (typeof m_vk.vkCmdEndRenderPass) vk_lib.get((char*) "vkCmdEndRenderPass");
  m_vk.vkCreateShaderModule = (typeof m_vk.vkCreateShaderModule) vk_lib.get((char*) "vkCreateShaderModule");
  m_vk.vkDestroyShaderModule = (typeof m_vk.vkDestroyShaderModule) vk_lib.get((char*) "vkDestroyShaderModule");
  m_vk.vkCreateImage = (typeof m_vk.vkCreateImage) vk_lib.get((char*) "vkCreateImage");
  m_vk.vkCreateImageView = (typeof m_vk.vkCreateImageView) vk_lib.get((char*) "vkCreateImageView");
  m_vk.vkCreateFramebuffer = (typeof m_vk.vkCreateFramebuffer) vk_lib.get((char*) "vkCreateFramebuffer");
  m_vk.vkAllocateMemory = (typeof m_vk.vkAllocateMemory) vk_lib.get((char*) "vkAllocateMemory");
  m_vk.vkFreeMemory = (typeof m_vk.vkFreeMemory) vk_lib.get((char*) "vkFreeMemory");
  m_vk.vkBindImageMemory = (typeof m_vk.vkBindImageMemory) vk_lib.get((char*) "vkBindImageMemory");
  m_vk.vkAllocateCommandBuffers = (typeof m_vk.vkAllocateCommandBuffers) vk_lib.get((char*) "vkAllocateCommandBuffers");
  m_vk.vkBeginCommandBuffer = (typeof m_vk.vkBeginCommandBuffer) vk_lib.get((char*) "vkBeginCommandBuffer");
  m_vk.vkResetCommandBuffer = (typeof m_vk.vkResetCommandBuffer) vk_lib.get((char*) "vkResetCommandBuffer");
  m_vk.vkEndCommandBuffer = (typeof m_vk.vkEndCommandBuffer) vk_lib.get((char*) "vkEndCommandBuffer");
  m_vk.vkCmdPipelineBarrier = (typeof m_vk.vkCmdPipelineBarrier) vk_lib.get((char*) "vkCmdPipelineBarrier");
  m_vk.vkCreateFence = (typeof m_vk.vkCreateFence) vk_lib.get((char*) "vkCreateFence");
  m_vk.vkResetFences = (typeof m_vk.vkResetFences) vk_lib.get((char*) "vkResetFences");
  m_vk.vkWaitForFences = (typeof m_vk.vkWaitForFences) vk_lib.get((char*) "vkWaitForFences");
  m_vk.vkDeviceWaitIdle = (typeof m_vk.vkDeviceWaitIdle) vk_lib.get((char*) "vkDeviceWaitIdle");
  m_vk.vkAcquireNextImageKHR = (typeof m_vk.vkAcquireNextImageKHR) vk_lib.get((char*) "vkAcquireNextImageKHR");
  m_vk.vkQueueSubmit = (typeof m_vk.vkQueueSubmit) vk_lib.get((char*) "vkQueueSubmit");
  m_vk.vkQueuePresentKHR = (typeof m_vk.vkQueuePresentKHR) vk_lib.get((char*) "vkQueuePresentKHR");
  m_vk.vkDestroyPipeline = (typeof m_vk.vkDestroyPipeline) vk_lib.get((char*) "vkDestroyPipeline");
  m_vk.vkDestroyRenderPass = (typeof m_vk.vkDestroyRenderPass) vk_lib.get((char*) "vkDestroyRenderPass");
  m_vk.vkDestroyFramebuffer = (typeof m_vk.vkDestroyFramebuffer) vk_lib.get((char*) "vkDestroyFramebuffer");
  m_vk.vkDestroyImageView = (typeof m_vk.vkDestroyImageView) vk_lib.get((char*) "vkDestroyImageView");
  m_vk.vkDestroyDescriptorSetLayout = (typeof m_vk.vkDestroyDescriptorSetLayout) vk_lib.get((char*) "vkDestroyDescriptorSetLayout");
  m_vk.vkFreeCommandBuffers = (typeof m_vk.vkFreeCommandBuffers) vk_lib.get((char*) "vkFreeCommandBuffers");

  VK_IMPORT_FUNCTION(vkDestroySurfaceKHR);
  VK_IMPORT_FUNCTION(vkDestroyImage);
  VK_IMPORT_FUNCTION(vkMapMemory);
  VK_IMPORT_FUNCTION(vkUnmapMemory);


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
  return m_error;
}

void  kgmVulkan::gcSet(u32 param, void* value) {}
void  kgmVulkan::gcGet(u32 param, void* value) {}

void  kgmVulkan::gcClear(u32 flag, u32 col, float depth, u32 sten)
{
  for (u32 i = 0; i < m_swapChainImages.length(); i++)
  {
    auto &commandBuffer = m_commandBuffers[i];
    auto &image         = m_swapChainImages[i];
    auto &framebuffer   = m_frameBuffers[i];

    VkResult result = m_vk.vkResetCommandBuffer(commandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

    VkCommandBufferInheritanceInfo commandBufferInheritanceInfo;
    commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    commandBufferInheritanceInfo.pNext = nullptr;
    commandBufferInheritanceInfo.renderPass = m_renderPass;
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

    result = m_vk.vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

    VkClearValue clearValue;

    ZeroObject(clearValue);

    float ggg = (float) ((float) rand() / (float) RAND_MAX);

    clearValue.color.float32[0] = 1.0f;
    clearValue.color.float32[1] = ggg; //0.0f;
    clearValue.color.float32[2] = 0.0f;
    clearValue.color.float32[3] = 1.0f;

    VkRenderPassBeginInfo renderPassBeginInfo;

    ZeroObject(renderPassBeginInfo);

    renderPassBeginInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.framebuffer = framebuffer;
    renderPassBeginInfo.renderArea = VkRect2D {
    VkOffset2D  {0, 0},
    VkExtent2D  {
    (u32) m_rect[2], (u32) m_rect[3]
    //surfaceCapabilities.currentExtent.width,
    //surfaceCapabilities.currentExtent.height
  }
};
renderPassBeginInfo.clearValueCount = 1;
renderPassBeginInfo.pClearValues = &clearValue;

m_vk.vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
m_vk.vkCmdEndRenderPass(commandBuffer);

VkImageMemoryBarrier imageMemoryBarrier;

ZeroObject(imageMemoryBarrier);

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
}

void kgmVulkan::gcResize(u32 width, u32 height)
{
  //return;
  m_rect[2] = width;
  m_rect[3] = height;

  kgm_log() << "Vulkan: Window resized, viewport need update.\n";

  refreshSwapchain();
}

void  kgmVulkan::gcBegin() {}
void  kgmVulkan::gcEnd() {}

void  kgmVulkan::gcRender()
{
  VkResult result;

  //kgm_log() << "Vulkan info: start render.\n";

  u32 swapChainImage = 0;

  if (!m_device || !m_swapChain)
    return;

  result = m_vk.vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, VK_NULL_HANDLE, m_fence, &swapChainImage);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
  {
    kgm_log() << "Vulkan render info: Surface incompatible, updating swapchans.\n";

    refreshSwapchain();

    return;
  }

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to get next swapchain image.\n";

    printResult(result);

    exit(0);

    return;
  }

  result = m_vk.vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, UINT64_MAX);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to wait for fence.\n";

    printResult(result);

    return;
  }

  result = m_vk.vkResetFences(m_device, 1, &m_fence);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to reset fence.\n";

    printResult(result);

    return;
  }

  VkQueue queue;

  m_vk.vkGetDeviceQueue(m_device, 0, 0, &queue);

  auto &commandBuffer = m_commandBuffers[swapChainImage];
  //auto &commandBuffer = m_commandBuffers[0];

  VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

  VkSubmitInfo submitInfo;

  ZeroObject(submitInfo);

  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = nullptr;
  submitInfo.waitSemaphoreCount = 0;
  submitInfo.pWaitSemaphores = nullptr;
  submitInfo.pWaitDstStageMask = &waitMask;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.pSignalSemaphores = nullptr;

  result = m_vk.vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to submit command buffer.\n";

    printResult(result);

    return;
  }

  result = m_vk.vkQueueWaitIdle(queue);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to wait for queue.\n";

    printResult(result);

    //return;
  }

  VkPresentInfoKHR presentInfo;

  ZeroObject(presentInfo);

  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;
  presentInfo.waitSemaphoreCount = 0;
  presentInfo.pWaitSemaphores = nullptr;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &m_swapChain;
  presentInfo.pImageIndices = &swapChainImage;
  presentInfo.pResults = &result;

  VkResult result2 = m_vk.vkQueuePresentKHR(queue, &presentInfo);

  if (result2 == VK_ERROR_OUT_OF_DATE_KHR || result2 == VK_SUBOPTIMAL_KHR)
  {
    kgm_log() << "Vulkan render info: Surface incompatible, updating swapchans.\n";

    refreshSwapchain();

    return;
  }

  if(result != VkResult::VK_SUCCESS || result2 != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to present swapchain.\n";

    printResult(result);
    printResult(result2);

    exit(0);

    return;
  }

  //kgm_log() << "Vulkan: Queue present passed.\n";

  m_swapChainImage = swapChainImage;
}

void  kgmVulkan::gcSetTarget(void*  rt) {}

// DRAWING
void  kgmVulkan::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt) {}

// TEXTURE
void* kgmVulkan::gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type)
{
  if (!m_device || !w || !h)
    return null;
  return null;

  Texture* t = new Texture;

  t->image = VK_NULL_HANDLE;
  t->iview = VK_NULL_HANDLE;
  t->memory = VK_NULL_HANDLE;

  u32 bypp = 1;

  VkFormat format;

  VkImageType itype;

  switch(bpp)
  {
  case gctex_fmt8:
    format = VK_FORMAT_R8_UNORM;
    bypp = 1;
    break;
  case gctex_fmt16:
    format = VK_FORMAT_R5G6B5_UNORM_PACK16;
    bypp = 2;
    break;
  case gctex_fmt24:
    format = VK_FORMAT_R8G8B8_UNORM;
    bypp = 3;
    break;
  case gctex_fmt32:
    format = VK_FORMAT_R8G8B8A8_UNORM;
    bypp = 4;
    break;
  case gctex_fmtdepth:
    format = VK_FORMAT_D16_UNORM;
    bypp = 2;
    break;
  case gctex_fmtdepten:
    format = VK_FORMAT_D16_UNORM_S8_UINT;
    bypp = 3;
    break;
  default:
    format = VK_FORMAT_R8G8B8A8_UNORM;
    bypp = 4;
  };

  format = VK_FORMAT_R8G8B8A8_UNORM;

  switch (type)
  {
  case gctype_texcube:
    itype = VK_IMAGE_TYPE_3D;
    break;
  case gctype_texdepth:
    itype = VK_IMAGE_TYPE_2D;
    break;
  case gctype_tex2d:
  default:
    itype = VK_IMAGE_TYPE_2D;
    break;
  }

  VkImageCreateInfo imageInfo = {};

  ZeroObject(imageInfo);

  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = itype;
  imageInfo.extent.width = w;
  imageInfo.extent.height = h;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkResult result = m_vk.vkCreateImage(m_device, &imageInfo, nullptr, &t->image);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create texture image.\n";

    printResult(result);

    delete t;

    return null;
  }

  VkMemoryRequirements requirements;

  m_vk.vkGetImageMemoryRequirements(m_device, t->image, &requirements);

  VkMemoryAllocateInfo allocInfo = {};

  ZeroObject(allocInfo);

  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = requirements.size;

  result = m_vk.vkAllocateMemory(m_device, &allocInfo, null, &t->memory);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to allocate texture image memory.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, t->image, null);

    delete t;

    return null;
  }

  result = m_vk.vkBindImageMemory(m_device, t->image, t->memory, null);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to bind texture image memory.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, t->image, null);
    m_vk.vkFreeMemory(m_device, t->memory, null);

    delete t;

    return null;
  }

  if (m)
  {
    void* data = null;

    u32 size = w * h * bypp;

    result = m_vk.vkMapMemory(m_device, t->memory, 0, size, 0, &data);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Failed to fill texture image memory.\n";

      printResult(result);

      m_vk.vkDestroyImage(m_device, t->image, null);

      m_vk.vkFreeMemory(m_device, t->memory, null);

      delete t;

      return null;
    }

    if (bypp == 4)
    {
      memcpy(data, m, size);
    }
    else if (bypp == 3)
    {

    }
    else if (bypp == 2)
    {

    }
    else
    {

    }

    m_vk.vkUnmapMemory(m_device, t->memory);
  }

  VkImageViewCreateInfo viewInfo;

  ZeroObject(viewInfo);

  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.image = t->image;

  result = m_vk.vkCreateImageView(m_device, &viewInfo, null, &t->iview);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create texture image view.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, t->image, null);

    m_vk.vkFreeMemory(m_device, t->memory, null);

    delete t;

    return null;
  }

  return t;
}

void  kgmVulkan::gcFreeTexture(void *t)
{
  if (!m_device || !t)
    return;

  if (((Texture*)t)->iview)
    m_vk.vkDestroyImageView(m_device, ((Texture*)t)->iview, null);

  if (((Texture*)t)->image)
    m_vk.vkDestroyImage(m_device, ((Texture*)t)->image, null);

  if (((Texture*)t)->memory)
    m_vk.vkFreeMemory(m_device, ((Texture*)t)->memory, null);

  delete (Texture*)t;
}

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
void* kgmVulkan::gcGenShader(const char* v, const char* f)
{
  if (!m_device)
  {
    kgm_log() << "Vulkan error: Device not initialized.\n";

    return null;
  }

  if (!v)
  {
    kgm_log() << "Vulkan error: Fragment shader absent.\n";

    return null;
  }

  Shader* shader = new Shader{null};

  kgmArray<u8> binary;

  kgmString base64;

  base64.init(v);

  kgmBase64::decode(base64, binary);

  VkShaderModuleCreateInfo createInfo;

  createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.codeSize = binary.length();
  createInfo.pCode = (typeof createInfo.pCode) binary.data();

  VkResult result = m_vk.vkCreateShaderModule(m_device, &createInfo, nullptr, &shader->vertex);

  if(result  != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create vertex shader.\n";

    printResult(result);

    delete shader;

    return null;
  }

  base64.init(v);

  binary.clear();

  kgmBase64::decode(base64, binary);

  createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.codeSize = binary.length();
  createInfo.pCode = (typeof createInfo.pCode) binary.data();

  result = m_vk.vkCreateShaderModule(m_device, &createInfo, nullptr, &shader->fragment);

  if(result  != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create fragment shader.\n";

    printResult(result);
  }

  return shader;
}

void  kgmVulkan::gcFreeShader(void* s)
{
  if (!s)
  {
    kgm_log() << "Vulkan error: Shader is invalid for free.\n";

    return;
  }

  if (!m_device)
  {
    kgm_log() << "Vulkan error: Device is invalid for free shader.\n";

    return;
  }

  if (((Shader*)s)->vertex)
  {
    m_vk.vkDestroyShaderModule(m_device, ((Shader*)s)->vertex, null);
  }

  if (((Shader*)s)->fragment)
  {
    m_vk.vkDestroyShaderModule(m_device, ((Shader*)s)->fragment, null);
  }

  delete (Shader *) s;
}

void  kgmVulkan::gcSetShader(void* s) {}
void  kgmVulkan::gcBindAttribute(void* s, int, const char*) {}
void  kgmVulkan::gcUniform(void* s, u32, u32, const char*, void*) {}
void  kgmVulkan::gcUniformMatrix(void* s, u32, u32, u32, const char*, void*) {}
void  kgmVulkan::gcUniformSampler(void* s, const char*, void*) {}

#ifdef DEBUG
void  kgmVulkan::gcGetUniform(void* s, const char*, void*) {}
#endif

void kgmVulkan::clean(u32 options)
{

}

void kgmVulkan::freeSurface()
{
  if (!m_surface)
  {
    return;
  }

  m_vk.vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

  m_surface = 0;
}

bool kgmVulkan::initInstance()
{
  if(m_instance != nullptr)
  {
    kgm_log() << "Vulkan error: Instance already initialized.\n";

    return false;
  }

  const s8* layerNames[] = {
    "VK_LAYER_LUNARG_api_dump",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_image",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_screenshot",
    "VK_LAYER_LUNARG_swapchain",
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_GOOGLE_unique_objects",
    "VK_LAYER_LUNARG_vktrace",
    "VK_LAYER_RENDERDOC_Capture",
    "VK_LAYER_NV_optimus",
    "VK_LAYER_VALVE_steam_overlay",
    "VK_LAYER_LUNARG_standard_validation",
  };

  u32 extensionsCount = 0;

  VkResult result = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

  if (result != VK_SUCCESS || extensionsCount == 0)
  {
    kgm_log() << "Vulkan error: No extensions supported!\n";

    printResult(result);

    return false;
  }

  m_extensionProperties.alloc(extensionsCount);
  m_extensionNames.alloc(extensionsCount);

  result = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, m_extensionProperties.data());

  if (result != VK_SUCCESS || extensionsCount == 0)
  {
    kgm_log() << "Vulkan error: No extensions supported!\n";

    printResult(result);

    return false;
  }

  u32 iname = 0;

  for (s32 i = 0; i < extensionsCount; i++)
  {
    const auto& ep = m_extensionProperties[i];

    kgm_log() << "Vulkan info: Extension name " << ep.extensionName << "\n";

    m_extensionNames[iname++] = ep.extensionName;
  }

  VkApplicationInfo appInfo = {};

  ZeroObject(appInfo);

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = "kgmEngine";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);;
  appInfo.pEngineName = "kgmEngine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);;
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo instanceInfo = {};

  ZeroObject(instanceInfo);

  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext = NULL;
  instanceInfo.flags = 0;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledExtensionCount = extensionsCount;
  instanceInfo.ppEnabledExtensionNames = m_extensionNames.data();
  //instanceInfo.enabledLayerCount = 14;
  //instanceInfo.ppEnabledLayerNames = layerNames;

  result = m_vk.vkCreateInstance(&instanceInfo, NULL, &m_instance);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Unable to create instance.\n";

    printResult(result);

    return false;
  }

  return true;
}

bool kgmVulkan::initDebug()
{
#ifdef DEBUG

  m_vk.vkCreateDebugReportCallbackEXT = (typeof m_vk.vkCreateDebugReportCallbackEXT) m_vk.vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");;

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
        kgm_log() << "{" << pLayerPrefix << "} ";
        kgm_log() << pMessage << "\n";

        return VK_FALSE;
      };
  debugReportCallbackCreateInfo.pUserData = nullptr;

  if (m_vk.vkCreateDebugReportCallbackEXT && m_vk.vkCreateDebugReportCallbackEXT(m_instance,
      &debugReportCallbackCreateInfo, nullptr, &m_debugReportCallback)
      != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to create debug collback.\n";

    return false;
  }

  kgm_log() << "Vulkan: Debug callback created.\n";

#endif

  return true;
}

bool kgmVulkan::listDevices()
{
  if (!m_instance)
  {
    kgm_log() << "Vulkan error: Not valid instance.\n";

    return false;
  }

  u32 count = 0;

  VkResult result = m_vk.vkEnumeratePhysicalDevices(m_instance, &count, nullptr);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot get devices count.\n";

    printResult(result);

    return false;
  }

  if (count < 1)
  {
    kgm_log() << "Vulkan error: No valid devices.\n";

    return false;
  }

  m_physicalDevices.alloc(count);

  result = m_vk.vkEnumeratePhysicalDevices(m_instance, &count, m_physicalDevices.data());

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot list devices.\n";

    printResult(result);

    return false;
  }

  if (count < 1)
  {
    m_physicalDevices.clear();

    kgm_log() << "Vulkan error: No valid device.\n";

    return false;
  }

  m_physicalDevice = m_physicalDevices[0];

  return true;
}

bool kgmVulkan::initDevice()
{
  VkResult result = VK_SUCCESS;

  if (!m_physicalDevice)
  {
    kgm_log() << "Vulkan error: Not choosed physical device.\n";

    return false;
  }

  VkPhysicalDevice physicalDevice = m_physicalDevice;

  kgmArray<VkQueueFamilyProperties> familyProperties;

  u32 count = 0;

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, null);

  if(count < 0)
  {
    kgm_log() << "kgmVulkan error: Unable get family properties count.\n";

    return false;
  }

  familyProperties.alloc(count);

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, familyProperties.data());

  if(count < 0)
  {
    kgm_log() << "kgmVulkan error: Unable get family properties.\n";

    return false;
  }

  bool found = false;

  for (u32 i = 0; i < count; i++)
  {
    if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      found = true;

      break;
    }
  }

  if (found)
  {
    u32 extensionsCount = 0;

    result = m_vk.vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties.\n";

      printResult(result);

      return false;
    }

    VkExtensionProperties deviceExtensionProperties[extensionsCount];

    result = m_vk.vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, deviceExtensionProperties);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties no extentions\n";

      return false;
    }

    bool isswapchain = false;

    for (const auto e: deviceExtensionProperties)
    {
      if (strcmp(e.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
        isswapchain = true;

      kgm_log() << "Vulkan: vkEnumerateDeviceExtensionProperties " << e.extensionName << "\n";
    }

    if (!isswapchain)
    {
      kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties no swap chain!\n";

      return false;
    }
  }

  VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

  m_vk.vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

  float priorities = 1.0f;

  VkDeviceQueueCreateInfo infoQueue = {};

  ZeroObject(infoQueue);

  infoQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  infoQueue.pNext = NULL;
  infoQueue.queueCount = 1;
  infoQueue.pQueuePriorities = &priorities;

  VkPhysicalDeviceFeatures enabledFeatures;

  ZeroObject(enabledFeatures);

  enabledFeatures.shaderClipDistance = VK_TRUE;
  enabledFeatures.shaderCullDistance = VK_TRUE;

  const s8* deviceExtensions = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

  VkDeviceCreateInfo infoDevice = {};

  ZeroObject(infoDevice);

  infoDevice.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  infoDevice.pNext = NULL;
  infoDevice.queueCreateInfoCount = 1;
  infoDevice.pQueueCreateInfos = &infoQueue;
  infoDevice.enabledExtensionCount = 1;
  infoDevice.ppEnabledExtensionNames = &deviceExtensions;
  infoDevice.enabledLayerCount = 0;
  infoDevice.ppEnabledLayerNames = NULL;
  infoDevice.pEnabledFeatures = &enabledFeatures;

#ifdef DEBUG
  m_debugLayer = "VK_LAYER_LUNARG_standard_validation";

  infoDevice.enabledLayerCount = 1;
  infoDevice.ppEnabledLayerNames = &m_debugLayer;
#endif

  VkDevice device = null;

  result = m_vk.vkCreateDevice(physicalDevice, &infoDevice, nullptr, &device);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: Cannot create device.\n";

    printResult(result);

    return false;
  }

  m_device = device;

  VkFence fence;

  VkFenceCreateInfo fenceCreateInfo;

  ZeroObject(fenceCreateInfo);

  fenceCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.pNext = nullptr;
  fenceCreateInfo.flags = 0;

  if (m_vk.vkCreateFence(m_device, &fenceCreateInfo, nullptr, &fence)
      != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to create fence.\n";

    return false;
  }

  m_fence = fence;

  return true;
}

bool kgmVulkan::initSurface()
{
  VkResult result = VK_SUCCESS;

#ifdef WIN32
    VkWin32SurfaceCreateInfoKHR createInfo;

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hinstance = GetModuleHandle(nullptr);
    createInfo.hwnd = m_window->m_wnd;

    result = m_vk.vkCreateWin32SurfaceKHR(m_instance, &createInfo, NULL, &m_surface);
#elif defined(ANDROID)
    GET_INSTANCE_PROC_ADDR(info.inst, CreateAndroidSurfaceKHR);

    VkAndroidSurfaceCreateInfoKHR createInfo;

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.window = AndroidGetApplicationWindow();
    res = info.fpCreateAndroidSurfaceKHR(info.inst, &createInfo, nullptr, &info.surface);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    VkWaylandSurfaceCreateInfoKHR createInfo = {};

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.display = m_window->m_display;
    createInfo.surface = m_window->m_window;
    result = m_vk.vkCreateWaylandSurfaceKHR(m_instance, &createInfo, NULL, &m_surface);
#elif defined(XCB)
    VkXcbSurfaceCreateInfoKHR createInfo = {};

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.connection = m_instance;
    createInfo.window = m_window->m_window;
    result = vkCreateXcbSurfaceKHR(m_instance, &createInfo, NULL, &info.surface);
#else
    VkXlibSurfaceCreateInfoKHR createInfo;

    ZeroObject(createInfo);

    createInfo.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.window = m_window->m_wnd;
    createInfo.dpy    = m_window->m_dpy;

    result = m_vk.vkCreateXlibSurfaceKHR(m_instance, &createInfo, nullptr, &m_surface);
#endif

  if(result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create surtface.\n";

    printResult(result);

    return false;
  }

  u32 queueFamilyCount = 0;

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

  if (queueFamilyCount < 1)
  {
    kgm_log()
        << "kgmVulkan error: vkGetPhysicalDeviceQueueFamilyProperties no count\n";

    return false;
  }

  VkQueueFamilyProperties queueFamilies[queueFamilyCount];

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies);

  if (queueFamilyCount < 1)
  {
    kgm_log() << "kgmVulkan error: vkGetPhysicalDeviceQueueFamilyProperties no families\n";

    return false;
  }

  bool hasGraphicsQueueFamily = false;
  bool hasPresentQueueFamily = false;

  u32 graphicsQueueFamily = -1, presentQueueFamily = -1;

  for (u32 i = 0; i < queueFamilyCount; i++)
  {
    VkBool32 presentSupport = false;

    m_vk.vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);

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
    kgm_log() << "Vulkan: queue family #" << graphicsQueueFamily
        << " supports graphics.\n";

    if (hasPresentQueueFamily)
    {
      kgm_log() << "Vulkan queue family #" << presentQueueFamily
          << " supports presentation.\n";
    } else
    {
      kgm_log()
          << "Vulkan error: could not find a valid queue family with present support.\n";

      return false;
    }
  } else
  {
    kgm_log()
        << "Vulkan error: could not find a valid queue family with graphics support.\n";

    return false;
  }

  m_graphicsQueueFamilyIndex = graphicsQueueFamily;
  m_presentQueueFamilyIndex  = presentQueueFamily;

  return true;
}

bool kgmVulkan::initCommands()
{
  VkCommandPoolCreateInfo infoPool = { };

  if (!m_instance || !m_device)
  {
    kgm_log() << "Vulkan error: Not prepared to init commands.\n";

    return false;
  }

  infoPool.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  infoPool.pNext = null;
  infoPool.queueFamilyIndex = m_graphicsQueueFamilyIndex;
  infoPool.flags = 0;

  VkResult result = m_vk.vkCreateCommandPool(m_device, &infoPool, nullptr, &m_commandPool);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create command pool.\n";

    printResult(result);

    return false;
  }

  result = m_vk.vkResetCommandPool(m_device, m_commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot reset command pool.\n";

    printResult(result);

    return false;
  }

  VkCommandBufferAllocateInfo infoCommand = { };

  infoCommand.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  infoCommand.pNext = null;
  infoCommand.commandPool = m_commandPool;
  infoCommand.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  infoCommand.commandBufferCount = 2;

  m_commandBuffers.alloc(2);

  result = m_vk.vkAllocateCommandBuffers(m_device, &infoCommand, m_commandBuffers.data());

  if(result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot allocate command buffer.\n";

    printResult(result);

    return false;
  }

  return true;
}

bool kgmVulkan::initSwapchain()
{
  VkResult result;

  VkSurfaceCapabilitiesKHR surfaceCapabilities;

  result = m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilities);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: failed to get surface capabilities.\n";

    printResult(result);

    return false;
  }

  u32 surfaceFormatsCount = 0;

  result = m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatsCount, nullptr);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get surface formats count.\n";

    printResult(result);

    return false;
  }

  VkSurfaceFormatKHR surfaceFormats[surfaceFormatsCount];

  result = m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatsCount, surfaceFormats);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get surface formats.\n";

    printResult(result);

    return false;
  }

  if(surfaceFormatsCount < 1)
  {
    kgm_log() << "Vulkan error: Failed to get valid surface formats.\n";

    return false;
  }

  if (surfaceFormatsCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
  {
    m_swapChainFormat = VK_FORMAT_B8G8R8A8_UNORM;
  }
  else
  {
    m_swapChainFormat = surfaceFormats[0].format;
  }

  u32 presentModeCount;

  result = m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);

  if (result != VK_SUCCESS || presentModeCount == 0)
  {
    kgm_log() << "Vulkan error: Failed to get number of supported presentation modes.\n";

    return false;
  }

  VkPresentModeKHR presentModes[presentModeCount];

  result = m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get supported presentation modes.\n";

    return false;
  }

  VkExtent2D swapChainExtent = surfaceCapabilities.currentExtent;

  VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

  u32 swapChainImagesCount = 2;

  VkSurfaceTransformFlagBitsKHR surfaceTransform;

  if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
  {
    surfaceTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }
  else
  {
    surfaceTransform = surfaceCapabilities.currentTransform;
  }

  VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] =
  { VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
      VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
      VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
  };

  for (uint32_t i = 0; i < sizeof(compositeAlphaFlags); i++)
  {
    if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i])
    {
      compositeAlpha = compositeAlphaFlags[i];

      break;
    }
  }

  VkSwapchainCreateInfoKHR swapchainCreateInfo = {};

  ZeroObject(swapchainCreateInfo);

  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.pNext = NULL;
  swapchainCreateInfo.surface = m_surface;
  swapchainCreateInfo.minImageCount = swapChainImagesCount;
  swapchainCreateInfo.imageFormat = m_swapChainFormat;
  //swapchainCreateInfo.imageExtent.width = swapChainExtent.width;
  //swapchainCreateInfo.imageExtent.height = swapChainExtent.height;
  swapchainCreateInfo.imageExtent.width = m_rect[2];
  swapchainCreateInfo.imageExtent.height = m_rect[3];
  swapchainCreateInfo.preTransform = surfaceTransform;
  swapchainCreateInfo.compositeAlpha = compositeAlpha;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.presentMode = presentMode;
  swapchainCreateInfo.clipped = true;
  swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = NULL;

  u32 queueFamilyIndices[2] = {m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex};

  if (m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex)
  {
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchainCreateInfo.queueFamilyIndexCount = 2;
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  }

  VkSwapchainKHR swapChain = m_swapChain;

  m_swapChain = nullptr;

  swapchainCreateInfo.oldSwapchain = swapChain;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.imageUsage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = nullptr;
  swapchainCreateInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  swapchainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainCreateInfo.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
  swapchainCreateInfo.clipped = VK_TRUE;

  m_vk.vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapChain);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create swap chain.\n";

    return false;
  }

  if (swapChain != VK_NULL_HANDLE)
    m_vk.vkDestroySwapchainKHR(m_device, swapChain, nullptr);

  uint32_t actualImageCount = swapChainImagesCount;

  result = m_vk.vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, nullptr);

  if (result != VK_SUCCESS || actualImageCount == 0)
  {
    kgm_log() << "Vulkan: failed to acquire number of swap chain images.\n";

    return false;
  }

  m_swapChainImages.alloc(actualImageCount);

  result = m_vk.vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, m_swapChainImages);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to acquire swap chain images.\n";

    return false;
  }

  m_swapChainImage = 0;

  return true;
}

bool kgmVulkan::initRenderPass()
{
  VkAttachmentDescription attachmentDescription = {};

  ZeroObject(attachmentDescription);

  attachmentDescription.format = m_swapChainFormat;
  attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
  attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentReference = {};

  ZeroObject(colorAttachmentReference);

  colorAttachmentReference.attachment = 0;
  colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subPassDescription = {};

  ZeroObject(subPassDescription);

  subPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subPassDescription.colorAttachmentCount = 1;
  subPassDescription.pColorAttachments = &colorAttachmentReference;

  VkRenderPassCreateInfo createInfo = {};

  ZeroObject(createInfo);

  createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  createInfo.attachmentCount = 1;
  createInfo.pAttachments = &attachmentDescription;
  createInfo.subpassCount = 1;
  createInfo.pSubpasses = &subPassDescription;

  VkResult result = m_vk.vkCreateRenderPass(m_device, &createInfo, nullptr, &m_renderPass);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create render pass.\n";

    printResult(result);

    return false;
  }

  return true;
}

bool kgmVulkan::initImageViews()
{
  u32 count = m_swapChainImages.length();

  if (count < 1)
  {
    kgm_log() << "Vulak error: Cannot init image views as swap images not initialized.\n";

    return false;
  }

  m_imageViews.alloc(count);

  for (u32 i = 0; i < count; i++)
  {
    VkImageViewCreateInfo createInfo = { };

    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.image = m_swapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = m_swapChainFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VkResult result = m_vk.vkCreateImageView(m_device, &createInfo, nullptr, &m_imageViews[i]);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulak error: failed to create image view for swap chain image #" << i << ".\n";

      printResult(result);

      return false;
    }
  }

  return true;
}

bool kgmVulkan::initFrameBuffers()
{
  u32 count = m_swapChainImages.length();

  if (count < 1)
  {
    kgm_log() << "Vulak error: Cannot init frame buffers as swap images not initialized.\n";

    return false;
  }

  if (m_imageViews.length() < 1)
  {
    kgm_log() << "Vulak error: Cannot init frame buffers as image views not initialized.\n";

    return false;
  }

  m_frameBuffers.alloc(count);

  for (u32 i = 0; i < count; i++)
  {
    VkFramebufferCreateInfo createInfo = {};

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = m_renderPass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &m_imageViews[i];
    createInfo.width = m_rect[2];
    createInfo.height = m_rect[3];
    createInfo.layers = 1;

    VkResult result = m_vk.vkCreateFramebuffer(m_device, &createInfo, nullptr, &m_frameBuffers[i]);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Failed to create framebuffer for swap chain image view #" << i << ".\n";

      return false;
    }
  }

  return true;
}

bool kgmVulkan::initDepthBuffer()
{
  VkImageCreateInfo infoImage = {};

  const VkFormat format = VK_FORMAT_D16_UNORM;

  VkFormatProperties properties;

  m_vk.vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &properties);

  if (properties.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
  {
    infoImage.tiling = VK_IMAGE_TILING_LINEAR;
  }
  else if (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
  {
    infoImage.tiling = VK_IMAGE_TILING_OPTIMAL;
  }
  else
  {
    // Try other depth formats?
    kgm_log() << "Vulkan error: VK_FORMAT_D16_UNORM Unsupported.\n";

    return false;
  }

  infoImage.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  infoImage.pNext = NULL;
  infoImage.imageType = VK_IMAGE_TYPE_2D;
  infoImage.format = format;
  infoImage.extent.width = m_rect[2];
  infoImage.extent.height = m_rect[3];
  infoImage.extent.depth = 1;
  infoImage.mipLevels = 1;
  infoImage.arrayLayers = 1;
  infoImage.samples = VK_SAMPLE_COUNT_16_BIT;
  infoImage.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  infoImage.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  infoImage.queueFamilyIndexCount = 0;
  infoImage.pQueueFamilyIndices = NULL;
  infoImage.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  infoImage.flags = 0;

  VkMemoryAllocateInfo alloc = {};

  alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc.pNext = NULL;
  alloc.allocationSize = 0;
  alloc.memoryTypeIndex = 0;

  VkImageViewCreateInfo infoView = {};
  infoView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  infoView.pNext = NULL;
  infoView.image = VK_NULL_HANDLE;
  infoView.format = format;
  infoView.components.r = VK_COMPONENT_SWIZZLE_R;
  infoView.components.g = VK_COMPONENT_SWIZZLE_G;
  infoView.components.b = VK_COMPONENT_SWIZZLE_B;
  infoView.components.a = VK_COMPONENT_SWIZZLE_A;
  infoView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  infoView.subresourceRange.baseMipLevel = 0;
  infoView.subresourceRange.levelCount = 1;
  infoView.subresourceRange.baseArrayLayer = 0;
  infoView.subresourceRange.layerCount = 1;
  infoView.viewType = VK_IMAGE_VIEW_TYPE_2D;
  infoView.flags = 0;

  VkMemoryRequirements requirements;

  m_depthFormat = format;

  VkResult result = m_vk.vkCreateImage(m_device, &infoImage, NULL, &m_depthImage);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create depth image.\n";

    printResult(result);

    return false;
  }

  m_vk.vkGetImageMemoryRequirements(m_device, m_depthImage, &requirements);

  alloc.allocationSize = requirements.size;

  result = m_vk.vkAllocateMemory(m_device, &alloc, NULL, &m_depthMemory);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot allocate memory for depth image.\n";

    printResult(result);

    return false;
  }

  result = m_vk.vkBindImageMemory(m_device, m_depthImage, m_depthMemory, 0);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot bind memory to depth image.\n";

    printResult(result);

    return false;
  }

  return true;
}

bool kgmVulkan::initSemaphores()
{
  VkSemaphoreCreateInfo createInfo = {};

  ZeroObject(createInfo);

  createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if (m_vk.vkCreateSemaphore(m_device, &createInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
      m_vk.vkCreateSemaphore(m_device, &createInfo, nullptr, &m_renderingFinishedSemaphore) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create semaphores.\n";

    return false;
  }

  return true;
}

bool kgmVulkan::initPipeline()
{
  if (!m_device)
  {
    kgm_log() << "Vulkan error: Failed create pipelines as no device.\n";

    return false;
  }

  return true;
}

bool kgmVulkan::refreshSwapchain()
{
  if(!m_instance || !m_device || !m_surface)
  {
    kgm_log() << "Vulkan error: Device is not valid.\n";

    return false;
  }

  VkResult result = m_vk.vkDeviceWaitIdle(m_device);

  if (result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: device wait idle failed.\n";

    printResult(result);

    return false;
  }

  m_vk.vkFreeCommandBuffers(m_device, m_commandPool, (u32) m_commandBuffers.length(), m_commandBuffers.data());

  m_commandBuffers.clear();

  kgm_log() << "Vulkan: Command buffer deleted.\n";

  if (m_renderPass)
  {
    m_vk.vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    m_renderPass = VK_NULL_HANDLE;
  }

  kgm_log() << "Vulkan: Render pass destroyed.\n";

  for (size_t i = 0; i < m_swapChainImages.length(); i++)
  {
    m_vk.vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr);
    m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
  }

  m_frameBuffers.clear();
  m_imageViews.clear();
  m_swapChainImages.clear();

  kgm_log() << "Vulkan: Framebuffers/Imageviews are destroyed.\n";

  //m_vk.vkDestroyDescriptorSetLayout(m_device, descriptorSetLayout, nullptr);

  initSwapchain();
  initRenderPass();
  initImageViews();
  initFrameBuffers();
  initCommands();

  return true;
}

void kgmVulkan::printResult(VkResult result)
{
  switch(result)
  {
  case VK_SUCCESS:
    kgm_log() << "Vulkan result: Is success.\n";
  break;
  case VK_NOT_READY:
    kgm_log() << "Vulkan result: Not ready.\n";
  break;
  case VK_TIMEOUT:
    kgm_log() << "Vulkan result: Timeout.\n";
  break;
  case VK_EVENT_SET:
    kgm_log() << "Vulkan result: Event set.\n";
  break;
  case VK_EVENT_RESET:
    kgm_log() << "Vulkan result: Event reset.\n";
  break;
  case VK_INCOMPLETE:
    kgm_log() << "Vulkan result: Incomplete.\n";
  break;
  case VK_ERROR_OUT_OF_HOST_MEMORY:
    kgm_log() << "Vulkan result: Error out of host memory.\n";
  break;
  case VK_ERROR_OUT_OF_DEVICE_MEMORY:
    kgm_log() << "Vulkan result: Error of device memory.\n";
  break;
  case VK_ERROR_INITIALIZATION_FAILED:
    kgm_log() << "Vulkan result: Error initialization failed.\n";
  break;
  case VK_ERROR_DEVICE_LOST:
    kgm_log() << "Vulkan result: Error device lost.\n";
  break;
  case VK_ERROR_MEMORY_MAP_FAILED:
    kgm_log() << "Vulkan result: Error memory map failed.\n";
  break;
  case VK_ERROR_LAYER_NOT_PRESENT:
    kgm_log() << "Vulkan result: Error laayer not present.\n";
  break;
  case VK_ERROR_EXTENSION_NOT_PRESENT:
    kgm_log() << "Vulkan result: Error extension not present.\n";
  break;
  case VK_ERROR_FEATURE_NOT_PRESENT:
    kgm_log() << "Vulkan result: Error feature not present.\n";
  break;
  case VK_ERROR_INCOMPATIBLE_DRIVER:
    kgm_log() << "Vulkan result: Error incompatible driver.\n";
  break;
  case VK_ERROR_TOO_MANY_OBJECTS:
    kgm_log() << "Vulkan result: Error too maby objects.\n";
  break;
  case VK_ERROR_FORMAT_NOT_SUPPORTED:
    kgm_log() << "Vulkan result: Error format not supported.\n";
  break;
  case VK_ERROR_FRAGMENTED_POOL:
    kgm_log() << "Vulkan result: Error fragmented pool.\n";
  break;
  case VK_ERROR_SURFACE_LOST_KHR:
    kgm_log() << "Vulkan result: Error surface lost.\n";
  break;
  case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
    kgm_log() << "Vulkan result: Error native window in use.\n";
  break;
  case VK_SUBOPTIMAL_KHR:
    kgm_log() << "Vulkan result: Suboptimal.\n";
  break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    kgm_log() << "Vulkan result: Error out of date.\n";
  break;
  case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
    kgm_log() << "Vulkan result: Error incompatible display.\n";
  break;
  case VK_ERROR_VALIDATION_FAILED_EXT:
    kgm_log() << "Vulkan result: Error validation failed.\n";
  break;
  case VK_ERROR_INVALID_SHADER_NV:
    kgm_log() << "Vulkan result: Error invalid shaders.\n";
  break;
  default:
    kgm_log() << "Vulkan result: Error unknown.\n";
  }
}

#endif
