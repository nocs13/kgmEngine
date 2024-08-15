#include "kgmVulkan.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmUtils/kgmBase64.h"
#include "kgmSystem.h"
#include "kgmThread.h"
#include "../kgmBase/kgmIResources.h"
#include "../kgmSystem/kgmTime.h"
#include "../kgmGame/kgmIGame.h"
#include "../kgmSystem/kgmApp.h"

#ifdef VULKAN

#include "inc/vk/vulkan.h"
#include "inc/vk/vk_kgm.h"

#define ZeroObject(o) memset(&o, 0, sizeof(typeof o))

#define SWAPCHAIN_IMAGES (2)
//#define VK_DYNAMIC_STATE_DEPTH_BOUNDS 5

//static int SWAPCHAIN_IMAGES = 2;


//https://gist.github.com/Overv/7ac07356037592a121225172d7d78f2d

u32           kgmVulkan::g_vulkans = 0;
kgmLib        kgmVulkan::vk_lib;
kgmVulkan::vk kgmVulkan::m_vk      = {0};

u32 rpass_cnt = 0;

kgmVulkan::kgmVulkan(kgmWindow* wnd)
{
  m_instance = 0;

  m_error = 0;

  if (wnd == nullptr)
    return;

  m_window = wnd;

  if (g_vulkans < 1)
  {
    if (vkInit() != 1 || m_vk.vkCreateInstance == nullptr)
    {
      m_error = 1;

      return;
    }
  }
  else
  {
    m_error = 0;

    return;
  }

  g_vulkans++;

  wnd->getRect(m_rect[0], m_rect[1], m_rect[2], m_rect[3]);

  m_viewport.x = 0;
  m_viewport.y = 0;
  m_viewport.width = m_rect[2];
  m_viewport.height = m_rect[3];
  m_viewport.minDepth = 0.0;
  m_viewport.maxDepth = 1.0;

  m_scissor.offset.x = 0;
  m_scissor.offset.y = 0;
  m_scissor.extent.width = m_rect[2];
  m_scissor.extent.height = m_rect[3];

  m_bgColor[0] = 1.0;
  m_bgColor[1] = 0.0;
  m_bgColor[2] = 0.0;
  m_bgColor[3] = 1.0;

  if (!initInstance())
  {
    m_error = 1;

    return;
  }

  initDebug();

  if (!listDevices() || !initDevice())
  {
    m_error = 1;

    return;
  }

  if (!initSurface() || !initSwapchain() || !initRenderPass())
  {
    m_error = 1;

    return;
  }

  if (!initImageViews() || !initDepthBuffer() || !initFrameBuffers())
  {
    m_error = 1;

    return;
  }

  if (!initSemaphores() || !initCommands() || !initSynchronizers())
  {
    m_error = 1;

    return;
  }

  m_error = 0;

  m_polygonMode = VK_POLYGON_MODE_FILL;
  m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  m_depthTest = VK_TRUE;
  m_depthWrite = VK_TRUE;
  m_depthCompare = VK_COMPARE_OP_LESS_OR_EQUAL;
  m_blending = VK_FALSE;
  m_cullMode = VK_CULL_MODE_NONE;
  //m_cullMode = VK_CULL_MODE_BACK_BIT;
  m_cullFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

  m_shader  = null;

  m_textures[0] = m_textures[1] =
  m_textures[2] = m_textures[3] = null;

  m_pipelines = new ActualPipelines(this);

  kgm_log() << "Vulkan: Successfully prepared.\n";
}

kgmVulkan::~kgmVulkan()
{
  uint64_t timeout = 3000000000;

  if (m_device)
  {
    kgm_log() << "Vulkan: API version major is " << VK_VERSION_MAJOR(m_deviceProperties.apiVersion) << ".\n";
    kgm_log() << "Vulkan: API version minor is " << VK_VERSION_MINOR(m_deviceProperties.apiVersion) << ".\n";
    kgm_log() << "Vulkan: API version patch is " << VK_VERSION_PATCH(m_deviceProperties.apiVersion) << ".\n";

    m_vk.vkDeviceWaitIdle(m_device);

    //m_vk.vkWaitForFences(m_device, SWAPCHAIN_IMAGES, m_fences, VK_TRUE, timeout);
    m_vk.vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, timeout);

    delete m_pipelines;

    for (size_t i = 0; i < SWAPCHAIN_IMAGES; i++)
    {
      m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
      m_vk.vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr);
    }

    if (m_depthImageView)
      m_vk.vkDestroyImageView(m_device, m_depthImageView, null);

    if (m_depthImage)
      m_vk.vkDestroyImage(m_device, m_depthImage, null);

    if (m_depthMemory)
      m_vk.vkFreeMemory(m_device, m_depthMemory, null);


    if (m_commandPool)
    {
      m_vk.vkFreeCommandBuffers(m_device, m_commandPool, (u32) m_commandBuffers.length(), m_commandBuffers.data());

      m_commandBuffers.clear();

      kgm_log() << "Vulkan: Command buffer destroyed.\n";

      m_vk.vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    }

    if (m_renderPass)
    {
      m_vk.vkDestroyRenderPass(m_device, m_renderPass, nullptr);

      m_renderPass = VK_NULL_HANDLE;
    }

    kgm_log() << "Vulkan: Render pass destroyed.\n";

    for (size_t i = 0; i < SWAPCHAIN_IMAGES; i++)
    {
      m_vk.vkDestroySemaphore(m_device, m_renderSemaphores[i], nullptr);
      m_vk.vkDestroySemaphore(m_device, m_imageSemaphores[i], nullptr);
      m_vk.vkDestroyFence(m_device, m_fences[i], nullptr);
    }

    m_vk.vkDestroyFence(m_device, m_fence, nullptr);
    m_vk.vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);
    m_vk.vkDestroySemaphore(m_device, m_renderingFinishedSemaphore, nullptr);

    m_swapChainImages.clear();
    m_imageViews.clear();
    m_frameBuffers.clear();
    m_fences.clear();
    m_imageSemaphores.clear();
    m_renderSemaphores.clear();


    if (m_swapChain != VK_NULL_HANDLE)
    {
      m_vk.vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);

      kgm_log() << "Vulkan: SwapChain destroyed.\n";
    }

    m_vk.vkDestroyDevice(m_device, nullptr);

    kgm_log() << "Vulkan: Device destroyed.\n";
  }

  if (m_instance)
  {
    if (m_surface)
      m_vk.vkDestroySurfaceKHR(m_instance, m_surface, null);

#ifdef DEBUG
    m_vk.vkDestroyDebugReportCallbackEXT = (typeof m_vk.vkDestroyDebugReportCallbackEXT) m_vk.vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");;

    if (m_vk.vkDestroyDebugReportCallbackEXT)
      m_vk.vkDestroyDebugReportCallbackEXT(m_instance, m_debugReportCallback, nullptr);
#endif

    m_vk.vkDestroyInstance(m_instance, null);
  }

  g_vulkans--;

  if (g_vulkans < 1)
    vkFree();
}

#if !defined(VK_IMPORT_FUNCTION)
#define VK_IMPORT_FUNCTION( fn ) m_vk.fn = (typeof m_vk.fn) vk_lib.get((s8*) #fn)
#endif

