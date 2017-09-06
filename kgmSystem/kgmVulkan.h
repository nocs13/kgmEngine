#pragma once

#include "../kgmBase/kgmIGC.h"

#include "kgmWindow.h"
#include "kgmLib.h"

#ifdef VULKAN

#include "inc/vk/vulkan.h"

class kgmVulkan: public kgmIGC
{
  struct vk
  {
    VkResult (VKAPI_PTR *vkCreateInstance)(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
    void (VKAPI_PTR *vkDestroyInstance)(VkInstance instance, const VkAllocationCallbacks* pAllocator);
  };

  static kgmLib vk_lib;  
  static vk     m_vk;
  static u32    g_vulkans;

  VkInstance m_instance;

public:
  kgmVulkan();
  ~kgmVulkan();

private:
  static int  vkInit();
  static void vkFree();
};

#endif
