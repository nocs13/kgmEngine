#include "kgmVulkan.h"
#include "../kgmBase/kgmLog.h"
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

  if (g_vulkans < 1)
  {
    if (vkInit() != 1 || m_vk.vkCreateInstance == nullptr)
      return;
  }

  g_vulkans++;

  s32 wrc[4];

  wnd->getRect(wrc[0], wrc[1], wrc[2], wrc[3]);

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

  VkSwapchainKHR swapChain;

  VkSwapchainCreateInfoKHR swapChainCreateInfo;

  memset(&swapChainCreateInfo, 0, sizeof(VkSwapchainCreateInfoKHR));

  swapChainCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapChainCreateInfo.pNext = nullptr;
  swapChainCreateInfo.flags = 0;
  swapChainCreateInfo.surface = surface;
  swapChainCreateInfo.minImageCount = 2;
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
  } */

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
      m_vk.vkDestroyFramebuffer(m_device, m_framebuffers[i], nullptr);
      m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
    }

    m_swapChainImages.clear();
    m_imageViews.clear();
    m_framebuffers.clear();


    if (m_swapChain != VK_NULL_HANDLE)
    {
      m_vk.vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);

      kgm_log() << "Vulkan: SwapChain destroyed.\n";
    }

    m_vk.vkDestroyDevice(m_device, nullptr);

    kgm_log() << "Vulkan: Device destroyed.\n";
  }

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

  m_vk.vkGetInstanceProcAddr = (typeof m_vk.vkGetInstanceProcAddr) vk_lib.get((char*) "vkGetInstanceProcAddr");
  m_vk.vkCreateInstance = (typeof m_vk.vkCreateInstance) vk_lib.get((char*) "vkCreateInstance");
  m_vk.vkDestroyInstance = (typeof m_vk.vkDestroyInstance) vk_lib.get((char*) "vkDestroyInstance");
  m_vk.vkEnumerateInstanceExtensionProperties = (typeof m_vk.vkEnumerateInstanceExtensionProperties) vk_lib.get((char*) "vkEnumerateInstanceExtensionProperties");
  m_vk.vkEnumeratePhysicalDevices = (typeof m_vk.vkEnumeratePhysicalDevices) vk_lib.get((char*) "vkEnumeratePhysicalDevices");
  m_vk.vkEnumerateDeviceExtensionProperties = (typeof m_vk.vkEnumerateDeviceExtensionProperties) vk_lib.get((char*) "vkEnumerateDeviceExtensionProperties");
  m_vk.vkGetPhysicalDeviceQueueFamilyProperties = (typeof m_vk.vkGetPhysicalDeviceQueueFamilyProperties) vk_lib.get((char*) "vkGetPhysicalDeviceQueueFamilyProperties");
  m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
  m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceFormatsKHR");
  m_vk.vkGetPhysicalDeviceSurfaceSupportKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceSupportKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceSupportKHR");
  m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR = (typeof m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfacePresentModesKHR");
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

void kgmVulkan::gcResize(u32 width, u32 height)
{
  m_rect[2] = width;
  m_rect[3] = height;

  kgm_log() << "Vulkan: Viewport need update.\n";

  if(!m_instance || !m_device || !m_surface)
  {
    kgm_log() << "Vulkan error: Device is not valid.\n";

    return;
  }

  VkResult result = m_vk.vkDeviceWaitIdle(m_device);

  if (result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: device wait idle failed.\n";

    printResult(result);

    return;
  }

  m_vk.vkFreeCommandBuffers(m_device, m_commandPool, (u32) m_commandBuffers.length(), m_commandBuffers.data());

  kgm_log() << "Vulkan: Command buffer deleted.\n";

  //m_vk.vkDestroyPipeline(m_device, graphicsPipeline, nullptr);
  m_vk.vkDestroyRenderPass(m_device, m_renderPass, nullptr);

  kgm_log() << "Vulkan: Render pass destroyed.\n";

  for (size_t i = 0; i < m_swapChainImages.length(); i++)
  {
    m_vk.vkDestroyFramebuffer(m_device, m_framebuffers[i], nullptr);
    m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
  }

  m_swapChainImages.clear();

  kgm_log() << "Vulkan: Framebuffers/Imageviews are destroyed.\n";

  //m_vk.vkDestroyDescriptorSetLayout(m_device, descriptorSetLayout, nullptr);

  createSwapChain();
  createRenderPass();
  createImageViews();
  createFramebuffers();
  createCommandBuffers();
}

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

    printResult(result);

    return;
  }

  kgm_log() << "Vulkan: Current swapchain image is " << swapChainImage << ".\n";

  result = m_vk.vkQueueWaitIdle(m_queue);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for queue.\n";

    printResult(result);

    return;
  }

  kgm_log() << "Vulkan: Idle wait for queue passed.\n";

  result = m_vk.vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, 1000);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for fence.\n";

    printResult(result);

    return;
  }

  kgm_log() << "Vulkan: Wait for fence passed.\n";

  result = m_vk.vkResetFences(m_device, 1, &m_fence);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to reset fence.\n";

    printResult(result);

    return;
  }

  kgm_log() << "Vulkan: Reset fence passed.\n";

  auto &commandBuffer = m_commandBuffers[m_swapChainImage];

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

  result = m_vk.vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to submit command buffer.\n";

    printResult(result);

    return;
  }

  kgm_log() << "Vulkan: Submit queue passed.\n";

  result = m_vk.vkQueueWaitIdle(m_queue);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to wait for queue.\n";

    printResult(result);

    return;
  }

  kgm_log() << "Vulkan: Idle wait for queue passed again.\n";

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


  if(m_vk.vkQueuePresentKHR(m_queue, &presentInfo) != VkResult::VK_SUCCESS || result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to present swapchain.\n";

    printResult(result);

    return;
  }

  kgm_log() << "Vulkan: Queue present passed.\n";

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