int kgmVulkan::vkInit()
{
  if (vk_lib.active())
    return -1;

#ifdef WIN32
  if (!vk_lib.open((char*) "vulkan.dll"))
    if (!vk_lib.open((char*) "vulkan-1.dll"))
      return -2;
#else
  if (!vk_lib.open((char*) "libvulkan.so"))
    if (!vk_lib.open((char*) "libvulkan.so.1"))
      return -2;
#endif

  #pragma GCC diagnostic push
  //#pragma GCC diagnostic ignored "-fpermissive"

  m_vk.vkGetInstanceProcAddr = (typeof m_vk.vkGetInstanceProcAddr) vk_lib.get((char*) "vkGetInstanceProcAddr");
  m_vk.vkCreateInstance = (typeof m_vk.vkCreateInstance) vk_lib.get((char*) "vkCreateInstance");
  m_vk.vkDestroyInstance = (typeof m_vk.vkDestroyInstance) vk_lib.get((char*) "vkDestroyInstance");
  m_vk.vkEnumerateInstanceExtensionProperties = (typeof m_vk.vkEnumerateInstanceExtensionProperties) vk_lib.get((char*) "vkEnumerateInstanceExtensionProperties");
  m_vk.vkEnumeratePhysicalDevices = (typeof m_vk.vkEnumeratePhysicalDevices) vk_lib.get((char*) "vkEnumeratePhysicalDevices");
  m_vk.vkEnumerateDeviceExtensionProperties = (typeof m_vk.vkEnumerateDeviceExtensionProperties) vk_lib.get((char*) "vkEnumerateDeviceExtensionProperties");
  m_vk.vkGetPhysicalDeviceFormatProperties = (typeof m_vk.vkGetPhysicalDeviceFormatProperties) vk_lib.get((char*) "vkGetPhysicalDeviceFormatProperties");
  m_vk.vkGetPhysicalDeviceQueueFamilyProperties = (typeof m_vk.vkGetPhysicalDeviceQueueFamilyProperties) vk_lib.get((char*) "vkGetPhysicalDeviceQueueFamilyProperties");
  m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
  m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceFormatsKHR");
  m_vk.vkGetPhysicalDeviceSurfaceSupportKHR = (typeof m_vk.vkGetPhysicalDeviceSurfaceSupportKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfaceSupportKHR");
  m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR = (typeof m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR) vk_lib.get((char*) "vkGetPhysicalDeviceSurfacePresentModesKHR");
  m_vk.vkGetImageMemoryRequirements = (typeof m_vk.vkGetImageMemoryRequirements) vk_lib.get((char*) "vkGetImageMemoryRequirements");
  m_vk.vkCreateDevice = (typeof m_vk.vkCreateDevice) vk_lib.get((char*) "vkCreateDevice");
  m_vk.vkDestroyDevice = (typeof m_vk.vkDestroyDevice) vk_lib.get((char*) "vkDestroyDevice");
  m_vk.vkGetDeviceQueue = (typeof m_vk.vkGetDeviceQueue) vk_lib.get((char*) "vkGetDeviceQueue");
  m_vk.vkQueueWaitIdle = (typeof m_vk.vkQueueWaitIdle) vk_lib.get((char*) "vkQueueWaitIdle");
  m_vk.vkGetPhysicalDeviceMemoryProperties = (typeof m_vk.vkGetPhysicalDeviceMemoryProperties) vk_lib.get((char*) "vkGetPhysicalDeviceMemoryProperties");
  m_vk.vkCreateSwapchainKHR = (typeof m_vk.vkCreateSwapchainKHR) vk_lib.get((char*) "vkCreateSwapchainKHR");
  m_vk.vkDestroySwapchainKHR = (typeof m_vk.vkDestroySwapchainKHR) vk_lib.get((char*) "vkDestroySwapchainKHR");
  m_vk.vkGetSwapchainImagesKHR = (typeof m_vk.vkGetSwapchainImagesKHR) vk_lib.get((char*) "vkGetSwapchainImagesKHR");
  m_vk.vkCreateSemaphore = (typeof m_vk.vkCreateSemaphore) vk_lib.get((char*) "vkCreateSemaphore");
  m_vk.vkCreateCommandPool = (typeof m_vk.vkCreateCommandPool) vk_lib.get((char*) "vkCreateCommandPool");
  m_vk.vkResetCommandPool = (typeof m_vk.vkResetCommandPool) vk_lib.get((char*) "vkResetCommandPool");
  m_vk.vkDestroyCommandPool = (typeof m_vk.vkDestroyCommandPool) vk_lib.get((char*) "vkDestroyCommandPool");
  m_vk.vkCreateRenderPass = (typeof m_vk.vkCreateRenderPass) vk_lib.get((char*) "vkCreateRenderPass");
  m_vk.vkCmdBeginRenderPass = (typeof m_vk.vkCmdBeginRenderPass) vk_lib.get((char*) "vkCmdBeginRenderPass");
  m_vk.vkCmdEndRenderPass = (typeof m_vk.vkCmdEndRenderPass) vk_lib.get((char*) "vkCmdEndRenderPass");
  m_vk.vkCreateShaderModule = (typeof m_vk.vkCreateShaderModule) vk_lib.get((char*) "vkCreateShaderModule");
  m_vk.vkDestroyShaderModule = (typeof m_vk.vkDestroyShaderModule) vk_lib.get((char*) "vkDestroyShaderModule");
  m_vk.vkCreateImage = (typeof m_vk.vkCreateImage) vk_lib.get((char*) "vkCreateImage");
  m_vk.vkCreateImageView = (typeof m_vk.vkCreateImageView) vk_lib.get((char*) "vkCreateImageView");
  m_vk.vkCreateFramebuffer = (typeof m_vk.vkCreateFramebuffer) vk_lib.get((char*) "vkCreateFramebuffer");
  m_vk.vkAllocateMemory = (typeof m_vk.vkAllocateMemory) vk_lib.get((char*) "vkAllocateMemory");
  m_vk.vkFreeMemory = (typeof m_vk.vkFreeMemory) vk_lib.get((char*) "vkFreeMemory");
  m_vk.vkBindImageMemory = (typeof m_vk.vkBindImageMemory) vk_lib.get((char*) "vkBindImageMemory");
  m_vk.vkAllocateCommandBuffers = (typeof m_vk.vkAllocateCommandBuffers) vk_lib.get((char*) "vkAllocateCommandBuffers");
  m_vk.vkBeginCommandBuffer = (typeof m_vk.vkBeginCommandBuffer) vk_lib.get((char*) "vkBeginCommandBuffer");
  m_vk.vkResetCommandBuffer = (typeof m_vk.vkResetCommandBuffer) vk_lib.get((char*) "vkResetCommandBuffer");
  m_vk.vkEndCommandBuffer = (typeof m_vk.vkEndCommandBuffer) vk_lib.get((char*) "vkEndCommandBuffer");
  m_vk.vkCmdPipelineBarrier = (typeof m_vk.vkCmdPipelineBarrier) vk_lib.get((char*) "vkCmdPipelineBarrier");
  m_vk.vkCreateFence = (typeof m_vk.vkCreateFence) vk_lib.get((char*) "vkCreateFence");
  m_vk.vkResetFences = (typeof m_vk.vkResetFences) vk_lib.get((char*) "vkResetFences");
  m_vk.vkWaitForFences = (typeof m_vk.vkWaitForFences) vk_lib.get((char*) "vkWaitForFences");
  m_vk.vkDeviceWaitIdle = (typeof m_vk.vkDeviceWaitIdle) vk_lib.get((char*) "vkDeviceWaitIdle");
  m_vk.vkAcquireNextImageKHR = (typeof m_vk.vkAcquireNextImageKHR) vk_lib.get((char*) "vkAcquireNextImageKHR");
  m_vk.vkQueueSubmit = (typeof m_vk.vkQueueSubmit) vk_lib.get((char*) "vkQueueSubmit");
  m_vk.vkQueuePresentKHR = (typeof m_vk.vkQueuePresentKHR) vk_lib.get((char*) "vkQueuePresentKHR");
  m_vk.vkDestroyPipeline = (typeof m_vk.vkDestroyPipeline) vk_lib.get((char*) "vkDestroyPipeline");
  m_vk.vkDestroyRenderPass = (typeof m_vk.vkDestroyRenderPass) vk_lib.get((char*) "vkDestroyRenderPass");
  m_vk.vkDestroyFramebuffer = (typeof m_vk.vkDestroyFramebuffer) vk_lib.get((char*) "vkDestroyFramebuffer");
  m_vk.vkDestroyImageView = (typeof m_vk.vkDestroyImageView) vk_lib.get((char*) "vkDestroyImageView");
  m_vk.vkDestroyDescriptorSetLayout = (typeof m_vk.vkDestroyDescriptorSetLayout) vk_lib.get((char*) "vkDestroyDescriptorSetLayout");
  m_vk.vkFreeCommandBuffers = (typeof m_vk.vkFreeCommandBuffers) vk_lib.get((char*) "vkFreeCommandBuffers");

  VK_IMPORT_FUNCTION(vkEnumerateInstanceLayerProperties);

  VK_IMPORT_FUNCTION(vkGetPhysicalDeviceProperties);
  VK_IMPORT_FUNCTION(vkGetPhysicalDeviceFeatures);

  VK_IMPORT_FUNCTION(vkDestroySemaphore);
  VK_IMPORT_FUNCTION(vkDestroyFence);
  VK_IMPORT_FUNCTION(vkDestroySurfaceKHR);
  VK_IMPORT_FUNCTION(vkDestroyImage);
  VK_IMPORT_FUNCTION(vkCreateSampler);
  VK_IMPORT_FUNCTION(vkDestroySampler);
  VK_IMPORT_FUNCTION(vkMapMemory);
  VK_IMPORT_FUNCTION(vkUnmapMemory);
  VK_IMPORT_FUNCTION(vkCreateGraphicsPipelines);
  VK_IMPORT_FUNCTION(vkDestroyPipeline);
  VK_IMPORT_FUNCTION(vkCreatePipelineLayout);
  VK_IMPORT_FUNCTION(vkDestroyPipelineLayout);
  VK_IMPORT_FUNCTION(vkCreatePipelineCache);
  VK_IMPORT_FUNCTION(vkDestroyPipelineCache);
  VK_IMPORT_FUNCTION(vkCreateDescriptorSetLayout);
  VK_IMPORT_FUNCTION(vkDestroyDescriptorSetLayout);
  VK_IMPORT_FUNCTION(vkCreateDescriptorPool);
  VK_IMPORT_FUNCTION(vkDestroyDescriptorPool);
  VK_IMPORT_FUNCTION(vkAllocateDescriptorSets);
  VK_IMPORT_FUNCTION(vkUpdateDescriptorSets);
  VK_IMPORT_FUNCTION(vkCreateBuffer);
  VK_IMPORT_FUNCTION(vkGetBufferMemoryRequirements);
  VK_IMPORT_FUNCTION(vkAllocateMemory);
  VK_IMPORT_FUNCTION(vkBindBufferMemory);
  VK_IMPORT_FUNCTION(vkDestroyBuffer);
  VK_IMPORT_FUNCTION(vkFlushMappedMemoryRanges);
  VK_IMPORT_FUNCTION(vkCmdBindDescriptorSets);
  VK_IMPORT_FUNCTION(vkCmdBindVertexBuffers);
  VK_IMPORT_FUNCTION(vkCmdBindIndexBuffer);
  VK_IMPORT_FUNCTION(vkCmdBindPipeline);
  VK_IMPORT_FUNCTION(vkCmdSetViewport);
  VK_IMPORT_FUNCTION(vkCmdSetScissor);
  VK_IMPORT_FUNCTION(vkCmdSetBlendConstants);
  VK_IMPORT_FUNCTION(vkCmdDraw);
  VK_IMPORT_FUNCTION(vkCmdDrawIndexed);
  VK_IMPORT_FUNCTION(vkCmdCopyBuffer);
  VK_IMPORT_FUNCTION(vkCmdCopyBufferToImage);
  VK_IMPORT_FUNCTION(vkCmdFillBuffer);
  VK_IMPORT_FUNCTION(vkCmdUpdateBuffer);
  VK_IMPORT_FUNCTION(vkCmdPushConstants);
  VK_IMPORT_FUNCTION(vkCmdExecuteCommands);

  //VK_IMPORT_FUNCTION(vkGetPhysicalDeviceMemoryProperties);

#ifdef WIN32
  m_vk.vkCreateWin32SurfaceKHR = (typeof m_vk.vkCreateWin32SurfaceKHR) vk_lib.get((char*) "vkCreateWin32SurfaceKHR");
#elif defined(WAYLAND)
  m_vk.vkCreateWaylandSurfaceKHR = (typeof m_vk.vkCreateWaylandSurfaceKHR) vk_lib.get((char*) "vkCreateWaylandSurfaceKHR");
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

u32 kgmVulkan::gcError()
{

  return m_error;
}

void  kgmVulkan::gcSet(u32 param, void* value)
{
  if (param == gcpar_vertype)
  {
    u32 size = 0, attr = 0;
    size_t vert = (size_t) value;

    if (vert & gcv_xyz) {
      size += (sizeof(float) * 3);
      attr++;
    }
    if (vert & gcv_nor){
      size += (sizeof(float) * 3);
      attr++;
    }
    if (vert & gcv_uv0){
      size += (sizeof(float) * 2);
      attr++;
    }
    if (vert & gcv_uv1){
      size += (sizeof(float) * 2);
      attr++;
    }
    if (vert & gcv_col){
      size += (sizeof(float) * 4);
      attr++;
    }

    if (size > 0)
    {
      m_vertexAttributes = attr;
      m_vertexStride     = size;
      m_vertexFormat     = vert;
    }
  }
  else if(param == gcpar_polymode)
  {
    u32 mode = (size_t) value;

    switch (mode)
    {
    case gcpmt_points:
      m_polygonMode = VK_POLYGON_MODE_POINT;
      break;
    case gcpmt_lines:
      m_polygonMode = VK_POLYGON_MODE_LINE;
      break;
    case gcpmt_triangles:
    default:
      m_polygonMode = VK_POLYGON_MODE_FILL;
    }
  }
}

void  kgmVulkan::gcGet(u32 param, void* value)
{

}

void  kgmVulkan::gcClear(u32 flag, u32 col, float depth, u32 sten)
{
  u8 r = (col & 0x000000ff);
  u8 g = ((col & 0x0000ff00) >> 8);
  u8 b = ((col & 0x00ff0000) >> 16);
  u8 a = ((col & 0xff000000) >> 24);

  m_bgColor[0] = (f32) r / 255.f;
  m_bgColor[1] = (f32) g / 255.f;
  m_bgColor[2] = (f32) b / 255.f;
  m_bgColor[3] = (f32) a / 255.f;

  m_bgColor[0] = .1;
  m_bgColor[1] = .1;
  m_bgColor[2] = .1;
  m_bgColor[3] = 1.0;

  m_depth   = depth;
  m_stensil = sten;
  m_clear   = flag;
}

void kgmVulkan::gcResize(u32 width, u32 height)
{
  if (width == 0 || height == 0)
    return;

  if (width == m_rect[2] && height == m_rect[3])
    return;

  m_rect[2] = width;
  m_rect[3] = height;

  kgm_log() << "Vulkan: Window resized, viewport need update.\n";

  refreshSwapchain();
}

void  kgmVulkan::gcBegin()
{

}

void  kgmVulkan::gcEnd()
{

}

/*
void  kgmVulkan::gcRender()
{
  VkResult result = VK_SUCCESS;

  //kgm_log() << "Vulkan info: start render.\n";

  fillCommands();

  u32 swapChainImage = 0;

  if (!m_device || !m_swapChain)
    return;

  //VkFence *fence = &m_fences[m_currentFrame];
  VkFence *fence = &m_fence; //&m_fences[(m_currentFrame == 1) ? (0) : (1)];

  uint64_t timeout = 3000000000; //UINT64_MAX

  m_vk.vkResetFences(m_device, 1, fence);

  result = m_vk.vkAcquireNextImageKHR(m_device, m_swapChain, timeout, VK_NULL_HANDLE, *fence, &swapChainImage);
  //result = m_vk.vkAcquireNextImageKHR(m_device, m_swapChain, timeout, VK_NULL_HANDLE, VK_NULL_HANDLE, &swapChainImage);

  //fence = &m_fences[swapChainImage];

  if(result == VkResult::VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
  {
    result = m_vk.vkWaitForFences(m_device, 1, fence, VK_TRUE, timeout);

    if (result == VkResult::VK_SUCCESS || result == VkResult::VK_TIMEOUT)
    {

    }
    else
    {
      kgm_log() << "Vulkan render info: Cannot wait fence.\n";

      printResult(result);

      kgmApp::exit(1);

      return;
    }
  }
  else
  {
    kgm_log() << "Vulkan render info: Cannot switch to next image.\n";

    printResult(result);

    kgmApp::exit(1);

    return;
  }

  m_vk.vkResetFences(m_device, 1, fence);

  auto commandBuffer = m_commandBuffers[swapChainImage];

  VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  //VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  //VkSemaphore waitSemaphores[] = { m_imageSemaphores[m_currentFrame] };
  //VkSemaphore signalSemaphores[] = { m_renderSemaphores[m_currentFrame] };

  VkSubmitInfo submitInfo;

  ZeroObject(submitInfo);

  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = nullptr;
  submitInfo.waitSemaphoreCount = 0;
  submitInfo.pWaitSemaphores = nullptr;
  submitInfo.pWaitDstStageMask = &waitMask;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.pSignalSemaphores = nullptr;

  result = m_vk.vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, *fence);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to submit command buffer.\n";

    printResult(result);

    kgmApp::exit(1);

    return;
  }

  result = m_vk.vkQueueWaitIdle(m_graphicsQueue);
  //result = m_vk.vkWaitForFences(m_device, 1, fence, VK_TRUE, timeout);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to wait for queue.\n";

    printResult(result);

    kgmApp::exit(1);
    //destroySwapchain();
    //destroyDevice();
    //initDevice();
    //refreshSwapchain();

    return;
  }

  VkResult resultPresent = VK_SUCCESS;

  VkPresentInfoKHR presentInfo;

  ZeroObject(presentInfo);

  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;
  presentInfo.waitSemaphoreCount = 0;
  presentInfo.pWaitSemaphores = nullptr;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &m_swapChain;
  presentInfo.pImageIndices = &swapChainImage;
  presentInfo.pResults = &result;

  result = m_vk.vkQueuePresentKHR(m_presentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR  || result == VK_SUBOPTIMAL_KHR)
  {
    kgm_log() << "Vulkan render info: Surface incompatible, updating swapchans.\n";

    refreshSwapchain();

    return;
  }
  else if(resultPresent != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: Failed to present swapchain.\n";

    printResult(result);

    kgmApp::exit(1);
  }

  m_currentFrame = swapChainImage;
}
*/

void  kgmVulkan::gcRender()
{
  VkResult result;

  fillCommands();

  u32 swapChainImage = 0;

  if (!m_device || !m_swapChain)
    return;

  //m_vk.vkWaitForFences(m_device, 1, &m_fences[m_currentFrame], VK_TRUE, UINT64_MAX);

  result = m_vk.vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, m_imageSemaphores[m_currentFrame], VK_NULL_HANDLE, &swapChainImage);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    kgm_log() << "Vulkan render info: Surface incompatible, updating swapchans.\n";

    refreshSwapchain();

    return;
  }
  else if(result != VkResult::VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    kgm_log() << "Vulkan render error: failed to get next swapchain image.\n";

    printResult(result);

    return;
  }

  result = m_vk.vkResetFences(m_device, 1, &m_fences[m_currentFrame]);

  if (result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: Failed to reset fance.\n";

    printResult(result);

    return;
  }

  auto commandBuffer = m_commandBuffers[swapChainImage];

  //VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  VkSemaphore waitSemaphores[] = { m_imageSemaphores[m_currentFrame] };
  VkSemaphore signalSemaphores[] = { m_renderSemaphores[m_currentFrame] };

  VkSubmitInfo submitInfo;

  ZeroObject(submitInfo);

  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = nullptr;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = &waitMask;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  result = m_vk.vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

  VkResult resultPresent = VK_SUCCESS;

  VkPresentInfoKHR presentInfo;

  ZeroObject(presentInfo);

  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &m_swapChain;
  presentInfo.pImageIndices = &swapChainImage;
  presentInfo.pResults = &result;

  result = m_vk.vkQueuePresentKHR(m_presentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR  || result == VK_SUBOPTIMAL_KHR)
  {
    kgm_log() << "Vulkan render info: Surface incompatible, updating swapchans.\n";

    refreshSwapchain();
  }

  if(resultPresent != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: Failed to present swapchain.\n";

    printResult(result);
  }

  //kgm_log() << "Vulkan: Queue present passed.\n";

  m_vk.vkQueueWaitIdle(m_graphicsQueue);
  m_vk.vkQueueWaitIdle(m_presentQueue);

  clearDraws();

  m_currentFrame = (m_currentFrame + 1) % SWAPCHAIN_IMAGES;
}

void  kgmVulkan::gcSetTarget(void*  rt)
{

}

// DRAWING
void  kgmVulkan::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt)
{
  if (!m_shader)
    return;

  if (pmt != gcpmt_trianglestrip && pmt != gcpmt_lines &&
      pmt != gcpmt_triangles && !gcpmt_linestrip)
    return;

  Draw* mesh = new Draw();

  ZeroObject(*mesh);

  u32 size = v_size * v_cnt;

  if (!createBuffer(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                    mesh->vbuffer, mesh->vmemory))
  {
    kgm_log() << "Vulkan error: Failed to prepare vertex buffer and memory!\n";

    return;
  }

  void* data;

  if (m_vk.vkMapMemory(m_device, mesh->vmemory, 0, size, 0, &data) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to map mamory!\n";

    clear(mesh);

    return;
  }

  memcpy(data, v_pnt, (size_t) size);

  m_vk.vkUnmapMemory(m_device, mesh->vmemory);

  if (i_cnt && i_pnt)
  {
     VkDeviceSize bufferSize = i_size * i_cnt;

     if (!createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                       mesh->ibuffer, mesh->imemory) != VK_SUCCESS)
     {
       kgm_log() << "Vulkan error: Failed create staging buffer!\n";

       clear(mesh);

       return;
     }

     void* data;
     m_vk.vkMapMemory(m_device, mesh->imemory, 0, bufferSize, 0, &data);
     memcpy(data, i_pnt, (size_t) bufferSize);
     m_vk.vkUnmapMemory(m_device, mesh->imemory);

     mesh->isize = bufferSize;
     mesh->icnt = i_cnt;

     if (i_size == 2)
       mesh->itype = VK_INDEX_TYPE_UINT16;
     else
       mesh->itype = VK_INDEX_TYPE_UINT32;
  }

  mesh->constants.model    = m_shader->ubo.g_mTran;
  mesh->constants.color    = m_shader->ubo.g_vColor;
  mesh->constants.specular = m_shader->ubo.g_vSpecular;

  for (u32 i = 0; i < VK_MAX_TEXTURES; i++)
  {
    if (m_textures[i] != null)
      mesh->textures[i] = m_textures[i];
    else
      break;
  }

  /*
  if (!createBuffer(sizeof(Shader::ubo), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    mesh->ubuffer, mesh->umemory))
  {
    kgm_log() << "Vulkan error: Cannot create memory buffer for shader.\n";
  }
  */

  switch(pmt)
  {
  case gcpmt_triangles:
    m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    break;
  case gcpmt_trianglestrip:
    m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    break;
  case gcpmt_lines:
    m_topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    break;
  case gcpmt_linestrip:
    m_topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    break;
  case gcpmt_points:
    m_topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    break;
  default:
    m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  };

  {
    u32 attr = 0;

    if (v_fmt & gcv_xyz) {
      attr++;
    }
    if (v_fmt & gcv_nor){
      attr++;
    }
    if (v_fmt & gcv_uv0){
      attr++;
    }
    if (v_fmt & gcv_uv1){
      attr++;
    }
    if (v_fmt & gcv_col){
      attr++;
    }

    m_vertexAttributes = attr;
  }

  m_vertexFormat = v_fmt;
  m_vertexStride = v_size;

  mesh->vcnt = v_cnt;
  mesh->icnt = i_cnt;


  PipelineStatus ps = getCurrentPipelineStatus();

  Pipeline* pipeline = m_pipelines->get(&ps);

  if (!pipeline)
  {
    pipeline = createPipeline();

    if (!pipeline)
      return;

    m_pipelines->add(pipeline);
  }

  m_drawGroups.add(mesh, pipeline);
}

