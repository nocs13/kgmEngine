#include "kgmVulkan.h"

#ifdef VULKAN

#include "inc/vk/vulkan.h"

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

  VkApplicationInfo appInfo;

  memset(&appInfo, 0, sizeof(VkApplicationInfo));

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo;

  memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));

  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  if (m_vk.vkCreateInstance(&createInfo, null, &m_instance) != VK_SUCCESS)
  {
    return;
  }

  VkSurfaceKHR surface;

#ifdef WIN32
  VkWin32SurfaceCreateInfoKHR sfCreateInfo;
  sfCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  sfCreateInfo.hwnd = wnd->m_wnd;
  sfCreateInfo.hinstance = GetModuleHandle(nullptr);

  if (m_vk.vkCreateWin32SurfaceKHR(m_instance, &sfCreateInfo, nullptr, &surface) != VK_SUCCESS)
  {
    return;
  }
#else
  VkXlibSurfaceCreateInfoKHR sfCreateInfo;
  sfCreateInfo.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
  sfCreateInfo.window = wnd->m_wnd;
  sfCreateInfo.dpy    = wnd->m_dpy;

  if (m_vk.vkCreateXlibSurfaceKHR(m_instance, &sfCreateInfo, nullptr, &surface) != VK_SUCCESS)
  {
    return;
  }
#endif
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
    return -2;
#else
  if (!vk_lib.open((char*) "libvulkan.so"))
    if (!vk_lib.open((char*) "libvulkan.so.1"))
      return -2;
#endif

  #pragma GCC diagnostic push
  //#pragma GCC diagnostic ignored "-fpermissive"

  m_vk.vkCreateInstance = (VkResult (*)(const VkInstanceCreateInfo*, const VkAllocationCallbacks*, VkInstance_T**)) vk_lib.get((char*) "vkCreateInstance");
  m_vk.vkDestroyInstance = (void (*)(VkInstance, const VkAllocationCallbacks*)) vk_lib.get((char*) "vkDestroyInstance");

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

// MATRIX
void  kgmVulkan::gcSetMatrix(u32 mode, float* mtx) {}
void  kgmVulkan::gcGetMatrix(u32 mode, float* mtx) {}
void  kgmVulkan::gcSetViewport(int x, int y, int w, int h, float n, float f) {}

//BLEND
void  kgmVulkan::gcBlend(bool, u32, u32) {}

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
