#include "kgmVulkan.h"

#ifdef VULKAN


u32           kgmVulkan::g_vulkans = 0;
kgmLib        kgmVulkan::vk_lib;
kgmVulkan::vk kgmVulkan::m_vk      = {0};

kgmVulkan::kgmVulkan()
{
  m_instance = 0;

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
  if (!vk_lib.open((char*) "vulkan.so"))
    if (!vk_lib.open((char*) "vulkan.so.1"))
      return -2;
#endif

  #pragma GCC diagnostic push
  //#pragma GCC diagnostic warning "-fpermissive"

  m_vk.vkCreateInstance = (VkResult (*)(const VkInstanceCreateInfo*, const VkAllocationCallbacks*, VkInstance_T**)) vk_lib.get((char*) "vkCreateInstance");
  m_vk.vkDestroyInstance = (void (*)(VkInstance, const VkAllocationCallbacks*)) vk_lib.get((char*) "vkDestroyInstance");

  #pragma GCC diagnostic pop

  return 1;
}

void kgmVulkan::vkFree()
{
  if (vk_lib.active())
    vk_lib.close();
}

#endif