// TEXTURE
void* kgmVulkan::gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type)
{
  if (!m_device || !w || !h)
    return null;

  kgm_log() << "Vulkan: Generating texture.\n";

  u32 bypp = 1;

  VkFormat format;

  VkImageType itype;

  switch(bpp)
  {
  case gctex_fmt8:
    bypp = 1;
    break;
  case gctex_fmt16:
    bypp = 2;
    break;
  case gctex_fmt24:
    bypp = 3;
    break;
  case gctex_fmt32:
    bypp = 4;
    break;
  case gctex_fmtdepth:
    bypp = 3;
    break;
  case gctex_fmtdepcil:
    bypp = 4;
    break;
  default:
    bypp = 4;
  };

  format = VK_FORMAT_R8G8B8A8_UNORM;

  switch (type)
  {
  case gctype_texcube:
  case gctype_texdepth:
  case gctype_tex2d:
  default:
    itype = VK_IMAGE_TYPE_2D;
    break;
  }

  VkBuffer       sbuffer = VK_NULL_HANDLE;
  VkDeviceMemory smemory = VK_NULL_HANDLE;

  void* data = null;

  u32 count = w * h;

  u32 size  = count * 4;

  if (type == gctype_texcube)
    size * 6;

  if (!createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    sbuffer, smemory))
  {
    kgm_log() << "Vulkan error: Failed create image stage buffer.\n";

    return null;
  }

  VkResult result = m_vk.vkMapMemory(m_device, smemory, 0, VK_WHOLE_SIZE, 0, &data);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to fill texture image memory.\n";

    printResult(result);

    m_vk.vkDestroyBuffer(m_device, sbuffer, null);

    m_vk.vkFreeMemory(m_device, smemory, null);

    return null;
  }

  if (m)
  {
    if (bypp == 4)
    {
      memcpy(data, m, size);
    }
    else if (bypp == 3)
    {
      u8* src = (u8*) m;
      u8* dst = (u8*) data;

      for (u32 i = 0; i < count; i++)
      {
        memcpy(dst, src, 3);
        dst[3] = 0xff;

        src += 3;
        dst += 4;
      }
    }
    else if (bypp == 2)
    {
      u16 red_mask   = 0xF800;
      u16 green_mask = 0x7E0;
      u16 blue_mask  = 0x1F;

      u8* src = (u8*) m;
      u8* dst = (u8*) data;

      for (u8 i = 0; i < count; i++)
      {
        u16 rgb16;

        memcpy(&rgb16, src, 2);

        dst[0] = (rgb16 & red_mask)   >> 11;
        dst[1] = (rgb16 & green_mask) >> 5;
        dst[2] = (rgb16 & blue_mask)  >> 0;
        dst[3] = 0xff;

        src += 2;
        dst += 4;
      }
    }
    else
    {
      memset(data, 0xff, size);
    }
  }
  else
  {
    memset(data, 0xff, size);
  }

  m_vk.vkUnmapMemory(m_device, smemory);

  Texture* t = new Texture;

  t->image = VK_NULL_HANDLE;
  t->iview = VK_NULL_HANDLE;
  t->memory = VK_NULL_HANDLE;

  VkImageCreateInfo imageInfo = {};

  ZeroObject(imageInfo);

  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = itype;
  imageInfo.extent.width = w;
  imageInfo.extent.height = h;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;

  if(type == gctype_texcube)
    imageInfo.arrayLayers = 6;
  else
    imageInfo.arrayLayers = 1;

  imageInfo.format = format;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if(type == gctype_texcube)
    imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

  result = m_vk.vkCreateImage(m_device, &imageInfo, nullptr, &t->image);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create texture image.\n";

    printResult(result);

    m_vk.vkDestroyBuffer(m_device, sbuffer, null);
    m_vk.vkFreeMemory(m_device, smemory, null);

    delete t;

    return null;
  }

  VkMemoryRequirements requirements;

  m_vk.vkGetImageMemoryRequirements(m_device, t->image, &requirements);

  VkMemoryAllocateInfo allocInfo;

  ZeroObject(allocInfo);
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = requirements.size;
  allocInfo.memoryTypeIndex = memoryTypeIndex(requirements.memoryTypeBits,
                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  result = m_vk.vkAllocateMemory(m_device, &allocInfo, null, &t->memory);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to allocate texture image memory.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, t->image, null);

    m_vk.vkDestroyBuffer(m_device, sbuffer, null);
    m_vk.vkFreeMemory(m_device, smemory, null);

    delete t;

    return null;
  }

  result = m_vk.vkBindImageMemory(m_device, t->image, t->memory, null);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to bind texture image memory.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, t->image, null);

    m_vk.vkFreeMemory(m_device, t->memory, null);

    m_vk.vkDestroyBuffer(m_device, sbuffer, null);
    m_vk.vkFreeMemory(m_device, smemory, null);

    delete t;

    return null;
  }

  kgm_log() << "Vulkan: Bind image memory to buffer.\n";

  transitionImageLayout(t->image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

  if (type == gctype_texcube)
    copyBufferToImageCube(sbuffer, t->image, w, h, bypp);
  else
    copyBufferToImage(sbuffer, t->image, w, h);

  transitionImageLayout(t->image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  m_vk.vkDestroyBuffer(m_device, sbuffer, null);
  m_vk.vkFreeMemory(m_device, smemory, null);

  kgm_log() << "Vulkan: Copied image from stage buffer to image.\n";

  VkImageViewCreateInfo viewInfo;

  ZeroObject(viewInfo);
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

  if (type == gctype_texcube)
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
  else
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

  viewInfo.format = format;
  viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.baseArrayLayer = 0;

  if (type == gctype_texcube)
    viewInfo.subresourceRange.layerCount = 6;
  else
    viewInfo.subresourceRange.layerCount = 1;

  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.image = t->image;

  result = m_vk.vkCreateImageView(m_device, &viewInfo, null, &t->iview);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create texture image view.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, t->image, null);

    m_vk.vkFreeMemory(m_device, t->memory, null);

    delete t;

    return null;
  }

  kgm_log() << "Vulkan: Generated image view.\n";

  VkSamplerCreateInfo samplerInfo;

  ZeroObject(samplerInfo);
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_FALSE;
  samplerInfo.maxAnisotropy = 1;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;

  result = m_vk.vkCreateSampler(m_device, &samplerInfo, null, &t->sampler);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create texture image view.\n";

    printResult(result);

    m_vk.vkDestroyImageView(m_device, t->iview, null);

    m_vk.vkDestroyImage(m_device, t->image, null);

    m_vk.vkFreeMemory(m_device, t->memory, null);

    delete t;

    return null;
  }

  kgm_log() << "Vulkan: Generated sampler.\n";

  kgm_log() << "Vulkan: Generated texture.\n";

  return t;
}

void  kgmVulkan::gcFreeTexture(void *t)
{
  if (!m_device || !t)
    return;

  if (((Texture*)t)->iview)
    m_vk.vkDestroyImageView(m_device, ((Texture*)t)->iview, null);

  if (((Texture*)t)->image)
    m_vk.vkDestroyImage(m_device, ((Texture*)t)->image, null);

  if (((Texture*)t)->memory)
    m_vk.vkFreeMemory(m_device, ((Texture*)t)->memory, null);

  delete (Texture*)t;
}

void  kgmVulkan::gcSetTexture(u32 stage, void *t)
{
  if (stage < VK_MAX_TEXTURES)
    m_textures[stage] = (Texture*) t;
}

// TARGET
gchandle kgmVulkan::gcGenTarget(u32 w, u32 h, bool depth, bool stencil)
{
  RenderTarget* target = new RenderTarget;

  ZeroObject(*target);

  VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

  VkImageCreateInfo imageInfo;

  ZeroObject(imageInfo);

  kgm_log() << "Vulkan info: Generating target.\n";

  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = w;
  imageInfo.extent.height = h;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  //imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkResult result;

  result = m_vk.vkCreateImage(m_device, &imageInfo, nullptr, &target->image);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create target texture image.\n";

    printResult(result);

    delete target;

    return null;
  }

  VkMemoryRequirements requirements;

  m_vk.vkGetImageMemoryRequirements(m_device, target->image, &requirements);

  VkMemoryAllocateInfo allocInfo;

  ZeroObject(allocInfo);
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = requirements.size;
  allocInfo.memoryTypeIndex = memoryTypeIndex(requirements.memoryTypeBits,
                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  result = m_vk.vkAllocateMemory(m_device, &allocInfo, null, &target->imageMemory);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to allocate texture image memory.\n";

    printResult(result);

    gcFreeTarget(target);

    delete target;

    return null;
  }

  result = m_vk.vkBindImageMemory(m_device, target->image, target->imageMemory, null);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to bind texture image memory.\n";

    printResult(result);

    gcFreeTarget(target);

    delete target;

    return null;
  }

  VkImageViewCreateInfo viewInfo;

  ZeroObject(viewInfo);
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  //viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.layerCount = 1;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.image = target->image;

  result = m_vk.vkCreateImageView(m_device, &viewInfo, null, &target->imageView);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create texture image view.\n";

    printResult(result);

    gcFreeTarget(target);

    delete target;

    return null;
  }

  kgm_log() << "Vulkan: Generated image view.\n";

  VkSamplerCreateInfo samplerInfo;

  ZeroObject(samplerInfo);
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  samplerInfo.anisotropyEnable = VK_FALSE;
  samplerInfo.maxAnisotropy = 1;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 1.0f;

  result = m_vk.vkCreateSampler(m_device, &samplerInfo, null, &target->sampler);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create texture image view.\n";

    printResult(result);

    gcFreeTarget(target);

    delete target;

    return null;
  }

  if (depth)
  {
    VkImageCreateInfo infoImage;

    ZeroObject(infoImage);

    infoImage.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    infoImage.tiling = VK_IMAGE_TILING_OPTIMAL;
    infoImage.format = chooseDepthFormat();
    infoImage.imageType = VK_IMAGE_TYPE_2D;
    infoImage.extent.width = w;
    infoImage.extent.height = h;
    infoImage.extent.depth = 1;
    infoImage.mipLevels = 1;
    infoImage.arrayLayers = 1;
    infoImage.samples = VK_SAMPLE_COUNT_1_BIT;
    infoImage.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    infoImage.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    //infoImage.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkImageViewCreateInfo infoView;

    ZeroObject(infoView);

    infoView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    infoView.pNext = NULL;
    infoView.image = VK_NULL_HANDLE;
    infoView.format = infoImage.format;
    infoView.components.r = VK_COMPONENT_SWIZZLE_R;
    infoView.components.g = VK_COMPONENT_SWIZZLE_G;
    infoView.components.b = VK_COMPONENT_SWIZZLE_B;
    infoView.components.a = VK_COMPONENT_SWIZZLE_A;
    infoView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    infoView.subresourceRange.baseMipLevel = 0;
    infoView.subresourceRange.levelCount = 1;
    infoView.subresourceRange.baseArrayLayer = 0;
    infoView.subresourceRange.layerCount = 1;
    infoView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    infoView.flags = 0;

    VkMemoryRequirements requirements;

    result = m_vk.vkCreateImage(m_device, &infoImage, NULL, &target->depth);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Cannot create depth image.\n";

      printResult(result);
    }
    else
    {
      m_vk.vkGetImageMemoryRequirements(m_device, target->depth, &requirements);

      VkMemoryAllocateInfo alloc;

      ZeroObject(alloc);

      alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      alloc.pNext = NULL;
      alloc.allocationSize = requirements.size;
      alloc.memoryTypeIndex = memoryTypeIndex(requirements.memoryTypeBits,
                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      result = m_vk.vkAllocateMemory(m_device, &alloc, NULL, &target->depthMemory);

      if (result != VK_SUCCESS)
      {
        kgm_log() << "Vulkan error: Cannot allocate memory for depth image.\n";

        printResult(result);

        m_vk.vkDestroyImage(m_device, target->depth, null);

        target->depth = VK_NULL_HANDLE;
      }
      else
      {
        result = m_vk.vkBindImageMemory(m_device, target->depth, target->depthMemory, 0);

        if (result != VK_SUCCESS)
        {
          kgm_log() << "Vulkan error: Cannot bind memory to depth image.\n";

          printResult(result);

          m_vk.vkDestroyImage(m_device, target->depth, null);
          m_vk.vkFreeMemory(m_device, target->depthMemory, null);

          target->depth = VK_NULL_HANDLE;
          target->depthMemory = VK_NULL_HANDLE;
        }
        else
        {
          VkImageViewCreateInfo viewInfo;

          ZeroObject(viewInfo);

          viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
          viewInfo.image = m_depthImage;
          viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
          viewInfo.format = infoImage.format;
          viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
          viewInfo.subresourceRange.baseMipLevel = 0;
          viewInfo.subresourceRange.levelCount = 1;
          viewInfo.subresourceRange.baseArrayLayer = 0;
          viewInfo.subresourceRange.layerCount = 1;

          result = m_vk.vkCreateImageView(m_device, &viewInfo, null, &target->depthView);

          if (result != VK_SUCCESS)
          {
            kgm_log() << "Vulkan error: Cannot create depth image view.\n";

            printResult(result);

            m_vk.vkDestroyImage(m_device, target->depth, null);
            m_vk.vkFreeMemory(m_device, target->depthMemory, null);

            target->depth = VK_NULL_HANDLE;
            target->depthMemory = VK_NULL_HANDLE;
          }
        }
      }
    }
  }

  VkAttachmentDescription attchmentDescriptions[2];

  ZeroObject(attchmentDescriptions);

  // Color attachment
  ZeroObject(attchmentDescriptions[0]);

  attchmentDescriptions[0].format = format;
  attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
  attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  // Depth attachment
  ZeroObject(attchmentDescriptions[1]);

  attchmentDescriptions[1].format = chooseDepthFormat();
  attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
  attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
  VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

  VkSubpassDescription subpassDescription;

  ZeroObject(subpassDescription);

  subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpassDescription.colorAttachmentCount = 1;
  subpassDescription.pColorAttachments = &colorReference;
  subpassDescription.pDepthStencilAttachment = &depthReference;

  VkSubpassDependency dependencies[2];

  ZeroObject(dependencies[0]);

  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

  ZeroObject(dependencies[1]);

  dependencies[1].srcSubpass = 0;
  dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

  VkRenderPassCreateInfo renderPassInfo;

  ZeroObject(renderPassInfo);

  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 2;
  renderPassInfo.pAttachments = attchmentDescriptions;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpassDescription;
  renderPassInfo.dependencyCount = 2;
  renderPassInfo.pDependencies = dependencies;

  result = m_vk.vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &target->renderPass);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create render pass for target.\n";

    gcFreeTarget(target);

    return null;
  }

  target->rp_id = ++rpass_cnt;

  VkFramebufferCreateInfo createInfo;

  ZeroObject(createInfo);

  VkImageView imageViews[2] = { target->imageView, target->depthView };

  createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  createInfo.renderPass = target->renderPass;
  createInfo.attachmentCount = 2; //(target->depthView) ? (2) : (1);
  createInfo.pAttachments = imageViews;
  createInfo.width = w;
  createInfo.height = h;
  createInfo.layers = 1;

  result = m_vk.vkCreateFramebuffer(m_device, &createInfo, nullptr, &target->framebuffer);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create framebuffer for target image view.\n";

    gcFreeTarget(target);

    return null;
  }

  kgm_log() << "Vulkan info: Generated target.\n";

  return (gchandle) target;
}

bool     kgmVulkan::gcTexTarget(gchandle tar, gchandle tex, u32 type)
{
  return false;
}

void kgmVulkan::gcFreeTarget(gchandle t)
{
  RenderTarget* target = (RenderTarget *) t;

  if (!t)
    return;

  if (target->framebuffer)
    m_vk.vkDestroyFramebuffer(m_device, target->framebuffer, null);

  if (target->imageView)
    m_vk.vkDestroyImageView(m_device, target->imageView, null);

  if (target->image)
    m_vk.vkDestroyImage(m_device, target->image, null);

  if (target->imageMemory)
    m_vk.vkFreeMemory(m_device, target->imageMemory, null);

  if (target->depthView)
    m_vk.vkDestroyImageView(m_device, target->depthView, null);

  if (target->depth)
    m_vk.vkDestroyImage(m_device, target->depth, null);

  if (target->depthMemory)
    m_vk.vkFreeMemory(m_device, target->depthMemory, null);

  if (target->renderPass)
    m_vk.vkDestroyRenderPass(m_device, target->renderPass, nullptr);

  delete target;
}

// VIEWPORT
void  kgmVulkan::gcSetViewport(int x, int y, int w, int h, float n, float f)
{
  m_viewport.x = x;
  m_viewport.y = y;
  m_viewport.width = w;
  m_viewport.height = h;
  m_viewport.minDepth = n;
  m_viewport.maxDepth = f;
  //m_viewport.minDepth = 0.0;
  //m_viewport.maxDepth = 1.0;
}

//BLEND
void  kgmVulkan::gcBlend(bool e, u32 m, u32 s, u32 d)
{
  m_blending = (e == true) ? (VK_TRUE) : (VK_FALSE);

  m_blendSource      = vk_blend(s);
  m_blendDestination = vk_blend(d);
}

//CULL
void  kgmVulkan::gcCull(u32 mode)
{
  if(!mode)
  {
    m_cullMode = VK_CULL_MODE_NONE;

    return;
  }

  m_cullFace = VK_FRONT_FACE_CLOCKWISE;

  switch(mode)
  {
  case gccull_back:
    m_cullMode = VK_CULL_MODE_BACK_BIT;
    break;
  case gccull_front:
  default:
    m_cullMode = VK_CULL_MODE_FRONT_BIT;
  }
}

//DEPTH
void  kgmVulkan::gcDepth(bool depth, bool mask, u32 mode)
{
  if(depth)
    m_depthTest = VK_TRUE;
  else
    m_depthTest = VK_FALSE;

  if(mask)
    m_depthWrite = VK_TRUE;
  else
    m_depthWrite = VK_FALSE;

  m_depthCompare = vk_compare(mode);
}

//SCISSOR
void kgmVulkan::gcScissor(bool on, int x, int y, int w, int h)
{
  m_sc_rect[0] = x;
  m_sc_rect[1] = y;
  m_sc_rect[2] = w;
  m_sc_rect[3] = h;

  if (on)
    m_sc_set = true;
  else
    m_sc_set = false;
}

//VERTEX & INDEX BUFFERS
void* kgmVulkan::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize)
{
  VertexBuffer* vb = new VertexBuffer;

  ZeroObject(*vb);

  if (!createBuffer(vsize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                    vb->vbuffer, vb->vmemory))
  {
    kgm_log() << "Vulkan error: Failed to prepare vertex buffer and memory!\n";

    return null;
  }

  void* data;

  if (m_vk.vkMapMemory(m_device, vb->vmemory, 0, vsize, 0, &data) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to map mamory!\n";

    gcFreeVertexBuffer(vb);

    return null;
  }

  memcpy(data, vdata, (size_t) vsize);

  m_vk.vkUnmapMemory(m_device, vb->vmemory);

  if (isize && idata)
  {
     VkDeviceSize bufferSize = isize;

     if (!createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                       vb->ibuffer, vb->imemory) != VK_SUCCESS)
     {
       kgm_log() << "Vulkan error: Failed create staging buffer!\n";

       gcFreeVertexBuffer(vb);

       return null;
     }

     if (m_vk.vkMapMemory(m_device, vb->imemory, 0, vsize, 0, &data) != VK_SUCCESS)
     {
       kgm_log() << "Vulkan error: Failed to map memory!\n";

       gcFreeVertexBuffer(vb);

       return null;
     }

     memcpy(data, idata, (size_t) isize);

     m_vk.vkUnmapMemory(m_device, vb->imemory);
  }

  return vb;
}

