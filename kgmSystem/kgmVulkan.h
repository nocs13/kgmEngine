#pragma once

#include "../kgmBase/kgmIGC.h"

#include "kgmWindow.h"
#include "kgmLib.h"

#ifndef VULKAN
#define VULKAN
#endif

#ifdef VULKAN

#ifdef WIN32

#ifndef _WIN32
#define _WIN32
#endif

#define VK_USE_PLATFORM_WIN32_KHR

#else

#define VK_USE_PLATFORM_XLIB_KHR

#endif

#include "inc/vk/vulkan.h"

#if !defined(VK_EXPORTED_FUNCTION)
#define VK_EXPORTED_FUNCTION( fn ) PFN_##fn fn;
#endif

//struct VkXlibSurfaceCreateInfoKHR;

class kgmVulkan: public kgmIGC
{
  struct vk
  {
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

    PFN_vkCreateInstance vkCreateInstance;
    PFN_vkDestroyInstance vkDestroyInstance;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices ;
    PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
    PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR     vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR     vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
    PFN_vkCreateDevice vkCreateDevice;
    PFN_vkDestroyDevice vkDestroyDevice;
    PFN_vkGetDeviceQueue vkGetDeviceQueue;
    PFN_vkQueueWaitIdle vkQueueWaitIdle;
    PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    PFN_vkCreateSemaphore vkCreateSemaphore;
    PFN_vkCreateCommandPool vkCreateCommandPool;
    PFN_vkResetCommandPool  vkResetCommandPool;
    PFN_vkDestroyCommandPool vkDestroyCommandPool;
    PFN_vkCreateRenderPass vkCreateRenderPass;
    PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
    PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
    PFN_vkCreateImage vkCreateImage;
    PFN_vkCreateImageView vkCreateImageView;
    PFN_vkCreateFramebuffer vkCreateFramebuffer;
    PFN_vkAllocateMemory vkAllocateMemory;
    PFN_vkBindImageMemory vkBindImageMemory;
    PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
    PFN_vkResetCommandBuffer vkResetCommandBuffer;
    PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
    PFN_vkEndCommandBuffer vkEndCommandBuffer;
    PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
    PFN_vkCreateFence vkCreateFence;
    PFN_vkResetFences vkResetFences;
    PFN_vkWaitForFences vkWaitForFences;
    PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
    PFN_vkQueueSubmit vkQueueSubmit;
    PFN_vkQueuePresentKHR vkQueuePresentKHR;
    PFN_vkCreateShaderModule vkCreateShaderModule;
    PFN_vkDestroyShaderModule vkDestroyShaderModule;
    PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
    PFN_vkDestroyPipeline vkDestroyPipeline;
    PFN_vkDestroyRenderPass vkDestroyRenderPass;
    PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
    PFN_vkDestroyImageView vkDestroyImageView;
    PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
    PFN_vkFreeCommandBuffers vkFreeCommandBuffers;

    VK_EXPORTED_FUNCTION(vkDestroySurfaceKHR);

    PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
    VK_EXPORTED_FUNCTION(vkDestroyDebugReportCallbackEXT);

#ifdef WIN32
    VkResult (VKAPI_PTR *vkCreateWin32SurfaceKHR)(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#else
    VkResult (VKAPI_PTR *vkCreateXlibSurfaceKHR)(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif
  };

  struct Shader
  {
    VkShaderModule vertex;
    VkShaderModule fragment;
  };

  static kgmLib vk_lib;  
  static vk     m_vk;
  static u32    g_vulkans;

  kgmWindow* m_window = null;

  VkInstance   m_instance = 0;
  VkSurfaceKHR m_surface  = 0;
  VkDevice     m_device   = 0;

  VkPhysicalDevice m_physicalDevice = 0;

  VkSwapchainKHR   m_swapChain      = 0;
  VkRenderPass     m_renderPass     = 0;
  VkCommandPool    m_commandPool    = 0;

  VkFence          m_fence           = 0;
  VkQueue          m_queue           = 0;

  kgmArray<VkPhysicalDevice> m_physicalDevices;
  kgmArray<VkImage> m_swapChainImages;
  kgmArray<VkFramebuffer> m_frameBuffers;
  kgmArray<VkImageView> m_imageViews;
  kgmArray<VkCommandBuffer> m_commandBuffers;

  VkFormat m_swapChainFormat;

  VkFormat m_depthFormat;
  VkImage  m_depthImage;
  VkImageView  m_depthImageView;
  VkDeviceMemory m_depthMemory;

  u32 m_swapChainImage;
  u32 m_graphicsQueueFamilyCount =  0;
  u32 m_graphicsQueueFamilyIndex = -1;
  u32 m_presentQueueFamilyIndex  = -1;

  s32 m_rect[4];

  u32 m_error = 0;

#ifdef DEBUG
  VkDebugReportCallbackEXT m_debugReportCallback = 0;
#endif

public:
  kgmVulkan(kgmWindow* wnd);
  ~kgmVulkan();

  u32   gcError();

  //BASE
  gc_enum gcGetBase() { return gc_vulkan; }

  void  gcSet(u32 param, void* value);
  void  gcGet(u32 param, void* value);
  void  gcClear(u32 flag, u32 col, float depth, u32 sten);
  void  gcBegin();
  void  gcEnd();
  void  gcRender();
  void  gcSetTarget(void*  rt);

  void gcResize(u32 width, u32 height);

  // DRAWING
  void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt);

  // TEXTURE
  void* gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type);
  void  gcFreeTexture(void *t);
  void  gcSetTexture(u32 stage, void *t);

  // MATRIX
  void  gcSetMatrix(u32 mode, float* mtx);
  void  gcGetMatrix(u32 mode, float* mtx);
  void  gcSetViewport(int x, int y, int w, int h, float n, float f);

  //BLEND
  void  gcBlend(bool, u32, u32, u32);

  //ALPHA
  void  gcAlpha(bool, u32, float);

  //CULL
  void  gcCull(u32 mode);

  //DEPTH
  void  gcDepth(bool en, bool mask, u32 mode);

  //LIGHT
  void gcSetLight(int i, float* pos, float forse, float* col, float* dir, float angle);

  //VERTEX & INDEX BUFFERS
  void* gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize);
  void  gcFreeVertexBuffer(void*);
  void  gcDrawVertexBuffer(void* buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff);

  // SHADER
  void* gcGenShader(const char*, const char*);
  void  gcFreeShader(void* s);
  void  gcSetShader(void* s);
  void  gcBindAttribute(void* s, int, const char*);
  void  gcUniform(void* s, u32, u32, const char*, void*);
  void  gcUniformMatrix(void* s, u32, u32, u32, const char*, void*);
  void  gcUniformSampler(void* s, const char*, void*);

  // TARGET
  gchandle gcGenTarget(u32 w, u32 h, bool depth, bool stencil);
  bool     gcTexTarget(gchandle tar, gchandle tex, u32 type);
  void     gcFreeTarget(gchandle t);

#ifdef DEBUG
  void  gcGetUniform(void* s, const char*, void*);
#endif

private:
  static int  vkInit();
  static void vkFree();

  void printResult(VkResult result);

  void clean(u32);
  void freeSurface();

  bool initInstance();
  bool initDebug();
  bool listDevices();
  bool initDevice();
  bool initSurface();
  bool initCommands();
  bool initSwapchains();
  bool initRenderPass();
  bool initImageViews();
  bool initFrameBuffers();
  bool initDepthBuffer();

  /*
  void createSwapChain();
  void createRenderPass();
  void createImageViews();
  void createFramebuffers();
  void createGraphicsPipeline();
  void createCommandBuffers();
  */
};

#endif