void kgmVulkan::clean(u32 options)
{

}

bool kgmVulkan::initInstance()
{
  if(m_instance != nullptr)
  {
    kgm_log() << "Vulkan error: Instance already initialized.\n";

    return false;
  }

  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = "";
  appInfo.applicationVersion = 1;
  appInfo.pEngineName = "";
  appInfo.engineVersion = 1;
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo instanceInfo = {};
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext = NULL;
  instanceInfo.flags = 0;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledExtensionCount = 0;
  instanceInfo.ppEnabledExtensionNames = NULL;
  instanceInfo.enabledLayerCount = 0;
  instanceInfo.ppEnabledLayerNames = NULL;

  VkResult result;

  result = m_vk.vkCreateInstance(&instanceInfo, NULL, &m_instance);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Unable to create instance.\n";
    printResult(result);

    return false;
  }

  return true;
}

void kgmVulkan::createSwapChain()
{
  VkResult result;

  VkSurfaceCapabilitiesKHR surfaceCapabilities;

  result = m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilities);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: failed to get surface capabilities.\n";

    printResult(result);

    return;
  }

  u32 surfaceFormatsCount = 0;

  result = m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatsCount, nullptr);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get surface formats count.\n";

    printResult(result);

    return;
  }

  VkSurfaceFormatKHR surfaceFormats[surfaceFormatsCount];

  result = m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatsCount, surfaceFormats);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get surface formats.\n";

    printResult(result);

    return;
  }

  kgm_log() << "Vulkan: Got surface formats.\n";

  // Find supported present modes
  u32 presentModeCount;

  result = m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);

  if (result != VK_SUCCESS || presentModeCount == 0)
  {
    kgm_log() << "Vulkan error: Failed to get number of supported presentation modes.\n";

    return;
  }

  VkPresentModeKHR presentModes[presentModeCount];

  result = m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get supported presentation modes.\n";

    return;
  }

  // Determine number of images for swap chain
  u32 imageCount = surfaceCapabilities.minImageCount + 1;

  if (surfaceCapabilities.maxImageCount != 0 && imageCount > surfaceCapabilities.maxImageCount)
  {
    imageCount = surfaceCapabilities.maxImageCount;
  }

  kgm_log() << "Vulkan: using " << imageCount << " images for swap chain.\n";

  // Select a surface format
  VkSurfaceFormatKHR surfaceFormat = { surfaceFormats[0].format, surfaceFormats[0].colorSpace };

  // Select swap chain size
  VkExtent2D swapChainExtent; // = chooseSwapExtent(surfaceCapabilities);

  swapChainExtent.width  = m_rect[2];
  swapChainExtent.height = m_rect[3];

  // Determine transformation to use (preferring no transform)
  VkSurfaceTransformFlagBitsKHR surfaceTransform;

  if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
  {
    surfaceTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }
  else
  {
    surfaceTransform = surfaceCapabilities.currentTransform;
  }

  // Choose presentation mode (preferring MAILBOX ~= triple buffering)
  VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

  for (const auto &i: presentModes)
  {
    if (i == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      presentMode == i;

      break;
    }
  }

  // Finally, create the swap chain
  VkSwapchainCreateInfoKHR createInfo;

  ZeroObject(createInfo);

  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = m_surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = swapChainExtent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  createInfo.queueFamilyIndexCount = 0;
  createInfo.pQueueFamilyIndices = nullptr;
  createInfo.preTransform = surfaceTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  VkSwapchainKHR swapChain = m_swapChain;

  createInfo.oldSwapchain = swapChain;

  m_swapChain = nullptr;

  m_vk.vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create swap chain.\n";

    return;
  }

  kgm_log() << "Vulkan: Created swap chain.\n";

  if (swapChain != VK_NULL_HANDLE)
  {
    m_vk.vkDestroySwapchainKHR(m_device, swapChain, nullptr);

    swapChain = m_swapChain;
  }

  m_swapChainFormat = surfaceFormat.format;

  // Store the images used by the swap chain
  // Note: these are the images that swap chain image indices refer to
  // Note: actual number of images may differ from requested number, since it's a lower bound
  uint32_t actualImageCount = 0;

  result = m_vk.vkGetSwapchainImagesKHR(m_device, swapChain, &actualImageCount, nullptr);

  if (result != VK_SUCCESS || actualImageCount == 0)
  {
    kgm_log() << "Vulkan: failed to acquire number of swap chain images.\n";

    return;
  }

  m_swapChainImages.alloc(actualImageCount);

  result = m_vk.vkGetSwapchainImagesKHR(m_device, swapChain, &actualImageCount, m_swapChainImages.data());

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to acquire swap chain images.\n";

    return;
  }

  kgm_log() << "Vulcan: Acquired swap chain images.\n";
}