void  kgmVulkan::gcFreeVertexBuffer(void* p)
{
  VertexBuffer* vb = (VertexBuffer*) p;

  if (!vb)
    return;

  if (vb->vbuffer)
    m_vk.vkDestroyBuffer(m_device, vb->vbuffer, null);

  if (vb->vmemory)
    m_vk.vkFreeMemory(m_device, vb->vmemory, null);

  if (vb->ibuffer)
    m_vk.vkDestroyBuffer(m_device, vb->ibuffer, null);

  if (vb->imemory)
    m_vk.vkFreeMemory(m_device, vb->imemory, null);

  delete vb;
}

void  kgmVulkan::gcDrawVertexBuffer(void* b, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff)
{
  VertexBuffer* vb = (VertexBuffer*) b;

  if (!vb)
    return;

  if (!m_shader)
    return;

  if (pmt != gcpmt_trianglestrip && pmt != gcpmt_lines &&
      pmt != gcpmt_triangles && !gcpmt_linestrip)
    return;

  Draw* mesh = new Draw();

  ZeroObject(*mesh);

  mesh->constants.model    = m_shader->ubo.g_mTran;
  mesh->constants.color    = m_shader->ubo.g_vColor;
  mesh->constants.specular = m_shader->ubo.g_vSpecular;

  for (u32 i = 0; i < VK_MAX_TEXTURES; i++)
  {
    if (m_textures[i] != null)
      mesh->textures[i] = m_textures[i];
    else
      break;
  }

  if (isize && icnt)
  {
    mesh->isize = icnt * isize;
    mesh->icnt = icnt;

    if (isize == 2)
      mesh->itype = VK_INDEX_TYPE_UINT16;
    else
      mesh->itype = VK_INDEX_TYPE_UINT32;
  }

  switch(pmt)
  {
  case gcpmt_triangles:
    m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    break;
  case gcpmt_trianglestrip:
    m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    break;
  case gcpmt_lines:
    m_topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    break;
  case gcpmt_linestrip:
    m_topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    break;
  case gcpmt_points:
    m_topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    break;
  default:
    m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  };

  {
    u32 attr = 0;

    if (vfmt & gcv_xyz) {
      attr++;
    }
    if (vfmt & gcv_nor){
      attr++;
    }
    if (vfmt & gcv_uv0){
      attr++;
    }
    if (vfmt & gcv_uv1){
      attr++;
    }
    if (vfmt & gcv_col){
      attr++;
    }

    m_vertexAttributes = attr;
  }

  m_vertexFormat = vfmt;
  m_vertexStride = vsize;

  mesh->vcnt = vcnt;

  mesh->vbuffer = vb->vbuffer;
  mesh->vmemory = vb->vmemory;
  mesh->ibuffer = vb->ibuffer;
  mesh->imemory = vb->imemory;

  mesh->vbo = true;

  PipelineStatus ps = getCurrentPipelineStatus();

  Pipeline* pipeline = m_pipelines->get(&ps);

  if (!pipeline)
  {
    pipeline = createPipeline();

    m_pipelines->add(pipeline);
  }

  m_drawGroups.add(mesh, pipeline);
}

// SHADER
void* kgmVulkan::gcGenShader(kgmArray<u8>& v, kgmArray<u8>& f)
{
  if (!m_device)
  {
    kgm_log() << "Vulkan error: Device not initialized.\n";

    return null;
  }

  if (!v.length() || !f.length())
  {
    kgm_log() << "Vulkan error: Shader source absent.\n";

    return null;
  }

  Shader* shader = new Shader{};

  ZeroObject(*shader);

  VkShaderModuleCreateInfo createInfo;

  createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.codeSize = v.length();
  createInfo.pCode = (typeof createInfo.pCode) v.data();

  VkResult result = m_vk.vkCreateShaderModule(m_device, &createInfo, nullptr, &shader->vertex);

  if(result  != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create vertex shader.\n";

    printResult(result);

    delete shader;

    return null;
  }

  createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.codeSize = f.length();
  createInfo.pCode = (typeof createInfo.pCode) f.data();

  result = m_vk.vkCreateShaderModule(m_device, &createInfo, nullptr, &shader->fragment);

  if(result  != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create fragment shader.\n";

    m_vk.vkDestroyShaderModule(m_device, shader->vertex, null);

    delete shader;

    printResult(result);

    return null;
  }

  int n = sizeof(Shader::ubo);

  if (!createBuffer(m_uboSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    shader->buffer,
                    shader->memory))
  {
    kgm_log() << "Vulkan error: Cannot create memory buffer for shader.\n";

    m_vk.vkDestroyShaderModule(m_device, shader->vertex, null);
    m_vk.vkDestroyShaderModule(m_device, shader->fragment, null);

    delete shader;

    return null;
  }

  return shader;
}

void  kgmVulkan::gcFreeShader(void* s)
{
  Shader* shader = (Shader *) s;

  if (!s)
  {
    kgm_log() << "Vulkan error: Shader is invalid for free.\n";

    return;
  }

  if (!m_device)
  {
    kgm_log() << "Vulkan error: Device is invalid for free shader.\n";

    return;
  }

  if (shader->buffer)
    m_vk.vkDestroyBuffer(m_device, shader->buffer, nullptr);

  if (shader->memory)
    m_vk.vkFreeMemory(m_device, shader->memory, nullptr);


  if (shader->vertex)
    m_vk.vkDestroyShaderModule(m_device, ((Shader*)s)->vertex, null);

  if (shader->fragment)
    m_vk.vkDestroyShaderModule(m_device, ((Shader*)s)->fragment, null);

  if (shader->setpool)
    m_vk.vkDestroyDescriptorPool(m_device, shader->setpool, nullptr);

  if (shader->cache)
    m_vk.vkDestroyPipelineCache(m_device, shader->cache, null);

  if (shader->layout)
    m_vk.vkDestroyPipelineLayout(m_device, shader->layout, null);

  if (shader->setlayout)
    m_vk.vkDestroyDescriptorSetLayout(m_device, shader->setlayout, null);

  if (shader->pipeline)
    m_vk.vkDestroyPipeline(m_device, shader->pipeline, null);

  delete (Shader *) s;
}

void  kgmVulkan::gcSetShader(void* s)
{
  Shader* shader = (Shader*) s;

  if (shader && shader->buffer)
  {
    void *data;

    int s = sizeof(Shader::ubo);
    int n = s / 64;
    int m = s % 64;

    if (m_vk.vkMapMemory(m_device, shader->memory, 0, m_uboSize, 0, &data) == VK_SUCCESS)
    {
      memcpy(data, &shader->ubo, sizeof(Shader::ubo));
      VkMappedMemoryRange memoryRange;

      ZeroObject(memoryRange);
      memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      memoryRange.size = VK_WHOLE_SIZE;
      memoryRange.memory = shader->memory;
      m_vk.vkFlushMappedMemoryRanges(m_device, 1, &memoryRange);
      m_vk.vkUnmapMemory(m_device, shader->memory);
    }
  }

  m_shader = shader;
}

void  kgmVulkan::gcBindAttribute(void* s, int, const char*)
{

}

void  kgmVulkan::gcUniform(void* s,  u32 type, u32 cnt, const char* par, void* val)
{
  Shader* shader = (Shader *) s;

  if (!s)
    return;

  void* data = null;
  u32   size = 0;

  switch(type)
  {
  case gcunitype_float1:
    size = cnt * sizeof(float);
    break;
  case gcunitype_float2:
    size = 2 * cnt * sizeof(float);
    break;
  case gcunitype_float3:
    size = 3 * cnt * sizeof(float);
    break;
  case gcunitype_float4:
    size = 4 * cnt * sizeof(float);
    break;
  case gcunitype_int1:
    size = cnt * sizeof(int);
    break;
  case gcunitype_int2:
    size = 2 * cnt * sizeof(int);
    break;
  case gcunitype_int3:
    size = 3 * cnt * sizeof(int);
    break;
  case gcunitype_int4:
    size = 4 * cnt * sizeof(int);
    break;
  }

  data = uniformLocation(shader, (char *) par);

  if (data)
    memcpy(data, val, size);
}

void  kgmVulkan::gcUniformMatrix(void* s, u32 type, u32 cnt, u32 trn, const char* par, void* val)
{
  Shader* shader = (Shader *) s;

  if (!s)
    return;

  void* data = null;
  u32   size = 0;

  mtx4 m4;

  switch(type)
  {
  case gcunitype_mtx2:
    size = cnt * 4 * sizeof(float);
    break;
  case gcunitype_mtx3:
    size = cnt * 9 * sizeof(float);
    break;
  case gcunitype_mtx4:
    size = cnt * 16 * sizeof(float);
    memcpy(m4.m, val, 16 * sizeof(float));
    val = m4.m;
    break;
  }

  data = uniformLocation(shader, (char *) par);

  if (data)
    memcpy(data, val, size);
}

void  kgmVulkan::gcUniformSampler(void* s, const char*, void*)
{

}

#ifdef DEBUG
void  kgmVulkan::gcGetUniform(void* s, const char*, void*) {}
#endif

void kgmVulkan::clean(u32 options)
{

}

void kgmVulkan::freeSurface()
{
  if (!m_surface)
  {
    return;
  }

  m_vk.vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

  m_surface = 0;
}

bool kgmVulkan::initInstance()
{
  VkResult result = VK_SUCCESS;

  if(m_instance != nullptr)
  {
    kgm_log() << "Vulkan error: Instance already initialized.\n";

    return false;
  }

  u32 extensionsCount = 0;

  result = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

  if (result != VK_SUCCESS || extensionsCount == 0)
  {
    kgm_log() << "Vulkan error: No extensions supported!\n";

    printResult(result);

    return false;
  }

  kgm_log() << "Vulkan: Listed extensions supported!\n";

  m_extensionProperties.alloc(extensionsCount);
  m_extensionNames.alloc(extensionsCount);

  result = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, m_extensionProperties.data());

  if (result != VK_SUCCESS || extensionsCount == 0)
  {
    kgm_log() << "Vulkan error: No extensions supported!\n";

    printResult(result);

    return false;
  }

  u32 iname = 0;

  for (s32 i = 0; i < extensionsCount; i++)
  {
    const auto& ep = m_extensionProperties[i];

    kgm_log() << "Vulkan info: Extension name " << ep.extensionName << "\n";

    m_extensionNames[iname++] = ep.extensionName;
  }

  VkApplicationInfo appInfo = {};

  ZeroObject(appInfo);

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = "kgmEngine";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "kgmEngine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  uint32_t layerPropertyCount = 0;

  result = m_vk.vkEnumerateInstanceLayerProperties(&layerPropertyCount, nullptr);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Unable get instance layer properties count.\n";

    printResult(result);

    return false;
  }

  VkLayerProperties layerProperties[layerPropertyCount];

  result = m_vk.vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Unable get instance layer properties.\n";

    printResult(result);

    return false;
  }

  //const char* layerNames[layerPropertyCount];
  int actualLayerPropertyCount = 0;

  for (int i = 0; i < layerPropertyCount; i ++)
  {
    kgm_log() << "Exist layer: " << layerProperties[i].layerName << "\n";
    //if (!strcmp(layerProperties[i].layerName, "VK_LAYER_LUNARG_vktrace"))
    //{
    //  layerNames[i] = layerProperties[i].layerName;
    //  actualLayerPropertyCount++;
    //}

    //if (!strcmp(layerProperties[i].layerName, "VK_LAYER_LUNARG_standard_validation"))
    //{
    //  layerNames[i] = layerProperties[i].layerName;
    //  actualLayerPropertyCount++;
    //}
  }

  const char* layerNames[] = {
    "VK_LAYER_LUNARG_api_dump",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_screenshot",
    "VK_LAYER_LUNARG_standard_validation",
    //"VK_LAYER_LUNARG_image",
    //"VK_LAYER_LUNARG_swapchain",
    //"VK_LAYER_GOOGLE_threading",
    //"VK_LAYER_GOOGLE_unique_objects",
    //"VK_LAYER_LUNARG_vktrace",
    //"VK_LAYER_RENDERDOC_Capture",
    //"VK_LAYER_NV_optimus",
    //"VK_LAYER_VALVE_steam_overlay",
  };
  actualLayerPropertyCount = 0;

  VkInstanceCreateInfo instanceInfo = {};

  ZeroObject(instanceInfo);

  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext = NULL;
  instanceInfo.flags = 0;
  instanceInfo.pApplicationInfo = &appInfo;

#ifdef DEBUG
  if (getenv("VK_LAYER_PATH") || getenv("VULKAN_LAYER_WIN32"))
  {
    instanceInfo.enabledLayerCount = actualLayerPropertyCount;
    instanceInfo.ppEnabledLayerNames = layerNames;
  }
  else
  {
    kgm_log() << "Vulkan: Layer environment not provided.";
  }
#endif

  instanceInfo.enabledExtensionCount = extensionsCount;
  instanceInfo.ppEnabledExtensionNames = m_extensionNames.data();

  result = m_vk.vkCreateInstance(&instanceInfo, NULL, &m_instance);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Unable to create instance.\n";

    printResult(result);

    return false;
  }

  kgm_log() << "Vulkan: Instance created!\n";

  return true;
}

#ifdef DEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData) {

    kgm_log() << "Vulkan validation layer: " << pCallbackData->pMessage << ".\n";
    return VK_FALSE;
}

#ifdef WIN32

__stdcall VkBool32 FN_vkDebugReportCallbackEXT(
    VkDebugReportFlagsEXT                       flags,
    VkDebugReportObjectTypeEXT                  objectType,
    uint64_t                                    object,
    size_t                                      location,
    int32_t                                     messageCode,
    const char*                                 pLayerPrefix,
    const char*                                 pMessage,
    void*                                       pUserData)
    {
        kgm_log() << "Vulkan DEBUG: (";

        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0) kgm_log() << "INFO";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0) kgm_log() << "WARNING";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0) kgm_log() << "PERFORMANCE";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0) kgm_log() << "DEBUG";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0) kgm_log() << "ERROR";
        kgm_log() << ")";
        kgm_log() << "{" << pLayerPrefix << "} ";
        kgm_log() << pMessage << "\n";

        return VK_FALSE;
    }

#endif

#endif

bool kgmVulkan::initDebug()
{
#ifdef DEBUG

  m_vk.vkCreateDebugReportCallbackEXT = (typeof m_vk.vkCreateDebugReportCallbackEXT) m_vk.vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");

  if (m_vk.vkCreateDebugReportCallbackEXT == NULL)
  {
    kgm_log() << "Vulkan DEBUG: No debug report callback found.\n";
  }

  VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;

  debugReportCallbackCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  debugReportCallbackCreateInfo.pNext = nullptr;
  debugReportCallbackCreateInfo.flags = VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT;
  #ifdef WIN32
  debugReportCallbackCreateInfo.pfnCallback = FN_vkDebugReportCallbackEXT;
  #else
  debugReportCallbackCreateInfo.pfnCallback = [](
          VkDebugReportFlagsEXT flags,
          VkDebugReportObjectTypeEXT objectType,
          uint64_t object,
          size_t location,
          int32_t messageCode,
          const char* pLayerPrefix,
          const char* pMessage,
          void* pUserData) -> VkBool32
      {
        kgm_log() << "Vulkan DEBUG: (";

        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0) kgm_log() << "INFO";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0) kgm_log() << "WARNING";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0) kgm_log() << "PERFORMANCE";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0) kgm_log() << "DEBUG";
        if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0) kgm_log() << "ERROR";
        kgm_log() << ")";
        kgm_log() << "{" << pLayerPrefix << "} ";
        kgm_log() << pMessage << "\n";

        return VK_FALSE;
      };
  #endif
  debugReportCallbackCreateInfo.pUserData = this;

  if (m_vk.vkCreateDebugReportCallbackEXT && m_vk.vkCreateDebugReportCallbackEXT(m_instance,
      &debugReportCallbackCreateInfo, nullptr, &m_debugReportCallback)
      != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to create debug callback.\n";

    return false;
  }

  kgm_log() << "Vulkan: Debug callback created.\n";

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debugCreateInfo.messageSeverity =
      VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
      VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debugCreateInfo.messageType =
      VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debugCreateInfo.pfnUserCallback = DebugMessageCallback;    // global function
  debugCreateInfo.pUserData = this;

  VkDebugUtilsMessengerEXT debugMessenger;

  auto fd_mesg = (PFN_vkCreateDebugUtilsMessengerEXT)m_vk.vkGetInstanceProcAddr(m_instance,
                                                                                "vkCreateDebugUtilsMessengerEXT");
  if (fd_mesg)
  {
    if (fd_mesg(m_instance, &debugCreateInfo, NULL, &debugMessenger) != VK_SUCCESS)
      kgm_log() << "Vulkan error: Cannot create debug messenger.\n";
  }
  else
  {
    kgm_log() << "Vulkan error: Cannot find vkCreateDebugUtilsMessengerEXT.\n";
  }

