#pragma once

#include "../kgmBase/kgmIGC.h"

#include "kgmWindow.h"
#include "kgmLib.h"

#ifndef VULKAN
//#define VULKAN
#endif

#ifdef VULKAN

#ifdef WIN32

#ifndef _WIN32
#define _WIN32
#endif

#define VK_USE_PLATFORM_WIN32_KHR

#elif defined(WAYLAND)
  #define VK_USE_PLATFORM_WAYLAND_KHR
#else

#define VK_USE_PLATFORM_XLIB_KHR

#endif

#include "inc/vk/vulkan.h"

#if !defined(VK_EXPORTED_FUNCTION)
#define VK_EXPORTED_FUNCTION( fn ) PFN_##fn fn;
#endif

//struct VkXlibSurfaceCreateInfoKHR;

class kgmVulkan: public kgmObject, public kgmIGC
{
  KGM_OBJECT(kgmVulkan)

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
    PFN_vkFreeMemory vkFreeMemory;
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
    PFN_vkDestroyRenderPass vkDestroyRenderPass;
    PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
    PFN_vkDestroyImageView vkDestroyImageView;
    PFN_vkFreeCommandBuffers vkFreeCommandBuffers;

    VK_EXPORTED_FUNCTION(vkEnumerateInstanceLayerProperties);

    VK_EXPORTED_FUNCTION(vkGetPhysicalDeviceProperties);
    VK_EXPORTED_FUNCTION(vkGetPhysicalDeviceFeatures);

    VK_EXPORTED_FUNCTION(vkDestroySemaphore);
    VK_EXPORTED_FUNCTION(vkDestroyFence);

    VK_EXPORTED_FUNCTION(vkDestroySurfaceKHR);
    VK_EXPORTED_FUNCTION(vkDestroyImage);
    VK_EXPORTED_FUNCTION(vkCreateSampler);
    VK_EXPORTED_FUNCTION(vkDestroySampler);
    VK_EXPORTED_FUNCTION(vkMapMemory);
    VK_EXPORTED_FUNCTION(vkUnmapMemory);

    VK_EXPORTED_FUNCTION(vkCreateGraphicsPipelines);
    VK_EXPORTED_FUNCTION(vkDestroyPipeline);

    VK_EXPORTED_FUNCTION(vkCreatePipelineLayout);
    VK_EXPORTED_FUNCTION(vkDestroyPipelineLayout);

    VK_EXPORTED_FUNCTION(vkCreatePipelineCache);
    VK_EXPORTED_FUNCTION(vkDestroyPipelineCache);

    VK_EXPORTED_FUNCTION(vkCreateDescriptorSetLayout);
    VK_EXPORTED_FUNCTION(vkDestroyDescriptorSetLayout);
    VK_EXPORTED_FUNCTION(vkCreateDescriptorPool);
    VK_EXPORTED_FUNCTION(vkDestroyDescriptorPool);
    VK_EXPORTED_FUNCTION(vkAllocateDescriptorSets);
    VK_EXPORTED_FUNCTION(vkUpdateDescriptorSets);

    VK_EXPORTED_FUNCTION(vkCreateBuffer);
    VK_EXPORTED_FUNCTION(vkGetBufferMemoryRequirements);
    VK_EXPORTED_FUNCTION(vkAllocateMemory);
    VK_EXPORTED_FUNCTION(vkBindBufferMemory);
    VK_EXPORTED_FUNCTION(vkDestroyBuffer);

    VK_EXPORTED_FUNCTION(vkFlushMappedMemoryRanges);

    VK_EXPORTED_FUNCTION(vkCmdBindDescriptorSets);
    VK_EXPORTED_FUNCTION(vkCmdBindVertexBuffers);
    VK_EXPORTED_FUNCTION(vkCmdBindIndexBuffer);
    VK_EXPORTED_FUNCTION(vkCmdBindPipeline);
    VK_EXPORTED_FUNCTION(vkCmdSetViewport);
    VK_EXPORTED_FUNCTION(vkCmdSetScissor);
    VK_EXPORTED_FUNCTION(vkCmdSetBlendConstants);
    VK_EXPORTED_FUNCTION(vkCmdDraw);
    VK_EXPORTED_FUNCTION(vkCmdDrawIndexed);
    VK_EXPORTED_FUNCTION(vkCmdCopyBufferToImage);
    VK_EXPORTED_FUNCTION(vkCmdCopyBuffer);
    VK_EXPORTED_FUNCTION(vkCmdFillBuffer);
    VK_EXPORTED_FUNCTION(vkCmdUpdateBuffer);
    VK_EXPORTED_FUNCTION(vkCmdPushConstants);
    VK_EXPORTED_FUNCTION(vkCmdExecuteCommands);

    PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
    VK_EXPORTED_FUNCTION(vkDestroyDebugReportCallbackEXT);

#ifdef WIN32
    VkResult (VKAPI_PTR *vkCreateWin32SurfaceKHR)(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#elif defined(WAYLAND)
    VkResult (VKAPI_PTR *vkCreateWaylandSurfaceKHR)(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#else
    VkResult (VKAPI_PTR *vkCreateXlibSurfaceKHR)(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif
  };

#define VK_MAX_LIGHTS   8
#define VK_MAX_TEXTURES 4

  enum VK_RT
  {
    VK_RT_POINT = 0,
    VK_RT_LINE,
    VK_RT_LINESTRIP,
    VK_RT_TRIANGLE,
    VK_RT_TRIANGLESTRIP,
    VK_RT_END,
  };

  struct Texture
  {
    VkImage        image;
    VkImageView    iview;
    VkDeviceMemory memory;
    VkSampler      sampler;
  };

  struct RenderTarget
  {
    VkImage        image;
    VkImageView    imageView;
    VkDeviceMemory imageMemory;

    VkImage        depth;
    VkImageView    depthView;
    VkDeviceMemory depthMemory;

    VkSampler      sampler;

    VkFramebuffer  framebuffer;
    VkRenderPass   renderPass;

    u32 width, height;

    u32 rp_id;
  };

  struct VertexBuffer
  {
    VkDeviceMemory vmemory;
    VkBuffer       vbuffer;

    VkDeviceMemory imemory;
    VkBuffer       ibuffer;
  };

  struct Uniforms
  {
    mtx4   g_mView;
    mtx4   g_mProj;
    mtx4   g_mTran;
    vec4   g_vColor;
    vec4   g_vSpecular;
    vec4   g_vClipPlane;

    vec4   g_vLightPos[VK_MAX_LIGHTS];
    vec4   g_vLightDir[VK_MAX_LIGHTS];
    vec4   g_vLightCol[VK_MAX_LIGHTS];

    vec3   g_vUp;
    vec3   g_vEye;
    vec3   g_vLook;
    float  g_fTime;
    float  g_fShine;
    float  g_fRandom;
    float  g_fAmbient;
    float  g_fLightPower;
    int    g_iClipping;
  };

  struct PushConstants
  {
    mtx4 model;
    vec4 color;
    vec4 specular;
  };

  struct Shader
  {
    Uniforms         ubo;

    VkBuffer         buffer;
    VkDeviceMemory   memory;

    VkShaderModule   vertex;
    VkShaderModule   fragment;

    VkPipeline       pipelines[VK_RT_END];

    VkPipeline       pipeline;
    VkPipelineLayout layout;
    VkPipelineCache  cache;

    VkDescriptorSetLayout setlayout;
    VkDescriptorPool      setpool;
    VkDescriptorSet       descriptor;
  };

  struct Pipeline
  {
    Shader*  shader;
    Uniforms ubo;

    VkPipeline       pipeline;
    VkPipelineLayout layout;
    VkPipelineCache  cache;

    VkDescriptorSetLayout setlayout;
    VkDescriptorPool      setpool;
    VkDescriptorSet       descriptor;

    VkPrimitiveTopology topology;
    u32 vertexFormat;

    VkBool32 blending;
    VkBlendFactor bsrc;
    VkBlendFactor bdst;

    VkBool32       depthTest;
    VkBool32       depthWrite;
    VkCompareOp    depthCompare;

    VkCullModeFlags cullMode;
    VkFrontFace     cullFace;

    VkPolygonMode   polygonMode;

    VkBool32        stencil;
  };

  struct PipelineStatus
  {
    Shader*  shader;

    VkPrimitiveTopology topology;
    u32                 vertexFormat;

    VkBool32            blending;
    VkBlendFactor       srcBlend;
    VkBlendFactor       dstBlend;

    VkBool32            depthTest;
    VkBool32            depthWrite;
    VkCompareOp         depthCompare;

    VkCullModeFlags     cullMode;
    VkFrontFace         cullFace;

    VkPolygonMode       polygonMode;

    VkBool32            stencil;
  };

  struct ActualPipelines
  {
    kgmVulkan* vk;
    kgmArray<Pipeline*> pipelines;
    u32 count;

    ActualPipelines(kgmVulkan* v)
    {
      vk = v;

      init();
    }

    ~ActualPipelines()
    {
      clear();
    }

    void clear()
    {
      for (u32 i = 0; i < count; i++)
        vk->freePipeline(pipelines[i]);

      pipelines.clear();
      count = 0;
    }

    void init()
    {
      if (pipelines.length() < 1)
        pipelines.alloc(128);
      count = 0;
    }

    Pipeline* get(PipelineStatus* ps)
    {
      if (!ps)
        return null;

      for(u32 i = 0; i < count; i++)
      {
        Pipeline* ppl = pipelines[i];

        if (ppl->shader != ps->shader)
          continue;

        if (ppl->vertexFormat != ps->vertexFormat)
          continue;

        if (ppl->topology != ps->topology)
          continue;

        if (ppl->polygonMode != ps->polygonMode)
          continue;

        if (ppl->cullFace != ps->cullFace || ppl->cullMode != ps->cullMode)
          continue;

        if (ps->blending == VK_TRUE && ppl->blending != ps->blending)
        {
          continue;
        }
        else if (ps->blending == VK_TRUE && ppl->blending == ps->blending)
        {
          if (ppl->bsrc != ps->srcBlend || ppl->bdst != ps->dstBlend)
            continue;
        }

        if (ps->depthTest == VK_TRUE && ppl->depthTest != ps->depthTest)
        {
          continue;
        }
        else if (ps->depthTest == VK_TRUE && ppl->depthTest == ps->depthTest)
        {
          if (ppl->depthWrite != ps->depthWrite || ppl->depthCompare != ps->depthCompare)
            continue;
        }

        return ppl;
      }

      return null;
    }

    void add(Pipeline* p)
    {
      if (!p)
        return;

      for (u32 i = 0; i < count; i++)
      {
        if (pipelines[i] == p)
          return;
      }

      if (count >= pipelines.length())
        pipelines.realloc(count + 64);

      pipelines[count] = p;

      count++;
    }

    bool have(Pipeline* p)
    {
      for(u32 i = 0; i < count; i++)
        if (p == pipelines[i])
          return true;

      return false;
    }
  };

  struct Draw
  {
    VkDeviceMemory vmemory;
    VkBuffer       vbuffer;

    VkDeviceMemory imemory;
    VkBuffer       ibuffer;

    VkDeviceMemory umemory;
    VkBuffer       ubuffer;

    VkDescriptorBufferInfo descriptor;

    u32 vcnt, icnt, isize;
    VkIndexType itype;

    PushConstants constants;

    Texture* textures[VK_MAX_TEXTURES];

    bool vbo;
  };

  struct DrawGroup
  {
    Pipeline* pipeline;

    kgmList<Draw*> draws;

    ~DrawGroup()
    {
      draws.clear();
    }
  };

  struct DrawGroups
  {
    kgmList<DrawGroup*> groups;

    void add(Draw* d, Pipeline *p)
    {
      DrawGroup* g = null;

      if (!p)
        return;

      if (p)
      {
        auto gi = groups.begin();

        bool have = false;

        while(gi != groups.end())
        {
          if ((*gi)->pipeline == p)
          {
            have = true;

            break;
          }

          gi.next();
        }

        if (!have)
        {
          g = new DrawGroup();

          g->pipeline = p;

          groups.add(g);
        }
        else
        {
          g = (*gi);
        }
      }

      if (g && d)
        g->draws.add(d);
    }

    void clear()
    {
      kgmList<DrawGroup*>::iterator i = groups.begin();

      while(i != groups.end())
      {
        delete (*i);

        i.next();
      }

      groups.clear();
    }
  };

  static kgmLib vk_lib;
  static vk     m_vk;
  static u32    g_vulkans;

  kgmWindow* m_window = null;

  VkInstance   m_instance = VK_NULL_HANDLE;
  VkSurfaceKHR m_surface  = 0;
  VkDevice     m_device   = 0;

  s32 m_api_version_major = -1;
  s32 m_api_version_minor = -1;
  s32 m_api_version_patch = -1;

  VkPhysicalDeviceProperties m_deviceProperties;

  s32 m_uboSize = 0;

  kgmArray<VkExtensionProperties> m_extensionProperties;
  kgmArray<const s8*>             m_extensionNames;

  VkPhysicalDevice m_physicalDevice = 0;

  VkSwapchainKHR   m_swapChain      = 0;
  VkSwapchainKHR   m_oldSwapChain   = 0;
  VkRenderPass     m_renderPass     = 0;
  VkCommandPool    m_commandPool    = 0;
  VkQueue          m_queue          = 0;

  kgmArray<VkSemaphore> m_imageSemaphores;
  kgmArray<VkSemaphore> m_renderSemaphores;
  kgmArray<VkFence>     m_fences;

  VkFence               m_fence = VK_NULL_HANDLE;

  kgmArray<VkPhysicalDevice> m_physicalDevices;
  kgmArray<VkImage>          m_swapChainImages;
  kgmArray<VkFramebuffer>    m_frameBuffers;
  kgmArray<VkImageView>      m_imageViews;
  kgmArray<VkCommandBuffer>  m_commandBuffers;

  VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
  VkPresentModeKHR         m_presentMode = VK_PRESENT_MODE_FIFO_KHR;


  VkFormat       m_swapChainFormat;

  VkFormat       m_depthFormat;
  VkImage        m_depthImage     = VK_NULL_HANDLE;
  VkImageView    m_depthImageView = VK_NULL_HANDLE;
  VkDeviceMemory m_depthMemory    = VK_NULL_HANDLE;


  VkSemaphore m_imageAvailableSemaphore;
  VkSemaphore m_renderingFinishedSemaphore;

  VkViewport m_viewport;
  VkRect2D   m_scissor;

  VkQueue    m_graphicsQueue = VK_NULL_HANDLE;
  VkQueue    m_presentQueue  = VK_NULL_HANDLE;

  VkPrimitiveTopology m_topology;

  VkBlendFactor m_blendSource;
  VkBlendFactor m_blendDestination;
  VkBool32      m_blending = VK_FALSE;

  VkBool32       m_depthTest;
  VkBool32       m_depthWrite;
  VkCompareOp    m_depthCompare;

  VkCullModeFlags m_cullMode;
  VkFrontFace     m_cullFace;

  VkPolygonMode   m_polygonMode;

  VkBool32        m_stencil;

  u32 m_currentFrame             = 0;
  u32 m_graphicsQueueFamilyCount = 0;
  u32 m_graphicsQueueFamilyIndex = -1;
  u32 m_presentQueueFamilyIndex  = -1;
  u32 m_vertexAttributes         = 1;
  u32 m_vertexStride             = 12;
  u32 m_vertexFormat             = gcv_xyz;

  Shader* m_shader = null;

  Texture* m_textures[4] = { null };

  ActualPipelines* m_pipelines = null;
  VkPipelineCache* m_pipelineCache = null;

  s32 m_rect[4];
  f32 m_bgColor[4];
  f32 m_depth;
  u32 m_stensil;

  s32  m_sc_rect[4];
  bool m_sc_set;

  u32 m_clear;

  u32 m_error = 0;

  const s8* m_debugLayer = null;

  DrawGroups m_drawGroups;

  Uniforms         m_ubo;

#ifdef DEBUG
  VkDebugReportCallbackEXT m_debugReportCallback = VK_NULL_HANDLE;
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

  void  gcResize(u32 width, u32 height);

  // DRAWING
  void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt);

  // TEXTURE
  gchandle gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type);
  void  gcFreeTexture(gchandle t);
  void  gcSetTexture(u32 stage, gchandle t);

