#include "kgmVulkan.h"
#include "../kgmBase/kgmLog.h"

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
  VkPhysicalDeviceFeatures enabledFeatures = {};
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

  VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

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
  else
  {
    kgm_log() << "Vulkan: created semaphores.\n";
  }

  VkCommandPool commandPool;
  VkCommandPoolCreateInfo poolCreateInfo = {};
  poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolCreateInfo.queueFamilyIndex = graphicsQueueFamily;

  if (m_vk.vkCreateCommandPool(device, &poolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan: failed to create command queue for graphics queue family.\n";

    return;
  }
  else
  {
    kgm_log() << "Vulkan: created command pool for graphics queue family.\n";
  }

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
  m_vk.vkGetPhysicalDeviceSurfaceSupportKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceSupportKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceSupportKHR");
  m_vk.vkCreateDevice = (typeof m_vk.vkCreateDevice) vk_lib.get((char*) "vkCreateDevice");
  m_vk.vkGetDeviceQueue = (typeof m_vk.vkGetDeviceQueue) vk_lib.get((char*) "vkGetDeviceQueue");
  m_vk.vkGetPhysicalDeviceMemoryProperties = (typeof m_vk.vkGetPhysicalDeviceMemoryProperties) vk_lib.get((char*) "vkGetPhysicalDeviceMemoryProperties");
  m_vk.vkCreateSemaphore = (typeof m_vk.vkCreateSemaphore) vk_lib.get((char*) "vkCreateSemaphore");
  m_vk.vkCreateCommandPool= (typeof m_vk.vkCreateCommandPool) vk_lib.get((char*) "vkCreateCommandPool");

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
void  kgmVulkan::gcRender() {}
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
void  kgmVulkan::gcSetViewport(int x, int y, int w, int h, float n, float f) {}

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