#endif

  return true;
}

bool kgmVulkan::listDevices()
{
  if (!m_instance)
  {
    kgm_log() << "Vulkan error: Not valid instance.\n";

    return false;
  }

  u32 count = 0;

  VkResult result = m_vk.vkEnumeratePhysicalDevices(m_instance, &count, nullptr);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot get devices count.\n";

    printResult(result);

    return false;
  }

  if (count < 1)
  {
    kgm_log() << "Vulkan error: No valid devices.\n";

    return false;
  }

  kgm_log() << "Vulkan: Physical device count is " << count << ".\n";

  m_physicalDevices.alloc(count);

  //count = 1;

  result = m_vk.vkEnumeratePhysicalDevices(m_instance, &count, m_physicalDevices.data());

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot list devices.\n";

    printResult(result);

    return false;
  }

  if (count < 1)
  {
    m_physicalDevices.clear();

    kgm_log() << "Vulkan error: No valid device.\n";

    return false;
  }

  kgm_log() << "Vulkan Info: List devices.\n";

  for (int i = 0; i < count; i++)
  {
    auto& dev = m_physicalDevices[i];

    kgm_log() << "Vulkan Info: Suitable device check index " << i << ".\n";

    isDeviceSuitable(dev);
  }

  kgm_log() << "Vulkan Info: Choose device.\n";

  for (int i = 0; i < count; i++)
  {
    auto& dev = m_physicalDevices[i];

    if (isDeviceSuitable(dev))
    {
      m_physicalDevice = dev;

      break;
    }
  }

  if (m_physicalDevice == VK_NULL_HANDLE)
  {
    kgm_log() << "Vulkan: No suitable physical device found.\n";

    return false;
  }

  m_vk.vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);

  kgm_log() << "Vulkan: Physical device choosen\n";

  kgm_log() << "Vulkan: API version major is " << VK_VERSION_MAJOR(m_deviceProperties.apiVersion) << ".\n";
  kgm_log() << "Vulkan: API version minor is " << VK_VERSION_MINOR(m_deviceProperties.apiVersion) << ".\n";
  kgm_log() << "Vulkan: API version patch is " << VK_VERSION_PATCH(m_deviceProperties.apiVersion) << ".\n";


  if ((VK_VERSION_MAJOR(m_deviceProperties.apiVersion) < 1) ||
      ((VK_VERSION_MAJOR(m_deviceProperties.apiVersion) ==  1) && (VK_VERSION_MINOR(m_deviceProperties.apiVersion) < 1)))
  {
    kgm_log() << "Vulkan: Version is low.\n";

    return false;
  }

  m_api_version_major = VK_VERSION_MAJOR(m_deviceProperties.apiVersion);
  m_api_version_minor = VK_VERSION_MINOR(m_deviceProperties.apiVersion);
  m_api_version_patch = VK_VERSION_PATCH(m_deviceProperties.apiVersion);

  m_uboSize = m_deviceProperties.limits.nonCoherentAtomSize;

  while(m_uboSize < sizeof(Shader::ubo))
    m_uboSize += m_deviceProperties.limits.nonCoherentAtomSize;

  return true;
}

bool kgmVulkan::initDevice()
{
  VkResult result = VK_SUCCESS;

  if (!m_physicalDevice)
  {
    kgm_log() << "Vulkan error: Not choosed physical device.\n";

    return false;
  }

  VkPhysicalDevice physicalDevice = m_physicalDevice;

  kgmArray<VkQueueFamilyProperties> familyProperties;

  u32 count = 0;

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, null);

  if(count < 0)
  {
    kgm_log() << "kgmVulkan error: Unable get family properties count.\n";

    return false;
  }

  familyProperties.alloc(count);

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, familyProperties.data());

  if(count < 0)
  {
    kgm_log() << "kgmVulkan error: Unable get family properties.\n";

    return false;
  }

  bool found = false;

  for (u32 i = 0; i < count; i++)
  {
    if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      found = true;

      break;
    }
  }

  if (found)
  {
    u32 extensionsCount = 0;

    result = m_vk.vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties.\n";

      printResult(result);

      return false;
    }

    VkExtensionProperties deviceExtensionProperties[extensionsCount];

    result = m_vk.vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, deviceExtensionProperties);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties no extentions\n";

      return false;
    }

    bool isswapchain = false;

    for (const auto e: deviceExtensionProperties)
    {
      if (strcmp(e.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
        isswapchain = true;

      kgm_log() << "Vulkan: vkEnumerateDeviceExtensionProperties " << e.extensionName << "\n";
    }

    if (!isswapchain)
    {
      kgm_log() << "Vulkan error: vkEnumerateDeviceExtensionProperties no swap chain!\n";

      return false;
    }
  }

  VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

  m_vk.vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

  float priorities[] = {0.0f};

  VkDeviceQueueCreateInfo infoQueue = {};

  ZeroObject(infoQueue);

  infoQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  infoQueue.pNext = NULL;
  infoQueue.queueCount = 1;
  infoQueue.pQueuePriorities = priorities;

  VkPhysicalDeviceFeatures enabledFeatures;

  ZeroObject(enabledFeatures);

  enabledFeatures.shaderClipDistance = VK_TRUE;
  enabledFeatures.shaderCullDistance = VK_TRUE;

  const s8* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                 };

  VkDeviceCreateInfo infoDevice = {};

  ZeroObject(infoDevice);

  infoDevice.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  infoDevice.pNext = NULL;
  infoDevice.queueCreateInfoCount = 1;
  infoDevice.pQueueCreateInfos = &infoQueue;
  infoDevice.enabledExtensionCount = 1;
  infoDevice.ppEnabledExtensionNames = deviceExtensions;
  infoDevice.enabledLayerCount = 0;
  infoDevice.ppEnabledLayerNames = NULL;
  infoDevice.pEnabledFeatures = &enabledFeatures;

/*#ifdef DEBUG
  const char* layerNames[] = {
    "VK_LAYER_NV_optimus",
    "VK_LAYER_LUNARG_api_dump",
    "VK_LAYER_LUNARG_screenshot",
    "VK_LAYER_RENDERDOC_Capture",
    "VK_LAYER_VALVE_steam_overlay",
    "VK_LAYER_LUNARG_standard_validation",
  };

  if (getenv("VK_LAYER_PATH") || getenv("VULKAN_LAYER_WIN32"))
  {
    infoDevice.enabledLayerCount = 6;
    infoDevice.ppEnabledLayerNames = layerNames;
  }
  else
  {
    //m_debugLayer = "VK_LAYER_LUNARG_standard_validation";
    m_debugLayer = "VK_LAYER_KHRONOS_validation";
    infoDevice.enabledLayerCount = 1;
    infoDevice.ppEnabledLayerNames = &m_debugLayer;
  }
#endif*/

  VkDevice device = null;

  result = m_vk.vkCreateDevice(physicalDevice, &infoDevice, nullptr, &device);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: Cannot create device.\n";

    printResult(result);

    return false;
  }
  kgm_log() << "Vulkan: Create device.\n";

  m_device = device;

  return true;
}

bool kgmVulkan::initSurface()
{
  VkResult result = VK_SUCCESS;

#ifdef WIN32
    VkWin32SurfaceCreateInfoKHR createInfo;

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hinstance = GetModuleHandle(nullptr);
    createInfo.hwnd = m_window->m_wnd;

    result = m_vk.vkCreateWin32SurfaceKHR(m_instance, &createInfo, NULL, &m_surface);
#elif defined(ANDROID)
    GET_INSTANCE_PROC_ADDR(info.inst, CreateAndroidSurfaceKHR);

    VkAndroidSurfaceCreateInfoKHR createInfo;

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.window = AndroidGetApplicationWindow();
    res = info.fpCreateAndroidSurfaceKHR(info.inst, &createInfo, nullptr, &info.surface);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    VkWaylandSurfaceCreateInfoKHR createInfo = {};

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.display = m_window->m_display;
    createInfo.surface = m_window->m_surface;
    result = m_vk.vkCreateWaylandSurfaceKHR(m_instance, &createInfo, NULL, &m_surface);
#elif defined(XCB)
    VkXcbSurfaceCreateInfoKHR createInfo = {};

    ZeroObject(createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.connection = m_instance;
    createInfo.window = m_window->m_window;
    result = vkCreateXcbSurfaceKHR(m_instance, &createInfo, NULL, &info.surface);
#else
    VkXlibSurfaceCreateInfoKHR createInfo;

    ZeroObject(createInfo);

    createInfo.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.window = m_window->m_wnd;
    createInfo.dpy    = m_window->m_dpy;

    result = m_vk.vkCreateXlibSurfaceKHR(m_instance, &createInfo, nullptr, &m_surface);
#endif

  if(result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create surtface.\n";

    printResult(result);

    return false;
  }

  kgm_log() << "Vulkan: Created surtface.\n";

  #ifdef WIN32

  VkBool32 isSupported = 0;

  if(m_vk.vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, 0, m_surface, &isSupported) != VkResult::VK_SUCCESS || isSupported == VK_FALSE)
  {
    kgm_log() << "Vulkan error: Surtface not supported.\n";

    printResult(result);

    return false;
  }
  kgm_log() << "Vulkan: Surface supported.\n";

  #endif

  u32 queueFamilyCount = 0;

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

  if (queueFamilyCount < 1)
  {
    kgm_log()
        << "kgmVulkan error: vkGetPhysicalDeviceQueueFamilyProperties no count\n";

    return false;
  }

  VkQueueFamilyProperties queueFamilies[queueFamilyCount];

  m_vk.vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies);

  if (queueFamilyCount < 1)
  {
    kgm_log() << "kgmVulkan error: vkGetPhysicalDeviceQueueFamilyProperties no families\n";

    return false;
  }

  bool hasGraphicsQueueFamily = false;
  bool hasPresentQueueFamily = false;

  u32 graphicsQueueFamily = -1, presentQueueFamily = -1;

  for (u32 i = 0; i < queueFamilyCount; i++)
  {
    VkBool32 presentSupport = false;

    m_vk.vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);

    if (queueFamilies[i].queueCount > 0
        && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      graphicsQueueFamily = i;
      hasGraphicsQueueFamily = true;

      if (presentSupport)
      {
        presentQueueFamily = i;
        hasPresentQueueFamily = true;
        break;
      }
    }

    if (!hasPresentQueueFamily && presentSupport)
    {
      presentQueueFamily = i;
      hasPresentQueueFamily = true;
    }
  }

  if (hasGraphicsQueueFamily)
  {
    kgm_log() << "Vulkan: queue family #" << graphicsQueueFamily
        << " supports graphics.\n";

    if (hasPresentQueueFamily)
    {
      kgm_log() << "Vulkan queue family #" << presentQueueFamily
          << " supports presentation.\n";
    }
    else
    {
      kgm_log()
          << "Vulkan error: could not find a valid queue family with present support.\n";

      return false;
    }
  }
  else
  {
    kgm_log()
        << "Vulkan error: could not find a valid queue family with graphics support.\n";

    return false;
  }

  m_graphicsQueueFamilyIndex = graphicsQueueFamily;
  m_presentQueueFamilyIndex  = presentQueueFamily;

  m_vk.vkGetDeviceQueue(m_device, graphicsQueueFamily, 0, &m_graphicsQueue);
  m_vk.vkGetDeviceQueue(m_device, presentQueueFamily, 0, &m_presentQueue);
  m_vk.vkGetDeviceQueue(m_device, 0, 0, &m_queue);

  result = m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCapabilities);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: failed to get surface capabilities.\n";

    printResult(result);

    return false;
  }

  //if (SWAPCHAIN_IMAGES < m_surfaceCapabilities.minImageCount)
  //  SWAPCHAIN_IMAGES = m_surfaceCapabilities.minImageCount;

  u32 surfaceFormatsCount = 0;

  result = m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatsCount, nullptr);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get surface formats count.\n";

    printResult(result);

    return false;
  }

  VkSurfaceFormatKHR surfaceFormats[surfaceFormatsCount];

  result = m_vk.vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatsCount, surfaceFormats);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get surface formats.\n";

    printResult(result);

    return false;
  }

  if(surfaceFormatsCount < 1)
  {
    kgm_log() << "Vulkan error: Failed to get valid surface formats.\n";

    return false;
  }

  if (surfaceFormatsCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
  {
    m_swapChainFormat = VK_FORMAT_B8G8R8A8_UNORM;
  }
  else
  {
    m_swapChainFormat = surfaceFormats[0].format;
  }

  u32 presentModeCount = 0;

  result = m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);

  if (result != VK_SUCCESS || presentModeCount == 0)
  {
    kgm_log() << "Vulkan error: Failed to get number of supported presentation modes.\n";

    return false;
  }

  VkPresentModeKHR presentModes[presentModeCount];

  result = m_vk.vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to get supported presentation modes.\n";

    return false;
  }

  m_presentMode = VK_PRESENT_MODE_FIFO_KHR;

  for (u32 i = 0; i < presentModeCount; i++)
  {
    if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
      m_presentMode == presentModes[i];
  }

  return true;
}

bool kgmVulkan::initCommands()
{
  VkCommandPoolCreateInfo infoPool = { };

  if (!m_instance || !m_device)
  {
    kgm_log() << "Vulkan error: Not prepared to init commands.\n";

    return false;
  }

  infoPool.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  infoPool.pNext = null;
  infoPool.queueFamilyIndex = m_graphicsQueueFamilyIndex;
  infoPool.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  VkResult result = m_vk.vkCreateCommandPool(m_device, &infoPool, nullptr, &m_commandPool);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create command pool.\n";

    printResult(result);

    return false;
  }

  result = m_vk.vkResetCommandPool(m_device, m_commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot reset command pool.\n";

    printResult(result);

    m_vk.vkDestroyCommandPool(m_device, m_commandPool, null);

    m_commandPool = VK_NULL_HANDLE;

    return false;
  }

  VkCommandBufferAllocateInfo infoCommand = { };

  infoCommand.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  infoCommand.pNext = null;
  infoCommand.commandPool = m_commandPool;
  infoCommand.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  infoCommand.commandBufferCount = SWAPCHAIN_IMAGES;

  m_commandBuffers.alloc(SWAPCHAIN_IMAGES);

  result = m_vk.vkAllocateCommandBuffers(m_device, &infoCommand, m_commandBuffers.data());

  if(result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot allocate command buffer.\n";

    printResult(result);

    m_vk.vkDestroyCommandPool(m_device, m_commandPool, null);

    m_commandPool = VK_NULL_HANDLE;

    m_commandBuffers.clear();

    return false;
  }

  return true;
}

bool kgmVulkan::initSwapchain()
{
  VkResult result;

  VkExtent2D swapChainExtent = m_surfaceCapabilities.currentExtent;

  //u32 swapChainImagesCount = m_surfaceCapabilities.minImageCount;
  u32 swapChainImagesCount = SWAPCHAIN_IMAGES;

  VkSurfaceTransformFlagBitsKHR surfaceTransform;

  if (m_surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
  {
    surfaceTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }
  else
  {
    surfaceTransform = m_surfaceCapabilities.currentTransform;
  }

  VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] =
  { VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
    VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
  };

  for (uint32_t i = 0; i < sizeof(compositeAlphaFlags); i++)
  {
    if (m_surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i])
    {
      compositeAlpha = compositeAlphaFlags[i];

      break;
    }
  }

  VkSwapchainCreateInfoKHR swapchainCreateInfo = {};

  ZeroObject(swapchainCreateInfo);
  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.pNext = NULL;
  swapchainCreateInfo.surface = m_surface;
  swapchainCreateInfo.minImageCount = swapChainImagesCount;
  swapchainCreateInfo.imageFormat = m_swapChainFormat;
  swapchainCreateInfo.imageExtent.width = swapChainExtent.width;
  swapchainCreateInfo.imageExtent.height = swapChainExtent.height;
  swapchainCreateInfo.preTransform = surfaceTransform;
  swapchainCreateInfo.compositeAlpha = compositeAlpha;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.presentMode = m_presentMode;
  swapchainCreateInfo.clipped = VK_TRUE;
  swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = nullptr;
  swapchainCreateInfo.oldSwapchain = m_oldSwapChain;

  u32 queueFamilyIndices[2] = {m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex};

  if (m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex)
  {
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchainCreateInfo.queueFamilyIndexCount = 2;
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  }

  m_vk.vkQueueWaitIdle(m_graphicsQueue);
  m_vk.vkQueueWaitIdle(m_presentQueue);

  result = m_vk.vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapChain);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create swap chain.\n";

    return false;
  }

  kgm_log() << "Vulkan: Created swapchain.\n";

  if (m_oldSwapChain)
  {
    kgm_log() << "Vulkan: Desrtoying old swapchain...\n";
    m_vk.vkDestroySwapchainKHR(m_device, m_oldSwapChain, nullptr);
    kgm_log() << "Vulkan: Old swapchain destroyied.\n";

    m_oldSwapChain = VK_NULL_HANDLE;
  }

  uint32_t actualImageCount = swapChainImagesCount;

  result = m_vk.vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, nullptr);

  if (result != VK_SUCCESS || actualImageCount == 0)
  {
    kgm_log() << "Vulkan: failed to acquire number of swap chain images.\n";

    return false;
  }

  m_swapChainImages.alloc(actualImageCount);

  result = m_vk.vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, m_swapChainImages);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to acquire swap chain images.\n";

    return false;
  }

  kgm_log() << "Vulkan: Got swapchain images.\n";

  m_currentFrame = 0;

  return true;
}

