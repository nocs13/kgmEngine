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

public:
  void  gcSet(u32 param, void* value) = 0;
  void  gcGet(u32 param, void* value) = 0;
  void  gcClear(u32 flag, u32 col, float depth, u32 sten) = 0;
  void  gcBegin() = 0;
  void  gcEnd() = 0;
  void  gcRender() = 0;
  void  gcSetTarget(void*  rt) = 0;

  // DRAWING
  void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt) = 0;

  // TEXTURE
  void* gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type) = 0;
  void  gcFreeTexture(void *t) = 0;
  void  gcSetTexture(u32 stage, void *t) = 0;

  // MATRIX
  void  gcSetMatrix(u32 mode, float* mtx) = 0;
  void  gcGetMatrix(u32 mode, float* mtx) = 0;
  void  gcSetViewport(int x, int y, int w, int h, float n, float f) = 0;

  //BLEND
  void  gcBlend(bool, u32, u32) = 0;

  //ALPHA
  void  gcAlpha(bool, u32, float) = 0;

  //CULL
  void  gcCull(u32 mode) = 0;

  //DEPTH
  void  gcDepth(bool en, bool mask, u32 mode) = 0;

  //LIGHT
  void gcSetLight(int i, float* pos, float forse, float* col, float* dir, float angle) = 0;

  //VERTEX & INDEX BUFFERS
  void* gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize) = 0;
  void  gcFreeVertexBuffer(void*) = 0;
  void  gcDrawVertexBuffer(void* buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff) = 0;

  // SHADER
  void* gcGenShader(const char*, const char*) = 0;
  void  gcFreeShader(void* s) = 0;
  void  gcSetShader(void* s) = 0;
  void  gcBindAttribute(void* s, int, const char*) = 0;
  void  gcUniform(void* s, u32, u32, const char*, void*) = 0;
  void  gcUniformMatrix(void* s, u32, u32, u32, const char*, void*) = 0;
  void  gcUniformSampler(void* s, const char*, void*) = 0;
#ifdef DEBUG
  void  gcGetUniform(void* s, const char*, void*) = 0;
#endif

private:
  static int  vkInit();
  static void vkFree();
};

#endif
