#pragma once

#include "../kgmBase/kgmIGC.h"

#include "kgmWindow.h"
#include "kgmLib.h"

#ifndef VULKAN
#define VULKAN
#endif

#ifdef VULKAN

#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#else
#define VK_USE_PLATFORM_XLIB_KHR
#endif

#include "inc/vk/vulkan.h"

//struct VkXlibSurfaceCreateInfoKHR;

class kgmVulkan: public kgmIGC
{
  struct vk
  {
    VkResult (VKAPI_PTR *vkCreateInstance)(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
    void     (VKAPI_PTR *vkDestroyInstance)(VkInstance instance, const VkAllocationCallbacks* pAllocator);

#ifdef WIN32
    VkResult (VKAPI_PTR *vkCreateWin32SurfaceKHR)(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#else
    VkResult (VKAPI_PTR *vkCreateXlibSurfaceKHR)(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif
  };

  static kgmLib vk_lib;  
  static vk     m_vk;
  static u32    g_vulkans;

  VkInstance m_instance;

public:
  kgmVulkan(kgmWindow* wnd);
  ~kgmVulkan();

  void  gcSet(u32 param, void* value);
  void  gcGet(u32 param, void* value);
  void  gcClear(u32 flag, u32 col, float depth, u32 sten);
  void  gcBegin();
  void  gcEnd();
  void  gcRender();
  void  gcSetTarget(void*  rt);

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
  void  gcBlend(bool, u32, u32);

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
#ifdef DEBUG
  void  gcGetUniform(void* s, const char*, void*);
#endif

private:
  static int  vkInit();
  static void vkFree();
};

#endif