bool kgmVulkan::initRenderPass()
{
  VkAttachmentDescription colorAttachmentDescription;

  ZeroObject(colorAttachmentDescription);

  colorAttachmentDescription.format = m_swapChainFormat;
  colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentDescription depthAttachmentDescription;

  ZeroObject(depthAttachmentDescription);
  depthAttachmentDescription.format = chooseDepthFormat();
  depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference colorAttachmentReference;

  ZeroObject(colorAttachmentReference);

  colorAttachmentReference.attachment = 0;
  colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentReference;

  ZeroObject(depthAttachmentReference);

  depthAttachmentReference.attachment = 1;
  depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subPassDescription;

  ZeroObject(subPassDescription);

  subPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subPassDescription.colorAttachmentCount = 1;
  subPassDescription.pColorAttachments = &colorAttachmentReference;
  subPassDescription.pDepthStencilAttachment = &depthAttachmentReference;

  VkSubpassDependency subPassDependency;

  ZeroObject(subPassDependency);

  subPassDependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
  subPassDependency.dstSubpass    = 0;
  subPassDependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subPassDependency.srcAccessMask = 0;
  subPassDependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subPassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkAttachmentDescription attachmentDescriptions[2] = { colorAttachmentDescription,
                                                        depthAttachmentDescription };
  VkRenderPassCreateInfo createInfo;

  ZeroObject(createInfo);

  createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  createInfo.attachmentCount = 2;
  createInfo.pAttachments = attachmentDescriptions;
  createInfo.subpassCount = 1;
  createInfo.pSubpasses = &subPassDescription;
  createInfo.dependencyCount = 1;
  createInfo.pDependencies = &subPassDependency;

  VkResult result = m_vk.vkCreateRenderPass(m_device, &createInfo, nullptr, &m_renderPass);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create render pass.\n";

    printResult(result);

    return false;
  }
  kgm_log() << "Vulkan: Created render pass.\n";

  return true;
}

bool kgmVulkan::initImageViews()
{
  u32 count = m_swapChainImages.length();

  if (count < 1)
  {
    kgm_log() << "Vulak error: Cannot init image views as swap images not initialized.\n";

    return false;
  }

  m_imageViews.alloc(count);

  for (u32 i = 0; i < count; i++)
  {
    VkImageViewCreateInfo createInfo = { };

    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.image = m_swapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = m_swapChainFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VkResult result = m_vk.vkCreateImageView(m_device, &createInfo, nullptr, &m_imageViews[i]);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulak error: failed to create image view for swap chain image #" << i << ".\n";

      printResult(result);

      return false;
    }
  }

  kgm_log() << "Vulkan: Creates image views.\n";

  return true;
}

bool kgmVulkan::initFrameBuffers()
{
  u32 count = m_swapChainImages.length();

  if (count < 1)
  {
    kgm_log() << "Vulak error: Cannot init frame buffers as swap images not initialized.\n";

    return false;
  }

  if (m_imageViews.length() < 1)
  {
    kgm_log() << "Vulak error: Cannot init frame buffers as image views not initialized.\n";

    return false;
  }

  m_frameBuffers.alloc(count);

  for (u32 i = 0; i < count; i++)
  {
    VkFramebufferCreateInfo createInfo = {};

    ZeroObject(createInfo);

    VkImageView imageViews[2] = { m_imageViews[i], m_depthImageView };

    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = m_renderPass;
    createInfo.attachmentCount = 2;
    createInfo.pAttachments = imageViews;
    createInfo.width = m_surfaceCapabilities.currentExtent.width;
    createInfo.height = m_surfaceCapabilities.currentExtent.height;
    //createInfo.width = m_rect[2];
    //createInfo.height = m_rect[3];
    createInfo.layers = 1;

    VkResult result = m_vk.vkCreateFramebuffer(m_device, &createInfo, nullptr, &m_frameBuffers[i]);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Failed to create framebuffer for swap chain image view #" << i << ".\n";

      return false;
    }
  }

  kgm_log() << "Vulkan: Created framebuffers.\n";

  return true;
}

bool kgmVulkan::initDepthBuffer()
{
  VkFormat candidates[3] = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT,
                             VK_FORMAT_D16_UNORM_S8_UINT };
  VkImageCreateInfo infoImage = {};

  ZeroObject(infoImage);

  VkFormat format = VK_FORMAT_UNDEFINED;

  for (u32 i = 0; i < 3; i ++)
  {
    VkFormatProperties properties;

    m_vk.vkGetPhysicalDeviceFormatProperties(m_physicalDevice, candidates[i], &properties);

    if (properties.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
      infoImage.tiling = VK_IMAGE_TILING_LINEAR;

      format = candidates[i];

      break;
    }
    else if (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
      infoImage.tiling = VK_IMAGE_TILING_OPTIMAL;

      format = candidates[i];

      break;
    }
  }

  if (format == VK_FORMAT_UNDEFINED)
  {
    kgm_log() << "Vulkan error: Cannot choose depth stencil format.\n";

    return false;
  }

  infoImage.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  infoImage.pNext = NULL;
  infoImage.imageType = VK_IMAGE_TYPE_2D;
  infoImage.format = format;
  infoImage.extent.width = m_surfaceCapabilities.currentExtent.width;
  infoImage.extent.height = m_surfaceCapabilities.currentExtent.height;
  infoImage.extent.depth = 1;
  infoImage.mipLevels = 1;
  infoImage.arrayLayers = 1;
  infoImage.samples = VK_SAMPLE_COUNT_1_BIT;
  infoImage.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  infoImage.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  infoImage.queueFamilyIndexCount = 0;
  infoImage.pQueueFamilyIndices = NULL;
  infoImage.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  infoImage.flags = 0;

  VkImageViewCreateInfo infoView;

  ZeroObject(infoView);

  infoView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  infoView.pNext = NULL;
  infoView.image = VK_NULL_HANDLE;
  infoView.format = format;
  infoView.components.r = VK_COMPONENT_SWIZZLE_R;
  infoView.components.g = VK_COMPONENT_SWIZZLE_G;
  infoView.components.b = VK_COMPONENT_SWIZZLE_B;
  infoView.components.a = VK_COMPONENT_SWIZZLE_A;
  infoView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  infoView.subresourceRange.baseMipLevel = 0;
  infoView.subresourceRange.levelCount = 1;
  infoView.subresourceRange.baseArrayLayer = 0;
  infoView.subresourceRange.layerCount = 1;
  infoView.viewType = VK_IMAGE_VIEW_TYPE_2D;
  infoView.flags = 0;

  VkMemoryRequirements requirements;

  m_depthFormat = format;

  VkResult result = m_vk.vkCreateImage(m_device, &infoImage, NULL, &m_depthImage);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create depth image.\n";

    printResult(result);

    return false;
  }

  m_vk.vkGetImageMemoryRequirements(m_device, m_depthImage, &requirements);

  VkMemoryAllocateInfo alloc;

  ZeroObject(alloc);

  alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc.pNext = NULL;
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex = memoryTypeIndex(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


  result = m_vk.vkAllocateMemory(m_device, &alloc, NULL, &m_depthMemory);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot allocate memory for depth image.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, m_depthImage, null);

    m_depthImage = VK_NULL_HANDLE;

    return false;
  }

  result = m_vk.vkBindImageMemory(m_device, m_depthImage, m_depthMemory, 0);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot bind memory to depth image.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, m_depthImage, null);
    m_vk.vkFreeMemory(m_device, m_depthMemory, null);

    m_depthImage = VK_NULL_HANDLE;
    m_depthMemory = VK_NULL_HANDLE;

    return false;
  }

  VkImageViewCreateInfo viewInfo;

  ZeroObject(viewInfo);

  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = m_depthImage;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  result = m_vk.vkCreateImageView(m_device, &viewInfo, null, &m_depthImageView);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create depth image view.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, m_depthImage, null);
    m_vk.vkFreeMemory(m_device, m_depthMemory, null);

    m_depthImage = VK_NULL_HANDLE;
    m_depthMemory = VK_NULL_HANDLE;

    return false;
  }

  m_depthFormat = format;

  return true;
}

bool kgmVulkan::initSemaphores()
{
  VkSemaphoreCreateInfo createInfo = {};

  ZeroObject(createInfo);

  createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if (m_vk.vkCreateSemaphore(m_device, &createInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
      m_vk.vkCreateSemaphore(m_device, &createInfo, nullptr, &m_renderingFinishedSemaphore) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create semaphores.\n";

    return false;
  }

  return true;
}

bool kgmVulkan::initPipeline()
{
  if (!m_device)
  {
    kgm_log() << "Vulkan error: Failed create pipelines as no device.\n";

    return false;
  }

  return true;
}

bool kgmVulkan::initSynchronizers()
{
  m_imageSemaphores.realloc(SWAPCHAIN_IMAGES);
  m_renderSemaphores.realloc(SWAPCHAIN_IMAGES);
  m_fences.realloc(SWAPCHAIN_IMAGES);

  VkSemaphoreCreateInfo semaphoreInfo;

  ZeroObject(semaphoreInfo);
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo;

  ZeroObject(fenceInfo);
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < SWAPCHAIN_IMAGES; i++)
  {
    if (m_vk.vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageSemaphores[i]) != VK_SUCCESS ||
        m_vk.vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderSemaphores[i]) != VK_SUCCESS ||
        m_vk.vkCreateFence(m_device, &fenceInfo, nullptr, &m_fences[i]) != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Failed to create synchronizers.\n";

      return false;
    }
  }

  fenceInfo.flags = 0;
  m_vk.vkCreateFence(m_device, &fenceInfo, nullptr, &m_fence);

  return true;
}

bool kgmVulkan::isDeviceSuitable(VkPhysicalDevice dev)
{
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;

  ZeroObject(deviceProperties);
  ZeroObject(deviceFeatures);

  m_vk.vkGetPhysicalDeviceProperties(dev, &deviceProperties);
  m_vk.vkGetPhysicalDeviceFeatures(dev, &deviceFeatures);

  kgm_log() << "Vulkan Info: device type is " << (int) deviceProperties.deviceType << "\n.";
  kgm_log() << "Vulkan Info: device name is " << deviceProperties.deviceName << "\n.";

  //return ((deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && deviceFeatures.geometryShader);
  return deviceFeatures.geometryShader;
}

bool kgmVulkan::destroyDevice()
{
  if (m_device)
    m_vk.vkDestroyDevice(m_device, nullptr);

  m_device = VK_NULL_HANDLE;

  initDevice();

  return true;
}

bool kgmVulkan::destroySwapchain()
{
  if(!m_instance || !m_device || !m_surface)
  {
    kgm_log() << "Vulkan error: Device is not valid.\n";

    return false;
  }

  VkResult result = m_vk.vkDeviceWaitIdle(m_device);

  if (result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: device wait idle failed.\n";

    printResult(result);

    return false;
  }

  if (m_depthImageView)
    m_vk.vkDestroyImageView(m_device, m_depthImageView, null);

  if (m_depthImage)
    m_vk.vkDestroyImage(m_device, m_depthImage, null);

  if (m_depthMemory)
    m_vk.vkFreeMemory(m_device, m_depthMemory, null);

  for (size_t i = 0; i < m_swapChainImages.length(); i++)
  {
    m_vk.vkResetCommandBuffer(m_commandBuffers[i], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    m_vk.vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr);
  }

  m_vk.vkFreeCommandBuffers(m_device, m_commandPool, (u32) m_commandBuffers.length(), m_commandBuffers.data());

  m_commandBuffers.clear();

  kgm_log() << "Vulkan: Command buffer deleted.\n";

  if (m_renderPass)
  {
    m_vk.vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    m_renderPass = VK_NULL_HANDLE;
  }

  kgm_log() << "Vulkan: Render pass destroyed.\n";

  for (size_t i = 0; i < m_swapChainImages.length(); i++)
  {
    m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
  }

  if (m_swapChain)
  {
    kgm_log() << "Vulkan: Desrtoying swapchain...\n";
    //m_vk.vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
    kgm_log() << "Vulkan: Swapchain destroyied.\n";
    m_oldSwapChain = m_swapChain;

    m_swapChain = VK_NULL_HANDLE;
  }

  m_frameBuffers.clear();
  m_imageViews.clear();
  m_swapChainImages.clear();

  clearDraws();

  return true;
}

bool kgmVulkan::refreshSwapchain()
{
  if(!m_instance || !m_device || !m_surface)
  {
    kgm_log() << "Vulkan error: Device is not valid.\n";

    return false;
  }

  VkResult result = m_vk.vkDeviceWaitIdle(m_device);

  if (result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: device wait idle failed.\n";

    printResult(result);

    return false;
  }

  if (m_depthImageView)
    m_vk.vkDestroyImageView(m_device, m_depthImageView, null);

  if (m_depthImage)
    m_vk.vkDestroyImage(m_device, m_depthImage, null);

  if (m_depthMemory)
    m_vk.vkFreeMemory(m_device, m_depthMemory, null);

  for (size_t i = 0; i < m_swapChainImages.length(); i++)
  {
    m_vk.vkResetCommandBuffer(m_commandBuffers[i], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    m_vk.vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr);
  }

  if (m_commandPool)
  {
    m_vk.vkFreeCommandBuffers(m_device, m_commandPool, (u32) m_commandBuffers.length(), m_commandBuffers.data());

    m_commandBuffers.clear();

    m_vk.vkDestroyCommandPool(m_device, m_commandPool, nullptr);

    kgm_log() << "Vulkan: Command buffer destroyed.\n";
  }


  if (m_renderPass)
  {
    m_vk.vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    m_renderPass = VK_NULL_HANDLE;
  }

  kgm_log() << "Vulkan: Render pass destroyed.\n";

  for (size_t i = 0; i < m_swapChainImages.length(); i++)
  {
    m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
  }

  if (m_swapChain)
  {
    kgm_log() << "Vulkan: Desrtoying swapchain...\n";
    //m_vk.vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
    kgm_log() << "Vulkan: Swapchain destroyied.\n";
    m_oldSwapChain = m_swapChain;

    m_swapChain = VK_NULL_HANDLE;
  }

  m_frameBuffers.clear();
  m_imageViews.clear();
  m_swapChainImages.clear();

  clearDraws();

  kgm_log() << "Vulkan: Framebuffers/Imageviews are destroyed.\n";

  result = m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCapabilities);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: failed to get surface capabilities.\n";

    printResult(result);

    return false;
  }

  initSwapchain();
  initRenderPass();
  initImageViews();
  initDepthBuffer();
  initFrameBuffers();
  initCommands();

  return true;
}

