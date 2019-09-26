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
#else
    VkResult (VKAPI_PTR *vkCreateXlibSurfaceKHR)(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif
  };

  enum VK_RT
  {
    //VK_RT_POINT = 0,
    VK_RT_LINE = 0,
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

  struct Uniforms
  {
    mtx4   g_mView;
    mtx4   g_mProj;
    mtx4   g_mTran;
    vec4   g_vColor;
    vec4   g_vSpecular;
    vec4   g_vLight;
    vec4   g_vLightColor;
    vec4   g_vLightDirection;
    vec4   g_vClipPlane;
    vec3   g_vUp;
    vec3   g_vEye;
    vec3   g_vLook;
    float  g_fTime;
    float  g_fShine;
    float  g_fRandom;
    float  g_fAmbient;
    float  g_fLightPower;
    int    g_iClipping = 0;
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
    VkPipeline       pipeline;
    VkPipelineLayout layout;
    VkPipelineCache  cache;

    VkDescriptorSetLayout setlayout;
    VkDescriptorPool      setpool;
    VkDescriptorSet       descriptor;

    VkBool32 blending;
    VkBlendFactor bsrc;
    VkBlendFactor bdst;

    Shader*  shader;
    Uniforms ubo;

    VkPrimitiveTopology topology;
    u32 vertexFormat;
  };


  struct ActualPipelines
  {
    kgmArray<Pipeline*> pipelines;
    u32 count;

    void clear(kgmVulkan* vk)
    {
      for (u32 i = 0; i < count; i++)
        vk->freePipeline(pipelines[i]);

      pipelines.clear();
    }

    ActualPipelines()
    {
      pipelines.alloc(1024);
      count = 0;
    }

    Pipeline* get(Shader* sh, VkPrimitiveTopology tp, u32 vf, VkBool32 bl,
                  VkBlendFactor bs, VkBlendFactor bd)
    {
      for(u32 i = 0; i < count; i++)
      {
        Pipeline* ppl = pipelines[i];

        if (sh == pipelines[i]->shader && tp == pipelines[i]->topology && vf == pipelines[i]->vertexFormat)
        {
          if (pipelines[i]->blending == VK_TRUE)
          {
            if (bl == VK_TRUE && bs == pipelines[i]->bsrc && bd == pipelines[i]->bdst)
              return pipelines[i];
          }
          else if (bl == VK_FALSE)
            return pipelines[i];
        }
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
        pipelines.realloc(count + 256);

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

    Texture* texture;
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

        while(!gi.end())
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

      while(!i.end())
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

  kgmArray<VkExtensionProperties> m_extensionProperties;
  kgmArray<const s8*>             m_extensionNames;


  VkPhysicalDevice m_physicalDevice = 0;

  VkSwapchainKHR   m_swapChain      = 0;
  VkRenderPass     m_renderPass     = 0;
  VkCommandPool    m_commandPool    = 0;

  kgmArray<VkSemaphore> m_imageSemaphores;
  kgmArray<VkSemaphore> m_renderSemaphores;
  kgmArray<VkFence>     m_fences;

  kgmArray<VkPhysicalDevice> m_physicalDevices;
  kgmArray<VkImage>          m_swapChainImages;
  kgmArray<VkFramebuffer>    m_frameBuffers;
  kgmArray<VkImageView>      m_imageViews;
  kgmArray<VkCommandBuffer>  m_commandBuffers;

  VkSurfaceCapabilitiesKHR m_surfaceCapabilities;

  VkFormat m_swapChainFormat;

  VkFormat       m_depthFormat;
  VkImage        m_depthImage;
  VkImageView    m_depthImageView;
  VkDeviceMemory m_depthMemory;


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

  u32 m_currentFrame             = 0;
  u32 m_graphicsQueueFamilyCount =  0;
  u32 m_graphicsQueueFamilyIndex = -1;
  u32 m_presentQueueFamilyIndex  = -1;
  u32 m_vertexAttributes         = 1;
  u32 m_vertexStride             = 12;
  u32 m_vertexFormat             = gcv_xyz;

  Shader* m_shader = null;

  Texture* m_texture = null;

  ActualPipelines m_pipelines;

  s32 m_rect[4];
  f32 m_bgColor[4];
  f32 m_depth;
  u32 m_stensil;

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

  void gcResize(u32 width, u32 height);

  // DRAWING
  void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt);

  // TEXTURE
  void* gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type);
  void  gcFreeTexture(void *t);
  void  gcSetTexture(u32 stage, void *t);

  // VIEWPORT
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
  void* gcGenShader(kgmMemory<u8>&, kgmMemory<u8>&);
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
  bool initSwapchain();
  bool initRenderPass();
  bool initImageViews();
  bool initFrameBuffers();
  bool initDepthBuffer();
  bool initSemaphores();
  bool initPipeline();
  bool initSynchronizers();

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

  Pipeline* createPipeline();
  void freePipeline(Pipeline*);

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
