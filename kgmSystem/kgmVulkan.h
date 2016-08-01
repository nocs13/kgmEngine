#pragma once

#include "../kgmBase/kgmIGC.h"

#include "kgmWindow.h"
#include "kgmLib.h"

#include "inc/vk/vulkan.h"

class kgmVulkan: public kgmIGC
{
  struct vk
  {
    kgmLib lib;

    VkResult (VKAPI_PTR *vkCreateInstance)(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
    void (VKAPI_PTR *vkDestroyInstance)(VkInstance instance, const VkAllocationCallbacks* pAllocator);
  };

  static vk m_vk;

  VkInstance m_instance;

public:
  kgmVulkan();
  ~kgmVulkan();

private:
  static int  vkInit();
  static void vkFree();
};