void kgmVulkan::fillCommands()
{
  VkResult result = VK_SUCCESS;

  s32 i = m_currentFrame;

  for (i = 0; i < m_swapChainImages.length(); i++)
  {
    auto &commandBuffer = m_commandBuffers[i];
    auto &image         = m_swapChainImages[i];
    auto &framebuffer   = m_frameBuffers[i];

    result = m_vk.vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

    static VkImageMemoryBarrier imageMemoryBarrier;

    ZeroObject(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = nullptr;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
    | VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT;
    imageMemoryBarrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
    //imageMemoryBarrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.srcQueueFamilyIndex = 0;
    imageMemoryBarrier.dstQueueFamilyIndex = 0;
    imageMemoryBarrier.image = image;
    //imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1};
    imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

    VkCommandBufferInheritanceInfo commandBufferInheritanceInfo;

    ZeroObject(commandBufferInheritanceInfo);
    commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    commandBufferInheritanceInfo.pNext = nullptr;
    commandBufferInheritanceInfo.renderPass = m_renderPass;
    commandBufferInheritanceInfo.subpass = 0;
    commandBufferInheritanceInfo.framebuffer = framebuffer;
    commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
    commandBufferInheritanceInfo.queryFlags = 0;
    commandBufferInheritanceInfo.pipelineStatistics = 0;

    VkCommandBufferBeginInfo commandBufferBeginInfo;

    ZeroObject(commandBufferBeginInfo);
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = nullptr;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = &commandBufferInheritanceInfo;

    result = m_vk.vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Cannot begin command buffer.\n";
    }

    VkClearValue clearValue[2];

    u32 clearCount = 1;

    ZeroObject(clearValue[0]);

    memcpy(clearValue[0].color.float32, m_bgColor, sizeof(float) * 4);

    if ((m_clear | gcflag_color) || (m_clear | gcflag_stencil))
    {
      ZeroObject(clearValue[1]);

      clearValue[1].depthStencil.depth = m_depth;
      clearValue[1].depthStencil.stencil = m_stensil;

      clearCount++;
    }

    VkRenderPassBeginInfo renderPassBeginInfo;

    ZeroObject(renderPassBeginInfo);
    renderPassBeginInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.framebuffer = framebuffer;
    renderPassBeginInfo.renderArea = VkRect2D {
    VkOffset2D  {0, 0},
    VkExtent2D  { m_surfaceCapabilities.currentExtent.width, m_surfaceCapabilities.currentExtent.height } };

    renderPassBeginInfo.clearValueCount = clearCount;
    renderPassBeginInfo.pClearValues = clearValue;

    m_vk.vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    m_viewport.x      = 0;
    m_viewport.y      = 0;
    m_viewport.width  = m_surfaceCapabilities.currentExtent.width;
    m_viewport.height = m_surfaceCapabilities.currentExtent.height;

    m_vk.vkCmdSetViewport(commandBuffer, 0, 1, &m_viewport);
    m_vk.vkCmdSetScissor(commandBuffer, 0, 1, &m_scissor);

    kgmList<DrawGroup*>::iterator ig = m_drawGroups.groups.begin();

    for(; ig != m_drawGroups.groups.end(); ig.next())
    {
      kgmList<Draw*>::iterator di = (*ig)->draws.begin();

      Pipeline* pipeline = (*ig)->pipeline;

      m_vk.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);

      for (; di != (*ig)->draws.end(); di.next())
      {
        Draw* draw = (*di);

        VkBuffer vertexBuffer = draw->vbuffer;

        if (!vertexBuffer)
          continue;

        VkBuffer vertexBuffers[] = {vertexBuffer};

        VkDeviceSize offsets[] = {0};

        if (pipeline->descriptor)
        {
          if (draw->textures[0])
          {
            VkDescriptorBufferInfo bufferInfo;

            ZeroObject(bufferInfo);
            bufferInfo.buffer = pipeline->shader->buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(Uniforms);

            VkDescriptorImageInfo imageInfo[VK_MAX_TEXTURES];
            u32                   imageInfoCount = 0;

            ZeroObject(imageInfo);

            for (u32 i = 0; i < VK_MAX_TEXTURES; i++)
            {
              if (draw->textures[i] == null)
                break;

              imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
              imageInfo[i].imageView   = draw->textures[i]->iview;
              imageInfo[i].sampler     = draw->textures[i]->sampler;

              imageInfoCount++;
            }

            VkWriteDescriptorSet descriptorWrites[2];

            u32 descriptorWritesCount = 2;

            ZeroObject(descriptorWrites[0]);
            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = pipeline->descriptor;
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            ZeroObject(descriptorWrites[1]);
	          descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	          descriptorWrites[1].dstSet = pipeline->descriptor;
	          descriptorWrites[1].dstBinding = 1;
	          descriptorWrites[1].dstArrayElement = 0;
	          descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	          descriptorWrites[1].descriptorCount = imageInfoCount;
	          descriptorWrites[1].pImageInfo = imageInfo;

            m_vk.vkUpdateDescriptorSets(m_device, descriptorWritesCount, descriptorWrites, 0, nullptr);
          }

          m_vk.vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout,
          0, 1, &pipeline->descriptor, 0, nullptr);

          m_vk.vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0,
          sizeof(PushConstants), &draw->constants);
        }

        m_vk.vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout,
        0, 1, &pipeline->descriptor, 0, nullptr);


        m_vk.vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        if (draw->ibuffer)
        {
          m_vk.vkCmdBindIndexBuffer(commandBuffer, draw->ibuffer, 0, draw->itype);
          m_vk.vkCmdDrawIndexed(commandBuffer, draw->icnt, 1, 0, 0, 0);
        }
        else
        {
          m_vk.vkCmdDraw(commandBuffer, static_cast<uint32_t>(draw->vcnt), 1, 0, 0);
        }
      }
    }

    m_vk.vkCmdEndRenderPass(commandBuffer);

    //m_vk.vkCmdPipelineBarrier(commandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    //                          VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    //                          0, 0, nullptr, 0, nullptr,  1, &imageMemoryBarrier);

    m_vk.vkEndCommandBuffer(commandBuffer);
  }
}

bool kgmVulkan::createBuffer(u32 size, VkBufferUsageFlags  usage, VkMemoryPropertyFlags properties,
                             VkBuffer& buffer, VkDeviceMemory& memory)
{
  VkBufferCreateInfo bufferInfo = {};

  ZeroObject(bufferInfo);
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkResult result = m_vk.vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: cannot create buffer.\n";

    printResult(result);

    return false;
  }

  VkMemoryRequirements memRequirements;

  ZeroObject(memRequirements);
  m_vk.vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo = { };

  ZeroObject(allocInfo);
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = memoryTypeIndex(memRequirements.memoryTypeBits, properties);

  result = m_vk.vkAllocateMemory(m_device, &allocInfo, nullptr, &memory);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: cannot allocate memory.\n";

    printResult(result);

    m_vk.vkDestroyBuffer(m_device, buffer, null);

    return false;
  }

  result = m_vk.vkBindBufferMemory(m_device, buffer, memory, 0);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot bind memory.\n";

    printResult(result);

    m_vk.vkFreeMemory(m_device, memory, null);
    m_vk.vkDestroyBuffer(m_device, buffer, null);

    return false;
  }

  return true;
}

u32 kgmVulkan::memoryTypeIndex(u32 type,  VkMemoryPropertyFlags properties)
{
  VkPhysicalDeviceMemoryProperties memProperties;

  m_vk.vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

  for (u32 i = 0; i < memProperties.memoryTypeCount; i++)
  {
    auto bit = ((u32)1 << i);

    if (type & bit)
    {
      if((memProperties.memoryTypes[i].propertyFlags & properties) != 0)
        return i;
    }
  }

  return -1;
}

VkCommandBuffer kgmVulkan::beginSingleTimeCommand()
{
  VkCommandBufferAllocateInfo allocInfo;

  ZeroObject(allocInfo);
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = m_commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;

  VkResult result = m_vk.vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

  if(result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to allocate single command.\n";

    printResult(result);

    return VK_NULL_HANDLE;
  }

  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  result = m_vk.vkBeginCommandBuffer(commandBuffer, &beginInfo);

  if(result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to begin single command.\n";

    printResult(result);

    m_vk.vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);

    return VK_NULL_HANDLE;
  }

  return commandBuffer;
}

void kgmVulkan::endSingleTimeCommand(VkCommandBuffer commandBuffer)
{
  m_vk.vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo;

  ZeroObject(submitInfo);
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  VkResult result = m_vk.vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

  if (result == VK_SUCCESS)
  {
    result = m_vk.vkQueueWaitIdle(m_graphicsQueue);

    if(result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Failed to wait idle of single command.\n";

      printResult(result);
    }
  }
  else
  {
    kgm_log() << "Vulkan error: Failed submit queue for single command.\n";

    printResult(result);
  }

  m_vk.vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
}

bool kgmVulkan::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
  VkCommandBuffer commandBuffer = beginSingleTimeCommand();

  VkImageMemoryBarrier barrier;

  ZeroObject(barrier);
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
  {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }
  else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
  {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }
  else
  {
    return false;
  }

  m_vk.vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage,
                            0, 0, nullptr, 0, nullptr, 1, &barrier);

  endSingleTimeCommand(commandBuffer);

  return true;
}

void kgmVulkan::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
  kgm_log() << "Vulkan: Start copy buffer to image.\n";
  VkCommandBuffer commandBuffer = beginSingleTimeCommand();

  VkBufferImageCopy region;

  ZeroObject(region);
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {
    width,
    height,
    1
  };

  m_vk.vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  endSingleTimeCommand(commandBuffer);
  kgm_log() << "Vulkan: End copy buffer to image.\n";
}

void kgmVulkan::copyBufferToImageCube(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t bpp)
{
  kgm_log() << "Vulkan: Start copy buffer to cube image.\n";
  VkCommandBuffer commandBuffer = beginSingleTimeCommand();

  VkBufferImageCopy region[4];

  ZeroObject(region);

  for (int i = 0; i < 6; i++)
  {
    region[i].bufferOffset = i * bpp * width * height;
    region[i].bufferRowLength = 0;
    region[i].bufferImageHeight = 0;
    region[i].imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region[i].imageSubresource.mipLevel = 0;
    region[i].imageSubresource.baseArrayLayer = 0;
    region[i].imageSubresource.layerCount = 1;
    region[i].imageOffset = {0, 0, 0};
    region[i].imageExtent = {
      width,
      height,
      1
    };
  }

  m_vk.vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6, region);

  endSingleTimeCommand(commandBuffer);
  kgm_log() << "Vulkan: End copy buffer to image.\n";
}

VkFormat kgmVulkan::chooseDepthFormat()
{
  VkFormat candidates[3] = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT,
                             VK_FORMAT_D16_UNORM_S8_UINT };

  for (u32 i = 0; i < 3; i ++)
  {
    VkFormatProperties properties;

    m_vk.vkGetPhysicalDeviceFormatProperties(m_physicalDevice, candidates[i], &properties);

    if (properties.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
      return candidates[i];
    }
    else if (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
      return candidates[i];
    }
  }

  return VK_FORMAT_UNDEFINED;
}

void kgmVulkan::freePipeline(Pipeline* p)
{
  if (!p)
    return;

  if (p->pipeline)
    m_vk.vkDestroyPipeline(m_device, p->pipeline, null);

  if (p->cache)
    m_vk.vkDestroyPipelineCache(m_device, p->cache, null);

  if (p->layout)
    m_vk.vkDestroyPipelineLayout(m_device, p->layout, null);

  if (p->setpool)
    m_vk.vkDestroyDescriptorPool(m_device, p->setpool, null);

  if (p->setlayout)
    m_vk.vkDestroyDescriptorSetLayout(m_device, p->setlayout, null);

  delete p;
}