  // VIEWPORT
  void  gcSetViewport(int x, int y, int w, int h, float n, float f);

  //BLEND
  void  gcBlend(bool, u32, u32, u32);

  //CULL
  void  gcCull(u32 mode);

  //DEPTH
  void  gcDepth(bool en, bool mask, u32 mode);

  // SCISSOR
  void  gcScissor(bool on, int x, int y, int w, int h);

  //VERTEX & INDEX BUFFERS
  gchandle gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize);
  void  gcFreeVertexBuffer(gchandle buf);
  void  gcDrawVertexBuffer(gchandle buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff);

  // SHADER
  gchandle gcGenShader(kgmArray<u8>&, kgmArray<u8>&);
  void  gcFreeShader(gchandle s);
  void  gcSetShader(gchandle s);
  void  gcBindAttribute(gchandle s, int, const char*);
  void  gcUniform(gchandle s, u32, u32, const char*, void*);
  void  gcUniformMatrix(gchandle s, u32, u32, u32, const char*, void*);
  void  gcUniformSampler(gchandle s, const char*, void*);

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
  bool initSwapchain();
  bool initRenderPass();
  bool initImageViews();
  bool initFrameBuffers();
  bool initDepthBuffer();
  bool initSemaphores();
  bool initPipeline();
  bool initSynchronizers();

  bool isDeviceSuitable(VkPhysicalDevice);

  bool destroyDevice();
  bool destroySwapchain();

  bool refreshSwapchain();
  bool createBuffer(u32 size, VkBufferUsageFlags  usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& memory);
  u32  memoryTypeIndex(u32 type,  VkMemoryPropertyFlags properties);

  void* uniformLocation(Shader*, char*);

  void clear(Shader*);
  void clear(Draw*);
  void clearDraws();

  void fillCommands();