void kgmVulkan::createRenderPass()
{
  VkAttachmentDescription attachmentDescription;

  ZeroObject(attachmentDescription);

  attachmentDescription.format = m_swapChainFormat;
  attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
  attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  // Note: hardware will automatically transition attachment to the specified layout
  // Note: index refers to attachment descriptions array
  VkAttachmentReference colorAttachmentReference;

  ZeroObject(colorAttachmentReference);

  colorAttachmentReference.attachment = 0;
  colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // Note: this is a description of how the attachments of the render pass will be used in this sub pass
  // e.g. if they will be read in shaders and/or drawn to
  VkSubpassDescription subPassDescription = {};

  ZeroObject(subPassDescription);

  subPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subPassDescription.colorAttachmentCount = 1;
  subPassDescription.pColorAttachments = &colorAttachmentReference;

  // Create the render pass
  VkRenderPassCreateInfo createInfo;

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

    return;
  }
  else
  {
    kgm_log() << "Vulkan: Created render pass.\n";
  }
}

void kgmVulkan::createImageViews()
{
  u32 swi = m_swapChainImages.length();

  m_imageViews.alloc(m_swapChainImages.length());

  // Create an image view for every image in the swap chain
  for (u32 i = 0; i < m_swapChainImages.length(); i++)
  {
    VkImageViewCreateInfo createInfo;

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = m_swapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = m_swapChainFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VkResult result = m_vk.vkCreateImageView(m_device, &createInfo, nullptr, &m_imageViews[i]);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulak error: failed to create image view for swap chain image #" << i << ".\n";

      return;
    }
  }

  kgm_log() << "Vulkan: Created image views for swap chain images.\n";
}

void kgmVulkan::createFramebuffers()
{
  m_framebuffers.alloc(m_swapChainImages.length());

  // Note: Framebuffer is basically a specific choice of attachments for a render pass
  // That means all attachments must have the same dimensions, interesting restriction
  for (u32 i = 0; i < m_swapChainImages.length(); i++)
  {
    VkFramebufferCreateInfo createInfo;

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = m_renderPass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &m_imageViews[i];
    createInfo.width = m_rect[2];
    createInfo.height = m_rect[3];
    createInfo.layers = 1;

    VkResult result = m_vk.vkCreateFramebuffer(m_device, &createInfo, nullptr, &m_framebuffers[i]);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Failed to create framebuffer for swap chain image view #" << i << ".\n";

      return;
    }
  }

  kgm_log() << "Vulkan: Created framebuffers for swap chain image views.\n";
}

void kgmVulkan::createGraphicsPipeline()
{

}

void kgmVulkan::createCommandBuffers()
{
  // Allocate graphics command buffers
  m_commandBuffers.alloc(m_swapChainImages.length());

  VkCommandBufferAllocateInfo allocInfo;

  ZeroObject(allocInfo);

  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = m_commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (u32) m_swapChainImages.length();

  VkResult result = m_vk.vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data());

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to allocate graphics command buffers.\n";

    return;
  }
  else
  {
    kgm_log() << "Vulkan: Allocated command buffers." << "\n";
  }

  // Prepare data for recording command buffers
  VkCommandBufferBeginInfo beginInfo;

  ZeroObject(beginInfo);

  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

  VkImageSubresourceRange subResourceRange;

  ZeroObject(subResourceRange);

  subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subResourceRange.baseMipLevel = 0;
  subResourceRange.levelCount = 1;
  subResourceRange.baseArrayLayer = 0;
  subResourceRange.layerCount = 1;

  VkClearValue clearColor =
  {
    { 0.1f, 0.1f, 0.1f, 1.0f } // R, G, B, A
  };

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
    commandBufferInheritanceInfo.renderPass = m_renderPass;
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
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.framebuffer = m_framebuffers[i];
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