kgmVulkan::Pipeline* kgmVulkan::createPipeline()
{
  if (!m_shader)
    return null;

  VkResult result = VK_SUCCESS;

  Pipeline* pipeline = new Pipeline;

  ZeroObject(*pipeline);

  kgmArray<VkDescriptorSetLayoutBinding> layoutBindings;

  VkDescriptorSetLayoutBinding uboLayoutBinding;

  ZeroObject(uboLayoutBinding);
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
  layoutBindings.add(uboLayoutBinding);

  ZeroObject(uboLayoutBinding);
  uboLayoutBinding.binding = 1;
  uboLayoutBinding.descriptorCount = VK_MAX_TEXTURES;
  uboLayoutBinding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  uboLayoutBinding.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
  layoutBindings.add(uboLayoutBinding);

  VkDescriptorSetLayoutCreateInfo layoutCreateInfo;

  ZeroObject(layoutCreateInfo);
  layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutCreateInfo.bindingCount = layoutBindings.length();
  layoutCreateInfo.pBindings = layoutBindings.data();

  kgm_log() << "Vulkan info: layoutBinding count " << layoutCreateInfo.bindingCount << ".\n";

  result = m_vk.vkCreateDescriptorSetLayout(m_device, &layoutCreateInfo, null, &pipeline->setlayout);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create descriptor layout.\n";

    printResult(result);

    freePipeline(pipeline);

    return null;
  }

  VkDescriptorPoolSize poolSizes[2];

  ZeroObject(poolSizes[0]);
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = 1;

  ZeroObject(poolSizes[1]);
  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = VK_MAX_TEXTURES;

  VkDescriptorPoolCreateInfo poolInfo;

  ZeroObject(poolInfo);
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 2;
  poolInfo.pPoolSizes = poolSizes;
  poolInfo.maxSets = 2;

  result = m_vk.vkCreateDescriptorPool(m_device, &poolInfo, null, &pipeline->setpool);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create descriptor pool.\n";

    printResult(result);

    freePipeline(pipeline);

    return null;
  }

  VkDescriptorSetAllocateInfo allocInfo;

  ZeroObject(allocInfo);
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = pipeline->setpool;
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts = &pipeline->setlayout;

  result = m_vk.vkAllocateDescriptorSets(m_device, &allocInfo, &pipeline->descriptor);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create descriptor set.\n";

    printResult(result);

    freePipeline(pipeline);

    return null;
  }

  VkDescriptorBufferInfo bufferInfo;

  ZeroObject(bufferInfo);
  bufferInfo.buffer = m_shader->buffer;
  bufferInfo.offset = 0;
  bufferInfo.range = sizeof(Uniforms);

  VkDescriptorImageInfo imageInfo[VK_MAX_TEXTURES];
  u32                   imageInfoCount = 0;

  ZeroObject(imageInfo);

  for (u32 i = 0; i < VK_MAX_TEXTURES; i++)
  {
    if (m_textures[i] == null)
      break;

    imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo[i].imageView   = m_textures[i]->iview;
    imageInfo[i].sampler     = m_textures[i]->sampler;

    imageInfoCount++;
  }

  VkWriteDescriptorSet descriptorWrites[2];

  u32 descriptorWritesCount = 2;

  ZeroObject(descriptorWrites[0]);
  descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[0].dstSet = pipeline->descriptor;
  descriptorWrites[0].dstBinding = 0;
  descriptorWrites[0].dstArrayElement = 0;
  descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrites[0].descriptorCount = 1;
  descriptorWrites[0].pBufferInfo = &bufferInfo;

  descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[1].dstSet = pipeline->descriptor;
  descriptorWrites[1].dstBinding = 1;
  descriptorWrites[1].dstArrayElement = 0;
  descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  descriptorWrites[1].descriptorCount = imageInfoCount;
  descriptorWrites[1].pImageInfo = imageInfo;

  m_vk.vkUpdateDescriptorSets(m_device, descriptorWritesCount, descriptorWrites, 0, nullptr);

  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

  ZeroObject(pipelineLayoutCreateInfo);
  pipelineLayoutCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.pNext = nullptr;
  pipelineLayoutCreateInfo.flags = 0;

  if (pipeline->setlayout)
  {
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &pipeline->setlayout;
  }

  VkPushConstantRange pushConstRange;
  pushConstRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  pushConstRange.size       = sizeof(PushConstants);
  pushConstRange.offset     = 0;

  pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
  pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstRange;

  result = m_vk.vkCreatePipelineLayout(m_device, &pipelineLayoutCreateInfo, nullptr, &pipeline->layout);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create pipeline layout.\n";

    printResult(result);

    freePipeline(pipeline);

    return null;
  }

  VkPipelineCacheCreateInfo pipelineCacheCreateInfo;

  ZeroObject(pipelineCacheCreateInfo);
  pipelineCacheCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  pipelineCacheCreateInfo.pNext = nullptr;
  pipelineCacheCreateInfo.flags = 0;
  pipelineCacheCreateInfo.initialDataSize = 0;
  pipelineCacheCreateInfo.pInitialData = nullptr;

  result = m_vk.vkCreatePipelineCache(m_device, &pipelineCacheCreateInfo, nullptr, &pipeline->cache);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create fragment shader.\n";

    printResult(result);

    freePipeline(pipeline);

    return null;
  }

  VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2];

  ZeroObject(pipelineShaderStageCreateInfos[0]);
  pipelineShaderStageCreateInfos[0].sType  = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  pipelineShaderStageCreateInfos[0].pNext  = nullptr;
  pipelineShaderStageCreateInfos[0].flags  = 0;
  pipelineShaderStageCreateInfos[0].stage  = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
  pipelineShaderStageCreateInfos[0].module =  m_shader->vertex;
  pipelineShaderStageCreateInfos[0].pName = "main";
  pipelineShaderStageCreateInfos[0].pSpecializationInfo = nullptr;

  ZeroObject(pipelineShaderStageCreateInfos[1]);
  pipelineShaderStageCreateInfos[1].sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  pipelineShaderStageCreateInfos[1].pNext = nullptr;
  pipelineShaderStageCreateInfos[1].flags = 0;
  pipelineShaderStageCreateInfos[1].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
  pipelineShaderStageCreateInfos[1].module = m_shader->fragment;
  pipelineShaderStageCreateInfos[1].pName = "main";
  pipelineShaderStageCreateInfos[1].pSpecializationInfo = nullptr;

  VkVertexInputBindingDescription vertexInputBindingDescription;

  ZeroObject(vertexInputBindingDescription);
  vertexInputBindingDescription.binding = 0;
  vertexInputBindingDescription.stride = m_vertexStride;
  vertexInputBindingDescription.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;

  u32 offset = 0, location = 0;
  VkFormat format = VK_FORMAT_R32G32B32_SFLOAT;

  VkVertexInputAttributeDescription vertexInputAttributeDescription[m_vertexAttributes];

  ZeroObject(vertexInputAttributeDescription[0]);
  vertexInputAttributeDescription[0].location = location;
  vertexInputAttributeDescription[0].binding = 0;
  vertexInputAttributeDescription[0].format = format;
  vertexInputAttributeDescription[0].offset = offset;

  offset += (sizeof(float) * 3);
  location++;

  u32 attributes = m_vertexFormat;

  for (u32 i = 1; i < m_vertexAttributes; i++)
  {
    u32 size = 0;

    if (attributes & gcv_nor)
    {
      format = VK_FORMAT_R32G32B32_SFLOAT;
      size = sizeof(float) * 3;

      attributes = (attributes ^ gcv_nor);
    }
    else if (attributes & gcv_col)
    {
      //format = VK_FORMAT_R8G8B8A8_UINT;
      format = VK_FORMAT_R32G32B32A32_SFLOAT;
      size = sizeof(float) * 4;

      attributes = (attributes ^ gcv_col);
    }
    else if (attributes & gcv_uv0)
    {
      format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
      size = sizeof(float) * 2;

      attributes = (attributes ^ gcv_uv0);
    }
    else if (attributes & gcv_uv1)
    {
      format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
      size = sizeof(float) * 2;

      attributes = (attributes ^ gcv_uv1);
    }
    else
    {
      continue;
    }

    ZeroObject(vertexInputAttributeDescription[i]);
    vertexInputAttributeDescription[i].location = location;
    vertexInputAttributeDescription[i].binding = 0;
    vertexInputAttributeDescription[i].format = format;
    vertexInputAttributeDescription[i].offset = offset;

    location++;
    offset += size;
  }

  VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;

  ZeroObject(pipelineVertexInputStateCreateInfo);
  pipelineVertexInputStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  pipelineVertexInputStateCreateInfo.pNext = nullptr;
  pipelineVertexInputStateCreateInfo.flags = 0;
  pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
  pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
  pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = location;
  pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescription;

  VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;

  ZeroObject(pipelineInputAssemblyStateCreateInfo);
  pipelineInputAssemblyStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
  pipelineInputAssemblyStateCreateInfo.flags = 0;
  pipelineInputAssemblyStateCreateInfo.topology = m_topology;
  pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

  VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;

  ZeroObject(pipelineViewportStateCreateInfo);
  pipelineViewportStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  pipelineViewportStateCreateInfo.pNext = nullptr;
  pipelineViewportStateCreateInfo.flags = 0;
  pipelineViewportStateCreateInfo.viewportCount = 1;
  pipelineViewportStateCreateInfo.pViewports = &m_viewport;
  pipelineViewportStateCreateInfo.scissorCount = 1;
  pipelineViewportStateCreateInfo.pScissors = &m_scissor;

  VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;

  ZeroObject(pipelineRasterizationStateCreateInfo);
  pipelineRasterizationStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  pipelineRasterizationStateCreateInfo.pNext = nullptr;
  pipelineRasterizationStateCreateInfo.flags = 0;
  pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
  pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
  pipelineRasterizationStateCreateInfo.polygonMode = m_polygonMode; //VkPolygonMode::VK_POLYGON_MODE_FILL;
  pipelineRasterizationStateCreateInfo.cullMode = m_cullMode;  //VK_CULL_MODE_BACK_BIT;//VK_CULL_MODE_NONE;
  pipelineRasterizationStateCreateInfo.frontFace = m_cullFace; //VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
  pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
  pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
  pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
  pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
  pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

  VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

  ZeroObject(pipelineMultisampleStateCreateInfo);
  pipelineMultisampleStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  pipelineMultisampleStateCreateInfo.pNext = nullptr;
  pipelineMultisampleStateCreateInfo.flags = 0;
  pipelineMultisampleStateCreateInfo.rasterizationSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
  pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
  pipelineMultisampleStateCreateInfo.minSampleShading = 0;
  pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
  pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
  pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

  VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;

  ZeroObject(pipelineColorBlendAttachmentState);
  pipelineColorBlendAttachmentState.blendEnable = m_blending;
  pipelineColorBlendAttachmentState.srcColorBlendFactor = m_blendSource;
  pipelineColorBlendAttachmentState.dstColorBlendFactor = m_blendDestination;
  pipelineColorBlendAttachmentState.colorBlendOp = VkBlendOp::VK_BLEND_OP_ADD;
  pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
  pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
  pipelineColorBlendAttachmentState.alphaBlendOp = VkBlendOp::VK_BLEND_OP_ADD;
  pipelineColorBlendAttachmentState.colorWriteMask = VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT |
                                                     VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT |
                                                     VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT;

  VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;

  ZeroObject(pipelineColorBlendStateCreateInfo);
  pipelineColorBlendStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  pipelineColorBlendStateCreateInfo.pNext = nullptr;
  pipelineColorBlendStateCreateInfo.flags = 0;
  pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
  pipelineColorBlendStateCreateInfo.logicOp = VkLogicOp::VK_LOGIC_OP_CLEAR;
  pipelineColorBlendStateCreateInfo.attachmentCount = 1;
  pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
  pipelineColorBlendStateCreateInfo.blendConstants;
  pipelineColorBlendStateCreateInfo.blendConstants[0] = 1.0f;
  pipelineColorBlendStateCreateInfo.blendConstants[1] = 1.0f;
  pipelineColorBlendStateCreateInfo.blendConstants[2] = 1.0f;
  pipelineColorBlendStateCreateInfo.blendConstants[3] = 1.0f;

  VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo;

  ZeroObject(pipelineDepthStencilStateCreateInfo);
  pipelineDepthStencilStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  pipelineDepthStencilStateCreateInfo.pNext = nullptr;
  pipelineDepthStencilStateCreateInfo.flags = 0;
  pipelineDepthStencilStateCreateInfo.depthTestEnable = m_depthTest; //VK_TRUE; //VK_FALSE;
  pipelineDepthStencilStateCreateInfo.depthWriteEnable = m_depthWrite; //VK_TRUE; //VK_FALSE;
  pipelineDepthStencilStateCreateInfo.depthCompareOp = m_depthCompare; //VK_COMPARE_OP_LESS_OR_EQUAL; //VK_COMPARE_OP_ALWAYS;
  pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
  pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
  pipelineDepthStencilStateCreateInfo.front.failOp = VkStencilOp::VK_STENCIL_OP_KEEP;
  pipelineDepthStencilStateCreateInfo.front.passOp = VkStencilOp::VK_STENCIL_OP_KEEP;
  pipelineDepthStencilStateCreateInfo.front.depthFailOp = VkStencilOp::VK_STENCIL_OP_KEEP;
  pipelineDepthStencilStateCreateInfo.front.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
  pipelineDepthStencilStateCreateInfo.front.compareMask = 0;
  pipelineDepthStencilStateCreateInfo.front.writeMask = 0;
  pipelineDepthStencilStateCreateInfo.front.reference = 0;
  pipelineDepthStencilStateCreateInfo.back.failOp = VkStencilOp::VK_STENCIL_OP_KEEP;
  pipelineDepthStencilStateCreateInfo.back.passOp = VkStencilOp::VK_STENCIL_OP_KEEP;
  pipelineDepthStencilStateCreateInfo.back.depthFailOp = VkStencilOp::VK_STENCIL_OP_KEEP;
  pipelineDepthStencilStateCreateInfo.back.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
  pipelineDepthStencilStateCreateInfo.back.compareMask = 0;
  pipelineDepthStencilStateCreateInfo.back.writeMask = 0;
  pipelineDepthStencilStateCreateInfo.back.reference = 0;
  pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
  pipelineDepthStencilStateCreateInfo.maxDepthBounds = 0.0f;

  VkDynamicState dynamicStates[4] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
                                      VK_DYNAMIC_STATE_BLEND_CONSTANTS, VK_DYNAMIC_STATE_DEPTH_BOUNDS };

  VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;

  ZeroObject(dynamicStateCreateInfo);
  dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicStateCreateInfo.dynamicStateCount = 4;
  dynamicStateCreateInfo.pDynamicStates = dynamicStates;

  VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;

  ZeroObject(graphicsPipelineCreateInfo);
  graphicsPipelineCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  graphicsPipelineCreateInfo.pNext = nullptr;
  graphicsPipelineCreateInfo.flags = 0;
  graphicsPipelineCreateInfo.stageCount = 2;
  graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfos;
  graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
  graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
  graphicsPipelineCreateInfo.pTessellationState = VK_NULL_HANDLE;
  graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
  graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
  graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
  graphicsPipelineCreateInfo.pDepthStencilState = &pipelineDepthStencilStateCreateInfo;
  graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
  graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
  graphicsPipelineCreateInfo.layout = pipeline->layout;
  graphicsPipelineCreateInfo.renderPass = m_renderPass;
  graphicsPipelineCreateInfo.subpass = 0;
  graphicsPipelineCreateInfo.basePipelineIndex = 0;
  graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

  result = m_vk.vkCreateGraphicsPipelines(m_device, pipeline->cache, 1, &graphicsPipelineCreateInfo,
                                          nullptr, &pipeline->pipeline);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create fragment shader.\n";

    printResult(result);

    freePipeline(pipeline);

    return null;
  }

  pipeline->depthCompare = m_depthCompare;
  pipeline->depthWrite   = m_depthWrite;
  pipeline->depthTest    = m_depthTest;
  pipeline->blending     = m_blending;
  pipeline->bsrc         = m_blendSource;
  pipeline->bdst         = m_blendDestination;
  pipeline->cullMode     = m_cullMode;
  pipeline->cullFace     = m_cullFace;
  pipeline->polygonMode  = m_polygonMode;
  pipeline->topology     = m_topology;
  pipeline->vertexFormat = m_vertexFormat;
  pipeline->shader       = m_shader;
  pipeline->ubo          = m_shader->ubo;

  return pipeline;
}

kgmVulkan::PipelineStatus kgmVulkan::getCurrentPipelineStatus()
{
  PipelineStatus ps;

  ZeroObject(ps);

  ps.shader       = m_shader;
  ps.topology     = m_topology;
  ps.vertexFormat = m_vertexFormat;
  ps.polygonMode  = m_polygonMode;
  ps.cullFace     = m_cullFace;
  ps.cullMode     = m_cullMode;
  ps.depthTest    = m_depthTest;
  ps.depthWrite   = m_depthWrite;
  ps.depthCompare = m_depthCompare;
  ps.blending     = m_blending;
  ps.srcBlend     = m_blendSource;
  ps.dstBlend     = m_blendDestination;

  return ps;
}

void* kgmVulkan::uniformLocation(Shader* s, char* u)
{
  if (!strcmp(u, "g_mView"))
    return &s->ubo.g_mView;
  else if (!strcmp(u, "g_mProj"))
    return &s->ubo.g_mProj;
  else if (!strcmp(u, "g_mTran"))
    return &s->ubo.g_mTran;
  else if (!strcmp(u, "g_vColor"))
    return &s->ubo.g_vColor;
  else if (!strcmp(u, "g_vSpecular"))
    return &s->ubo.g_vSpecular;
  else if (!strcmp(u, "g_vClipPlane"))
    return &s->ubo.g_vClipPlane;
  else if (!strcmp(u, "g_vUp"))
    return &s->ubo.g_vUp;
  else if (!strcmp(u, "g_vEye"))
    return &s->ubo.g_vEye;
  else if (!strcmp(u, "g_vLook"))
    return &s->ubo.g_vLook;
  else if (!strcmp(u, "g_fTime"))
    return &s->ubo.g_fTime;
  else if (!strcmp(u, "g_fShine"))
    return &s->ubo.g_fShine;
  else if (!strcmp(u, "g_fRandom"))
    return &s->ubo.g_fRandom;
  else if (!strcmp(u, "g_fAmbient"))
    return &s->ubo.g_fAmbient;
  else if (!strcmp(u, "g_fLightPower"))
    return &s->ubo.g_fLightPower;
  else if (!strcmp(u, "g_iClipping"))
    return &s->ubo.g_iClipping;

  char lbuf[32];

  if (!strncmp(u, "g_vLight", 8))
  {
    for (u32 i = 0; i < VK_MAX_LIGHTS; i++)
    {
      snprintf(lbuf, 31, "g_vLightPos[%d]", i);

      if (!strcmp(u, lbuf))
        return &s->ubo.g_vLightPos[i];

      snprintf(lbuf, 31, "g_vLightDir[%d]", i);

      if (!strcmp(u, lbuf))
        return &s->ubo.g_vLightDir[i];

      snprintf(lbuf, 31, "g_vLightCol[%d]", i);

      if (!strcmp(u, lbuf))
        return &s->ubo.g_vLightCol[i];
    }
  }

  return nullptr;
}

void  kgmVulkan::clear(Shader* s)
{
  if (s == null || s->pipeline == VK_NULL_HANDLE)
    return;

  m_vk.vkDestroyPipelineLayout(m_device, s->layout, null);
  m_vk.vkDestroyPipelineCache(m_device, s->cache, null);
  m_vk.vkDestroyPipeline(m_device, s->pipeline, null);

  s->pipeline = VK_NULL_HANDLE;
  s->layout = VK_NULL_HANDLE;
  s->cache = VK_NULL_HANDLE;
}

void kgmVulkan::clear(Draw* d)
{
  if(!d->vbo && d->vmemory != VK_NULL_HANDLE)
    m_vk.vkFreeMemory(m_device, d->vmemory, NULL);

  if(!d->vbo && d->vbuffer != VK_NULL_HANDLE)
    m_vk.vkDestroyBuffer(m_device, d->vbuffer, NULL);

  if(!d->vbo && d->imemory != VK_NULL_HANDLE)
    m_vk.vkFreeMemory(m_device, d->imemory, NULL);

  if(!d->vbo && d->ibuffer != VK_NULL_HANDLE)
    m_vk.vkDestroyBuffer(m_device, d->ibuffer, NULL);

  if(d->umemory != VK_NULL_HANDLE)
    m_vk.vkFreeMemory(m_device, d->umemory, NULL);

  if(d->ubuffer != VK_NULL_HANDLE)
    m_vk.vkDestroyBuffer(m_device, d->ubuffer, NULL);

  delete d;
}

void kgmVulkan::clearDraws()
{
  auto i = m_drawGroups.groups.begin();

  while(i != m_drawGroups.groups.end())
  {
    auto j = (*i)->draws.begin();
    int n = 0;
    while(j != (*i)->draws.end())
    {
      clear((*j));
      n++;
      j.next();
    }

    i.next();
  }

  m_drawGroups.clear();
}

void kgmVulkan::printResult(VkResult result)
{
  switch(result)
  {
  case VK_SUCCESS:
    kgm_log() << "Vulkan result: Is success.\n";
  break;
  case VK_NOT_READY:
    kgm_log() << "Vulkan result: Not ready.\n";
  break;
  case VK_TIMEOUT:
    kgm_log() << "Vulkan result: Timeout.\n";
  break;
  case VK_EVENT_SET:
    kgm_log() << "Vulkan result: Event set.\n";
  break;
  case VK_EVENT_RESET:
    kgm_log() << "Vulkan result: Event reset.\n";
  break;
  case VK_INCOMPLETE:
    kgm_log() << "Vulkan result: Incomplete.\n";
  break;
  case VK_ERROR_OUT_OF_HOST_MEMORY:
    kgm_log() << "Vulkan result: Error out of host memory.\n";
  break;
  case VK_ERROR_OUT_OF_DEVICE_MEMORY:
    kgm_log() << "Vulkan result: Error of device memory.\n";
  break;
  case VK_ERROR_INITIALIZATION_FAILED:
    kgm_log() << "Vulkan result: Error initialization failed.\n";
  break;
  case VK_ERROR_DEVICE_LOST:
    kgm_log() << "Vulkan result: Error device lost.\n";
  break;
  case VK_ERROR_MEMORY_MAP_FAILED:
    kgm_log() << "Vulkan result: Error memory map failed.\n";
  break;
  case VK_ERROR_LAYER_NOT_PRESENT:
    kgm_log() << "Vulkan result: Error layer not present.\n";
  break;
  case VK_ERROR_EXTENSION_NOT_PRESENT:
    kgm_log() << "Vulkan result: Error extension not present.\n";
  break;
  case VK_ERROR_FEATURE_NOT_PRESENT:
    kgm_log() << "Vulkan result: Error feature not present.\n";
  break;
  case VK_ERROR_INCOMPATIBLE_DRIVER:
    kgm_log() << "Vulkan result: Error incompatible driver.\n";
  break;
  case VK_ERROR_TOO_MANY_OBJECTS:
    kgm_log() << "Vulkan result: Error too maby objects.\n";
  break;
  case VK_ERROR_FORMAT_NOT_SUPPORTED:
    kgm_log() << "Vulkan result: Error format not supported.\n";
  break;
  case VK_ERROR_FRAGMENTED_POOL:
    kgm_log() << "Vulkan result: Error fragmented pool.\n";
  break;
  case VK_ERROR_SURFACE_LOST_KHR:
    kgm_log() << "Vulkan result: Error surface lost.\n";
  break;
  case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
    kgm_log() << "Vulkan result: Error native window in use.\n";
  break;
  case VK_SUBOPTIMAL_KHR:
    kgm_log() << "Vulkan result: Suboptimal.\n";
  break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    kgm_log() << "Vulkan result: Error out of date.\n";
  break;
  case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
    kgm_log() << "Vulkan result: Error incompatible display.\n";
  break;
  case VK_ERROR_VALIDATION_FAILED_EXT:
    kgm_log() << "Vulkan result: Error validation failed.\n";
  break;
  case VK_ERROR_INVALID_SHADER_NV:
    kgm_log() << "Vulkan result: Error invalid shaders.\n";
  break;
  case VK_ERROR_OUT_OF_POOL_MEMORY:
    kgm_log() << "Vulkan result: Error out of pool memory.\n";
  break;
  default:
    kgm_log() << "Vulkan result: Error unknown(" << (int) result << ").\n";
  }
}

#endif
