#include "kgmVulkan.h"

kgmVulkan::kgmVulkan()
{
  m_instance = 0;

  m_vk;

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
}

int kgmVulkan::vkInit()
{
  if (m_vk.lib.active())
    return -1;

#ifdef WIN32
  if (!m_vk.lib.open((char*) "vulkan.dll"))
    return -2;
#else
  if (!m_vk.lib.open((char*) "vulkan.so"))
    return -2;
#endif

  //#pragma GCC diagnostic push
  //#pragma GCC diagnostic warning "-fpermissive"

  //m_vk.vkCreateInstance = (VkResult (*)(const VkInstanceCreateInfo*, const VkAllocationCallbacks*, VkInstance_T**)) m_vk.lib.get((char*) "vkCreateInstance");
  //m_vk.vkDestroyInstance = (void (*)(VkInstance, const VkAllocationCallbacks*)) m_vk.lib.get((char*) "vkDestroyInstance");

  //#pragma GCC diagnostic pop

  return 1;
}

void kgmVulkan::vkFree()
{
  if (m_vk.lib.active())
    m_vk.lib.close();
}