private:
  VkCommandBuffer beginSingleTimeCommand();
  void endSingleTimeCommand(VkCommandBuffer commandBuffer);
  bool transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
  void copyBufferToImageCube(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t bpp);

  VkFormat chooseDepthFormat();

  Pipeline* createPipeline();
  void freePipeline(Pipeline*);

  PipelineStatus getCurrentPipelineStatus();

  VkCompareOp vk_compare(u32 e)
  {
    switch(e)
    {
      case gccmp_always: return VK_COMPARE_OP_ALWAYS;
      case gccmp_never:	 return VK_COMPARE_OP_NEVER;
      case gccmp_equal:	 return VK_COMPARE_OP_EQUAL;
      case gccmp_nequal: return VK_COMPARE_OP_NOT_EQUAL;
      case gccmp_less:   return VK_COMPARE_OP_LESS;
      case gccmp_lequal: return VK_COMPARE_OP_LESS_OR_EQUAL;
      case gccmp_great:	 return VK_COMPARE_OP_GREATER;
      case gccmp_gequal: return VK_COMPARE_OP_GREATER_OR_EQUAL;
    }

    return VK_COMPARE_OP_NEVER;
  }

  VkBlendFactor vk_blend(u32 e)
  {
    switch(e)
    {
    case gcblend_zero:		  return VK_BLEND_FACTOR_ZERO;
    case gcblend_one:		    return VK_BLEND_FACTOR_ONE;
    case gcblend_dstcol:		return VK_BLEND_FACTOR_DST_COLOR;
    case gcblend_srccol:		return VK_BLEND_FACTOR_SRC_COLOR;
    case gcblend_dsticol:		return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case gcblend_srcicol:		return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case gcblend_dstalpha:	return VK_BLEND_FACTOR_DST_ALPHA;
    case gcblend_srcalpha:	return VK_BLEND_FACTOR_SRC_ALPHA;
    case gcblend_dstialpha:	return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case gcblend_srcialpha:	return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    }

    return VK_BLEND_FACTOR_ZERO;
  }

  VkPrimitiveTopology vk_primitive(u32 e)
  {
    switch(e)
    {
    case gcpmt_points:         return  VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case gcpmt_lines:          return  VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case gcpmt_triangles:      return  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case gcpmt_linestrip:      return  VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case gcpmt_trianglestrip:  return  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    }

    return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
  }

};

#endif
