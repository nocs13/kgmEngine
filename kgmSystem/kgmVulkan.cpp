#include "kgmVulkan.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmUtils/kgmBase64.h"
#include "kgmSystem.h"
#include "kgmThread.h"
#include "../kgmBase/kgmIResources.h"
#include "../kgmGame/kgmIGame.h"

#ifdef VULKAN

#include "inc/vk/vulkan.h"
#include "inc/vk/vk_kgm.h"

#define ZeroObject(o) memset(&o, 0, sizeof(typeof o))

//https://gist.github.com/Overv/7ac07356037592a121225172d7d78f2d

u32           kgmVulkan::g_vulkans = 0;
kgmLib        kgmVulkan::vk_lib;
kgmVulkan::vk kgmVulkan::m_vk      = {0};

float cc_color[4] = {1.0, 0.0, 0.0, 1.0};

kgmVulkan::kgmVulkan(kgmWindow* wnd)
{
  m_instance = 0;

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
  //u32 wr[2];
  //kgmSystem::getDesktopDimension(wr[0], wr[1]);
  //m_rect[2] = wr[0];
  //m_rect[3] = wr[1];

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

  if (!initSurface() || !initSwapchain())
  {
    m_error = 1;

    return;
  }

  if (!initRenderPass() || !initImageViews() || !initFrameBuffers())
  {
    m_error = 1;

    return;
  }

  if (!initSemaphores() || !initCommands() || !initFence())
  {
    m_error = 1;

    return;
  }

  fillCommands();

  kgm_log() << "Vulkan: Successfully prepared.\n";
}

kgmVulkan::~kgmVulkan()
{
  if (m_device)
  {
    m_vk.vkDeviceWaitIdle(m_device);

    if (m_commandPool)
    {
      m_vk.vkFreeCommandBuffers(m_device, m_commandPool, (u32) m_commandBuffers.length(), m_commandBuffers.data());

      m_commandBuffers.clear();

      kgm_log() << "Vulkan: Command buffer destroyed.\n";

      m_vk.vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    }


    //m_vk.vkDestroyPipeline(m_device, graphicsPipeline, nullptr);
    if (m_renderPass)
    {
      m_vk.vkDestroyRenderPass(m_device, m_renderPass, nullptr);

      kgm_log() << "Vulkan: Render pass destroyed.\n";
    }

    for (size_t i = 0; i < m_swapChainImages.length(); i++)
    {
      m_vk.vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr);
      m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
    }

    m_swapChainImages.clear();
    m_imageViews.clear();
    m_frameBuffers.clear();


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

  VK_IMPORT_FUNCTION(vkDestroySurfaceKHR);
  VK_IMPORT_FUNCTION(vkDestroyImage);
  VK_IMPORT_FUNCTION(vkMapMemory);
  VK_IMPORT_FUNCTION(vkUnmapMemory);
  VK_IMPORT_FUNCTION(vkCreateGraphicsPipelines);
  VK_IMPORT_FUNCTION(vkDestroyPipeline);
  VK_IMPORT_FUNCTION(vkCreatePipelineLayout);
  VK_IMPORT_FUNCTION(vkDestroyPipelineLayout);
  VK_IMPORT_FUNCTION(vkCreatePipelineCache);
  VK_IMPORT_FUNCTION(vkDestroyPipelineCache);
  VK_IMPORT_FUNCTION(vkCreateBuffer);
  VK_IMPORT_FUNCTION(vkGetBufferMemoryRequirements);
  VK_IMPORT_FUNCTION(vkAllocateMemory);
  VK_IMPORT_FUNCTION(vkBindBufferMemory);
  VK_IMPORT_FUNCTION(vkDestroyBuffer);
  VK_IMPORT_FUNCTION(vkCmdBindVertexBuffers);
  VK_IMPORT_FUNCTION(vkCmdBindPipeline);
  VK_IMPORT_FUNCTION(vkCmdDraw);

  //VK_IMPORT_FUNCTION(vkGetPhysicalDeviceMemoryProperties);

#ifdef WIN32
  m_vk.vkCreateWin32SurfaceKHR = (typeof m_vk.vkCreateWin32SurfaceKHR) vk_lib.get((char*) "vkCreateWin32SurfaceKHR");
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

void  kgmVulkan::gcSet(u32 param, void* value) {}
void  kgmVulkan::gcGet(u32 param, void* value) {}

void  kgmVulkan::gcClear(u32 flag, u32 col, float depth, u32 sten)
{
  u8 r = (col & 0x000000ff);
  u8 g = ((col & 0x0000ff00) >> 8);
  u8 b = ((col & 0x00ff0000) >> 16);
  u8 a = ((col & 0xff000000) >> 24);

  cc_color[0] = (f32) r / 255.f;
  cc_color[1] = (f32) g / 255.f;
  cc_color[2] = (f32) b / 255.f;
  cc_color[3] = (f32) a / 255.f;

  return;

  for (u32 i = 0; i < m_swapChainImages.length(); i++)
  {
    auto &commandBuffer = m_commandBuffers[i];
    auto &image         = m_swapChainImages[i];
    auto &framebuffer   = m_frameBuffers[i];

    VkResult result = m_vk.vkResetCommandBuffer(commandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

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
      continue;
    }

    VkClearValue clearValue;

    ZeroObject(clearValue);

    float ggg = (float) ((float) rand() / (float) RAND_MAX);

    clearValue.color.float32[0] = (f32) r / 255.f;
    clearValue.color.float32[1] = (f32) g / 255.f;
    clearValue.color.float32[2] = (f32) b / 255.f;
    clearValue.color.float32[3] = (f32) a / 255.f;

    VkRenderPassBeginInfo renderPassBeginInfo;

    ZeroObject(renderPassBeginInfo);
    renderPassBeginInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.framebuffer = framebuffer;
    renderPassBeginInfo.renderArea = VkRect2D {
      VkOffset2D  {0, 0},
      VkExtent2D  {
        (u32) m_rect[2], (u32) m_rect[3]
      }
    };

    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;

    m_vk.vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    VkImageMemoryBarrier imageMemoryBarrier;

    ZeroObject(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = nullptr;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                       VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT;
    imageMemoryBarrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.srcQueueFamilyIndex = 0;
    imageMemoryBarrier.dstQueueFamilyIndex = 0;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = {
      VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1
    };

    m_vk.vkCmdPipelineBarrier(commandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, null, 0, null,
                              1, &imageMemoryBarrier);
  }
}

void kgmVulkan::gcResize(u32 width, u32 height)
{
  if (width == m_rect[2] && height == m_rect[3])
    return;

  m_rect[2] = width;
  m_rect[3] = height;

  //kgm_log() << "Vulkan: Window resized, viewport need update.\n";

  //refreshSwapchain();
}

void  kgmVulkan::gcBegin()
{

}

void  kgmVulkan::gcEnd()
{

}

void  kgmVulkan::gcRender()
{
  VkResult result;

  //kgm_log() << "Vulkan info: start render.\n";

  u32 swapChainImage = 0;

  if (!m_device || !m_swapChain)
    return;

  result = m_vk.vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, VK_NULL_HANDLE, m_fence, &swapChainImage);

  auto commandBuffer = m_commandBuffers[swapChainImage];

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to get next swapchain image.\n";

    printResult(result);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
      kgm_log() << "Vulkan render info: Surface incompatible, updating swapchans.\n";

      refreshSwapchain();

      return;
    }

    exit(0);

    return;
  }

  result = m_vk.vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, UINT64_MAX);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to wait for fence.\n";

    printResult(result);

    return;
  }

  result = m_vk.vkResetFences(m_device, 1, &m_fence);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to reset fence.\n";

    printResult(result);

    return;
  }

  VkQueue queue;

  m_vk.vkGetDeviceQueue(m_device, 0, 0, &queue);

  VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

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

  result = m_vk.vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to submit command buffer.\n";

    printResult(result);

    return;
  }

  result = m_vk.vkQueueWaitIdle(queue);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to wait for queue.\n";

    printResult(result);

    exit(0);

    return;
  }

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

  VkResult result2 = m_vk.vkQueuePresentKHR(queue, &presentInfo);

  if (result2 == VK_ERROR_OUT_OF_DATE_KHR)
  {
    kgm_log() << "Vulkan render info: Surface incompatible, updating swapchans.\n";

    //refreshSwapchain();

    return;
  }

  if(result != VkResult::VK_SUCCESS || result2 != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan render error: failed to present swapchain.\n";

    printResult(result);
    printResult(result2);

    exit(0);

    return;
  }

  //kgm_log() << "Vulkan: Queue present passed.\n";

  //clearDraws();

  m_swapChainImage = swapChainImage;
}

void  kgmVulkan::gcSetTarget(void*  rt) {}

// DRAWING
static s32 yyy = 0;
void  kgmVulkan::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt)
{
  Draw mesh = {0};
  if(yyy > 0)
    return;
  yyy=1;
  float source[] = {
    -0.5f, +0.5f,
    +0.5f, +0.5f,
    +0.0f, -0.5f
  };

  v_pnt = source;
  v_cnt = 3;
  v_size = sizeof(float) * 2;


  VkBufferCreateInfo bufferInfo = {};

  ZeroObject(bufferInfo);
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = v_size * v_cnt;
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (!m_device)
    return;

  if (m_vk.vkCreateBuffer(m_device, &bufferInfo, nullptr, &mesh.vbuffer) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create vertex buffer!\n";

    return;
  }

  VkMemoryRequirements memRequirements;

  ZeroObject(memRequirements);
  m_vk.vkGetBufferMemoryRequirements(m_device, mesh.vbuffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {};

  ZeroObject(allocInfo);
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = memoryTypeIndex(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  kgm_log() << "Vulkan: memoryTypeIndex is " << allocInfo.memoryTypeIndex <<  "!\n";

  if (m_vk.vkAllocateMemory(m_device, &allocInfo, nullptr, &mesh.vmemory) != VK_SUCCESS)
  {
      kgm_log() << "Vulkan error: Failed to allocate vertex buffer memory!\n";

      m_vk.vkDestroyBuffer(m_device, mesh.vbuffer, null);

      return;
  }

  void* data;

  if (m_vk.vkMapMemory(m_device, mesh.vmemory, 0, bufferInfo.size, 0, &data) != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to map mamory!\n";

    m_vk.vkDestroyBuffer(m_device, mesh.vbuffer, null);
    m_vk.vkFreeMemory(m_device, mesh.vmemory, null);

    return;
  }

  memcpy(data, v_pnt, (size_t) bufferInfo.size);

  m_vk.vkUnmapMemory(m_device, mesh.vmemory);

  if (m_vk.vkBindBufferMemory(m_device, mesh.vbuffer, mesh.vmemory, 0) != VK_SUCCESS)
  {
    m_vk.vkDestroyBuffer(m_device, mesh.vbuffer, null);
    m_vk.vkFreeMemory(m_device, mesh.vmemory, null);

    kgm_log() << "Vulkan error: Failed bind vertex memory to buffer!\n";

    return;
  }

  if (i_cnt && i_pnt)
  {

  }

  mesh.shader = m_shader;

  mesh.vcnt = v_cnt;
  mesh.icnt = i_cnt;

  m_draws.add(mesh);

  fillCommands();

  kgm_log() << "Vulkan: Mesh add and filled commands.\n";

  return;

  //auto commandBuffer = m_commandBuffers[m_swapChainImage];

  if (m_shader && m_shader->pipeline)
  {
    //m_vk.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_shader->pipeline);

    //VkBuffer vertexBuffers[] = { mesh.vbuffer };
    //VkDeviceSize offsets[] = {0};

    //m_vk.vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    //m_vk.vkCmdDraw(commandBuffer, static_cast<uint32_t>(v_cnt), 1, 0, 0);

    for (u32 i = 0; i < m_swapChainImages.length(); i++)
    {
      auto &commandBuffer = m_commandBuffers[i];
      auto &framebuffer   = m_frameBuffers[i];

      VkRenderPassBeginInfo renderPassBeginInfo;

      ZeroObject(renderPassBeginInfo);
      renderPassBeginInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassBeginInfo.pNext = nullptr;
      renderPassBeginInfo.renderPass = m_renderPass;
      renderPassBeginInfo.framebuffer = framebuffer;
      renderPassBeginInfo.renderArea = VkRect2D{
          VkOffset2D{0, 0},
          VkExtent2D{(u32)m_rect[2], (u32)m_rect[3]}
      };
      renderPassBeginInfo.clearValueCount = 1;
      renderPassBeginInfo.pClearValues = NULL;

      //m_vk.vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo,
      //                          VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

      //m_vk.vkCmdBindPipeline(commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,
      //                       m_shader->pipeline);

      VkBuffer vbuffers[] = {mesh.vbuffer};
      VkDeviceSize dsizes[] = {0};

      m_vk.vkCmdBindVertexBuffers(commandBuffer, 0, 1, vbuffers, dsizes);
      m_vk.vkCmdDraw(commandBuffer, v_cnt, 1, 0, 0);
      //m_vk.vkCmdEndRenderPass(commandBuffer);
    }
  }
}

// TEXTURE
void* kgmVulkan::gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type)
{
  if (!m_device || !w || !h)
    return null;
  return null;

  Texture* t = new Texture;

  t->image = VK_NULL_HANDLE;
  t->iview = VK_NULL_HANDLE;
  t->memory = VK_NULL_HANDLE;

  u32 bypp = 1;

  VkFormat format;

  VkImageType itype;

  switch(bpp)
  {
  case gctex_fmt8:
    format = VK_FORMAT_R8_UNORM;
    bypp = 1;
    break;
  case gctex_fmt16:
    format = VK_FORMAT_R5G6B5_UNORM_PACK16;
    bypp = 2;
    break;
  case gctex_fmt24:
    format = VK_FORMAT_R8G8B8_UNORM;
    bypp = 3;
    break;
  case gctex_fmt32:
    format = VK_FORMAT_R8G8B8A8_UNORM;
    bypp = 4;
    break;
  case gctex_fmtdepth:
    format = VK_FORMAT_D16_UNORM;
    bypp = 2;
    break;
  case gctex_fmtdepten:
    format = VK_FORMAT_D16_UNORM_S8_UINT;
    bypp = 3;
    break;
  default:
    format = VK_FORMAT_R8G8B8A8_UNORM;
    bypp = 4;
  };

  format = VK_FORMAT_R8G8B8A8_UNORM;

  switch (type)
  {
  case gctype_texcube:
    itype = VK_IMAGE_TYPE_3D;
    break;
  case gctype_texdepth:
    itype = VK_IMAGE_TYPE_2D;
    break;
  case gctype_tex2d:
  default:
    itype = VK_IMAGE_TYPE_2D;
    break;
  }

  VkImageCreateInfo imageInfo = {};

  ZeroObject(imageInfo);

  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = itype;
  imageInfo.extent.width = w;
  imageInfo.extent.height = h;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkResult result = m_vk.vkCreateImage(m_device, &imageInfo, nullptr, &t->image);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create texture image.\n";

    printResult(result);

    delete t;

    return null;
  }

  VkMemoryRequirements requirements;

  m_vk.vkGetImageMemoryRequirements(m_device, t->image, &requirements);

  VkMemoryAllocateInfo allocInfo = {};

  ZeroObject(allocInfo);

  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = requirements.size;

  result = m_vk.vkAllocateMemory(m_device, &allocInfo, null, &t->memory);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to allocate texture image memory.\n";

    printResult(result);

    m_vk.vkDestroyImage(m_device, t->image, null);

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

    delete t;

    return null;
  }

  if (m)
  {
    void* data = null;

    u32 size = w * h * bypp;

    result = m_vk.vkMapMemory(m_device, t->memory, 0, size, 0, &data);

    if (result != VK_SUCCESS)
    {
      kgm_log() << "Vulkan error: Failed to fill texture image memory.\n";

      printResult(result);

      m_vk.vkDestroyImage(m_device, t->image, null);

      m_vk.vkFreeMemory(m_device, t->memory, null);

      delete t;

      return null;
    }

    if (bypp == 4)
    {
      memcpy(data, m, size);
    }
    else if (bypp == 3)
    {

    }
    else if (bypp == 2)
    {

    }
    else
    {

    }

    m_vk.vkUnmapMemory(m_device, t->memory);
  }

  VkImageViewCreateInfo viewInfo;

  ZeroObject(viewInfo);

  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.baseArrayLayer = 0;
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

void  kgmVulkan::gcSetTexture(u32 stage, void *t) {}

// TARGET
gchandle kgmVulkan::gcGenTarget(u32 w, u32 h, bool depth, bool stencil){}
bool     kgmVulkan::gcTexTarget(gchandle tar, gchandle tex, u32 type){}
void     kgmVulkan::gcFreeTarget(gchandle t){}

// VIEWPORT
void  kgmVulkan::gcSetViewport(int x, int y, int w, int h, float n, float f)
{
  m_viewport.x = x;
  m_viewport.y = y;
  m_viewport.width = w;
  m_viewport.height = h;
  //m_viewport.minDepth = n;
  //m_viewport.maxDepth = f;
  m_viewport.minDepth = 0.0;
  m_viewport.maxDepth = 1.0;
}

//BLEND
void  kgmVulkan::gcBlend(bool, u32, u32, u32) {}

//ALPHA
void  kgmVulkan::gcAlpha(bool, u32, float) {}

//CULL
void  kgmVulkan::gcCull(u32 mode) {}

//DEPTH
void  kgmVulkan::gcDepth(bool en, bool mask, u32 mode) {}

//LIGHT
void kgmVulkan::gcSetLight(int i, float* pos, float forse, float* col, float* dir, float angle) {}

//VERTEX & INDEX BUFFERS
void* kgmVulkan::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize) {}
void  kgmVulkan::gcFreeVertexBuffer(void*) {}
void  kgmVulkan::gcDrawVertexBuffer(void* buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff) {}

// SHADER
void* kgmVulkan::gcGenShader(kgmMemory<u8>& v, kgmMemory<u8>& f)
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

  if (!createBuffer(sizeof(Shader::uo), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, shader->buffer, shader->memory))
  {
    kgm_log() << "Vulkan error: Cannot create memory buffer for shader.\n";

    m_vk.vkDestroyShaderModule(m_device, shader->vertex, null);
    m_vk.vkDestroyShaderModule(m_device, shader->fragment, null);

    delete shader;

    return null;
  }

  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

  ZeroObject(pipelineLayoutCreateInfo);
  pipelineLayoutCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.pNext = nullptr;
  pipelineLayoutCreateInfo.flags = 0;
  pipelineLayoutCreateInfo.setLayoutCount = 0;
  pipelineLayoutCreateInfo.pSetLayouts = nullptr;
  pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
  pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

  result = m_vk.vkCreatePipelineLayout(m_device, &pipelineLayoutCreateInfo, nullptr, &shader->layout);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create fragment shader.\n";

    printResult(result);

    m_vk.vkDestroyShaderModule(m_device, shader->vertex, null);
    m_vk.vkDestroyShaderModule(m_device, shader->fragment, null);

    delete shader;

    return null;
  }

  VkPipelineCacheCreateInfo pipelineCacheCreateInfo;

  ZeroObject(pipelineCacheCreateInfo);
  pipelineCacheCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  pipelineCacheCreateInfo.pNext = nullptr;
  pipelineCacheCreateInfo.flags = 0;
  pipelineCacheCreateInfo.initialDataSize = 0;
  pipelineCacheCreateInfo.pInitialData = nullptr;

  result = m_vk.vkCreatePipelineCache(m_device, &pipelineCacheCreateInfo, nullptr, &shader->cache);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create fragment shader.\n";

    printResult(result);

    m_vk.vkDestroyPipelineLayout(m_device, shader->layout, null);
    m_vk.vkDestroyShaderModule(m_device, shader->vertex, null);
    m_vk.vkDestroyShaderModule(m_device, shader->fragment, null);

    delete shader;

    return null;
  }

  VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2];

  ZeroObject(pipelineShaderStageCreateInfos[0]);
  pipelineShaderStageCreateInfos[0].sType  = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  pipelineShaderStageCreateInfos[0].pNext  = nullptr;
  pipelineShaderStageCreateInfos[0].flags  = 0;
  pipelineShaderStageCreateInfos[0].stage  = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
  pipelineShaderStageCreateInfos[0].module =  shader->vertex;
  pipelineShaderStageCreateInfos[0].pName = "main";
  pipelineShaderStageCreateInfos[0].pSpecializationInfo = nullptr;

  ZeroObject(pipelineShaderStageCreateInfos[1]);
  pipelineShaderStageCreateInfos[1].sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  pipelineShaderStageCreateInfos[1].pNext = nullptr;
  pipelineShaderStageCreateInfos[1].flags = 0;
  pipelineShaderStageCreateInfos[1].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
  pipelineShaderStageCreateInfos[1].module = shader->fragment;
  pipelineShaderStageCreateInfos[1].pName = "main";
  pipelineShaderStageCreateInfos[1].pSpecializationInfo = nullptr;

  VkVertexInputBindingDescription vertexInputBindingDescription;

  ZeroObject(vertexInputBindingDescription);
  vertexInputBindingDescription.binding = 0;
  vertexInputBindingDescription.stride = sizeof(float) * 2;
  vertexInputBindingDescription.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;

  VkVertexInputAttributeDescription vertexInputAttributeDescription;

  ZeroObject(vertexInputAttributeDescription);
  vertexInputAttributeDescription.location = 0;
  vertexInputAttributeDescription.binding = 0;
  vertexInputAttributeDescription.format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
  vertexInputAttributeDescription.offset = 0;

  VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;

  ZeroObject(pipelineVertexInputStateCreateInfo);
  pipelineVertexInputStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  pipelineVertexInputStateCreateInfo.pNext = nullptr;
  pipelineVertexInputStateCreateInfo.flags = 0;
  pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
  pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
  pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 1;
  pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = &vertexInputAttributeDescription;

  VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;

  ZeroObject(pipelineInputAssemblyStateCreateInfo);
  pipelineInputAssemblyStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
  pipelineInputAssemblyStateCreateInfo.flags = 0;
  pipelineInputAssemblyStateCreateInfo.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
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
  pipelineRasterizationStateCreateInfo.polygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
  pipelineRasterizationStateCreateInfo.cullMode = VkCullModeFlagBits::VK_CULL_MODE_NONE;
  pipelineRasterizationStateCreateInfo.frontFace = VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
  pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
  pipelineColorBlendAttachmentState.srcColorBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
  pipelineColorBlendAttachmentState.dstColorBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
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
  pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_FALSE;
  pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_FALSE;
  pipelineDepthStencilStateCreateInfo.depthCompareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
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
  graphicsPipelineCreateInfo.pDynamicState = nullptr;
  graphicsPipelineCreateInfo.layout = shader->layout;
  graphicsPipelineCreateInfo.renderPass = m_renderPass;
  graphicsPipelineCreateInfo.subpass = 0;
  graphicsPipelineCreateInfo.basePipelineIndex = 0;
  graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

  result = m_vk.vkCreateGraphicsPipelines(m_device, shader->cache, 1, &graphicsPipelineCreateInfo,
                                          nullptr, &shader->pipeline);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot create fragment shader.\n";

    printResult(result);

    m_vk.vkDestroyPipelineCache(m_device, shader->cache, null);
    m_vk.vkDestroyPipelineLayout(m_device, shader->layout, null);
    m_vk.vkDestroyShaderModule(m_device, shader->vertex, null);
    m_vk.vkDestroyShaderModule(m_device, shader->fragment, null);

    delete shader;

    return null;
  }

  if (!m_shader)
    m_shader = shader;

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
  {
    m_vk.vkDestroyShaderModule(m_device, ((Shader*)s)->vertex, null);
  }

  if (shader->fragment)
  {
    m_vk.vkDestroyShaderModule(m_device, ((Shader*)s)->fragment, null);
  }

  m_vk.vkDestroyPipelineCache(m_device, shader->cache, null);
  m_vk.vkDestroyPipelineLayout(m_device, shader->layout, null);
  m_vk.vkDestroyPipeline(m_device, shader->pipeline, null);

  delete (Shader *) s;
}

void  kgmVulkan::gcSetShader(void* s)
{
  Shader* shader = (Shader*) s;

  if (!s)
    return;

  if (shader == null)
  {
    clear(shader);

    m_shader = null;

    return;
  }

  VkResult result = VK_SUCCESS;

  VkDeviceSize uosize = sizeof(Shader::uo);

  void* data = null;

  m_vk.vkMapMemory(m_device, shader->memory, 0, uosize, 0, &data);
  memcpy(data, &shader->uo, uosize);
  m_vk.vkUnmapMemory(m_device, shader->memory);

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
  if(m_instance != nullptr)
  {
    kgm_log() << "Vulkan error: Instance already initialized.\n";

    return false;
  }

  u32 extensionsCount = 0;

  VkResult result = m_vk.vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

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
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);;
  appInfo.pEngineName = "kgmEngine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);;
  appInfo.apiVersion = VK_API_VERSION_1_0;

  const char* layerNames[] = {
    "VK_LAYER_LUNARG_standard_validation",
    //"VK_LAYER_LUNARG_api_dump",
    //"VK_LAYER_LUNARG_core_validation",
    //"VK_LAYER_LUNARG_image",
    //"VK_LAYER_LUNARG_object_tracker",
    //"VK_LAYER_LUNARG_parameter_validation",
    //"VK_LAYER_LUNARG_screenshot",
    //"VK_LAYER_LUNARG_swapchain",
    //"VK_LAYER_GOOGLE_threading",
    //"VK_LAYER_GOOGLE_unique_objects",
    "VK_LAYER_LUNARG_vktrace",
    //"VK_LAYER_RENDERDOC_Capture",
    //"VK_LAYER_NV_optimus",
    //"VK_LAYER_VALVE_steam_overlay",
  };

  VkInstanceCreateInfo instanceInfo = {};

  ZeroObject(instanceInfo);

  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext = NULL;
  instanceInfo.flags = 0;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledLayerCount = 2;
  instanceInfo.ppEnabledLayerNames = layerNames;
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

#endif

bool kgmVulkan::initDebug()
{
#ifdef DEBUG

  m_vk.vkCreateDebugReportCallbackEXT = (typeof m_vk.vkCreateDebugReportCallbackEXT) m_vk.vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");;

  VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;

  debugReportCallbackCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  debugReportCallbackCreateInfo.pNext = nullptr;
  debugReportCallbackCreateInfo.flags = VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT
          | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT;
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

  m_physicalDevice = m_physicalDevices[0];

  kgm_log() << "Vulkan: Physical device choosen\n";

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

#ifdef DEBUG
  const char* layerNames[] = {
    "VK_LAYER_NV_optimus",
    "VK_LAYER_LUNARG_api_dump",
    "VK_LAYER_LUNARG_screenshot",
    "VK_LAYER_RENDERDOC_Capture",
    "VK_LAYER_VALVE_steam_overlay",
    "VK_LAYER_LUNARG_standard_validation",
  };
  //m_debugLayer = "VK_LAYER_LUNARG_standard_validation";
  //m_debugLayer = "VK_LAYER_KHRONOS_validation";

  infoDevice.enabledLayerCount = 6;
  infoDevice.ppEnabledLayerNames = layerNames;
#endif

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
    createInfo.surface = m_window->m_window;
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
  infoCommand.commandBufferCount = 2;

  m_commandBuffers.alloc(2);

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

  VkSurfaceCapabilitiesKHR surfaceCapabilities;

  result = m_vk.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilities);

  if(result != VkResult::VK_SUCCESS)
  {
    kgm_log() << "kgmVulkan error: failed to get surface capabilities.\n";

    printResult(result);

    return false;
  }

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

  u32 presentModeCount;

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

  VkExtent2D swapChainExtent = surfaceCapabilities.currentExtent;

  VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

  u32 swapChainImagesCount = 2;

  VkSurfaceTransformFlagBitsKHR surfaceTransform;

  if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
  {
    surfaceTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }
  else
  {
    surfaceTransform = surfaceCapabilities.currentTransform;
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
    if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i])
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
  //swapchainCreateInfo.imageExtent.width = swapChainExtent.width;
  //swapchainCreateInfo.imageExtent.height = swapChainExtent.height;
  swapchainCreateInfo.imageExtent.width = m_rect[2];
  swapchainCreateInfo.imageExtent.height = m_rect[3];
  swapchainCreateInfo.preTransform = surfaceTransform;
  swapchainCreateInfo.compositeAlpha = compositeAlpha;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.presentMode = presentMode;
  swapchainCreateInfo.clipped = true;
  swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = NULL;

  u32 queueFamilyIndices[2] = {m_graphicsQueueFamilyIndex, m_presentQueueFamilyIndex};

  if (m_graphicsQueueFamilyIndex != m_presentQueueFamilyIndex)
  {
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchainCreateInfo.queueFamilyIndexCount = 2;
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  }

  VkSwapchainKHR swapChain = m_swapChain;

  m_swapChain = nullptr;

  swapchainCreateInfo.oldSwapchain = swapChain;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.imageUsage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = nullptr;
  swapchainCreateInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  swapchainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainCreateInfo.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
  swapchainCreateInfo.clipped = VK_TRUE;

  m_vk.vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapChain);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to create swap chain.\n";

    return false;
  }

  kgm_log() << "Vulkan: Created swapchain.\n";

  uint32_t actualImageCount = swapChainImagesCount;

  result = m_vk.vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, nullptr);

  if (result != VK_SUCCESS || actualImageCount == 0)
  {
    kgm_log() << "Vulkan: failed to acquire number of swap chain images.\n";

    return false;
  }
  kgm_log() << "Vulkan: Got swapchain images count.\n";
  if (swapChain != VK_NULL_HANDLE)
  {
    m_vk.vkDestroySwapchainKHR(m_device, swapChain, nullptr);

    swapChain = nullptr;
  }

  m_swapChainImages.alloc(actualImageCount);

  result = m_vk.vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, m_swapChainImages);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Failed to acquire swap chain images.\n";

    return false;
  }

  kgm_log() << "Vulkan: Got swapchain images.\n";

  m_swapChainImage = 0;

  return true;
}

bool kgmVulkan::initRenderPass()
{
  VkAttachmentDescription attachmentDescription = {};

  ZeroObject(attachmentDescription);

  attachmentDescription.format = m_swapChainFormat;
  attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
  attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentReference = {};

  ZeroObject(colorAttachmentReference);

  colorAttachmentReference.attachment = 0;
  colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subPassDescription = {};

  ZeroObject(subPassDescription);

  subPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subPassDescription.colorAttachmentCount = 1;
  subPassDescription.pColorAttachments = &colorAttachmentReference;

  VkRenderPassCreateInfo createInfo = {};

  ZeroObject(createInfo);

  createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  createInfo.attachmentCount = 1;
  createInfo.pAttachments = &attachmentDescription;
  createInfo.subpassCount = 1;
  createInfo.pSubpasses = &subPassDescription;

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

    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = m_renderPass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &m_imageViews[i];
    createInfo.width = m_rect[2];
    createInfo.height = m_rect[3];
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
  VkImageCreateInfo infoImage = {};

  const VkFormat format = VK_FORMAT_D16_UNORM;

  VkFormatProperties properties;

  m_vk.vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &properties);

  if (properties.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
  {
    infoImage.tiling = VK_IMAGE_TILING_LINEAR;
  }
  else if (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
  {
    infoImage.tiling = VK_IMAGE_TILING_OPTIMAL;
  }
  else
  {
    // Try other depth formats?
    kgm_log() << "Vulkan error: VK_FORMAT_D16_UNORM Unsupported.\n";

    return false;
  }

  infoImage.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  infoImage.pNext = NULL;
  infoImage.imageType = VK_IMAGE_TYPE_2D;
  infoImage.format = format;
  infoImage.extent.width = m_rect[2];
  infoImage.extent.height = m_rect[3];
  infoImage.extent.depth = 1;
  infoImage.mipLevels = 1;
  infoImage.arrayLayers = 1;
  infoImage.samples = VK_SAMPLE_COUNT_16_BIT;
  infoImage.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  infoImage.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  infoImage.queueFamilyIndexCount = 0;
  infoImage.pQueueFamilyIndices = NULL;
  infoImage.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  infoImage.flags = 0;

  VkMemoryAllocateInfo alloc = {};

  alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc.pNext = NULL;
  alloc.allocationSize = 0;
  alloc.memoryTypeIndex = 0;

  VkImageViewCreateInfo infoView = {};
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

  alloc.allocationSize = requirements.size;

  result = m_vk.vkAllocateMemory(m_device, &alloc, NULL, &m_depthMemory);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot allocate memory for depth image.\n";

    printResult(result);

    return false;
  }

  result = m_vk.vkBindImageMemory(m_device, m_depthImage, m_depthMemory, 0);

  if (result != VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: Cannot bind memory to depth image.\n";

    printResult(result);

    return false;
  }

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
    m_vk.vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr);
    m_vk.vkDestroyImageView(m_device, m_imageViews[i], nullptr);
  }

  m_frameBuffers.clear();
  m_imageViews.clear();
  m_swapChainImages.clear();

  kgm_log() << "Vulkan: Framebuffers/Imageviews are destroyed.\n";

  //m_vk.vkDestroyDescriptorSetLayout(m_device, descriptorSetLayout, nullptr);

  initSwapchain();
  initRenderPass();
  initImageViews();
  initFrameBuffers();
  initCommands();

  fillCommands();

  return true;
}

bool kgmVulkan::initFence()
{
  VkFence fence;

  VkFenceCreateInfo fenceCreateInfo;

  ZeroObject(fenceCreateInfo);

  fenceCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.pNext = nullptr;
  fenceCreateInfo.flags = 0;

  if (m_vk.vkCreateFence(m_device, &fenceCreateInfo, nullptr, &fence)
      != VkResult::VK_SUCCESS)
  {
    kgm_log() << "Vulkan error: failed to create fence.\n";

    return false;
  }

  kgm_log() << "Vulkan: Created fence.\n";

  m_fence = fence;

  return true;
}

void kgmVulkan::fillCommands()
{
  VkResult result = VK_SUCCESS;

  if (!m_shader)
    return;

  for (s32 i = 0; i < m_swapChainImages.length(); i++)
  {
    auto &commandBuffer = m_commandBuffers[i];
    auto &image         = m_swapChainImages[i];
    auto &framebuffer   = m_frameBuffers[i];

    result = m_vk.vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

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

    VkClearValue clearValue;

    ZeroObject(clearValue);

    clearValue.color.float32[0] = 0.1;
    clearValue.color.float32[1] = 0.2;
    clearValue.color.float32[2] = 0.5;
    clearValue.color.float32[3] = 1.0;

    VkRenderPassBeginInfo renderPassBeginInfo;

    ZeroObject(renderPassBeginInfo);
    renderPassBeginInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.framebuffer = framebuffer;
    renderPassBeginInfo.renderArea = VkRect2D {
      VkOffset2D  {0, 0},
      VkExtent2D  { (u32) m_rect[2], (u32) m_rect[3] }
    };

    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;

    m_vk.vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    m_vk.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_shader->pipeline);

    kgmList<Draw>::iterator mi;

    for (mi = m_draws.begin(); !mi.end(); mi.next())
    {
      VkBuffer vertexBuffers[] = {(*mi).vbuffer};
      VkDeviceSize offsets[] = {0};

      m_vk.vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
      m_vk.vkCmdDraw(commandBuffer, static_cast<uint32_t>((*mi).vcnt), 1, 0, 0);

      break;
    }

    m_vk.vkCmdEndRenderPass(commandBuffer);

    VkImageMemoryBarrier imageMemoryBarrier;

    ZeroObject(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = nullptr;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                       VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT;
    imageMemoryBarrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.srcQueueFamilyIndex = 0;
    imageMemoryBarrier.dstQueueFamilyIndex = 0;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

    m_vk.vkCmdPipelineBarrier(commandBuffer,
                              VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              0, 0, null, 0, null, 1, &imageMemoryBarrier);

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

void* kgmVulkan::uniformLocation(Shader* s, char* u)
{
  if (!strcmp(u, "g_mView"))
    return &s->uo.g_mView;
  else if (!strcmp(u, "g_mProj"))
    return &s->uo.g_mProj;
  else if (!strcmp(u, "g_mTran"))
    return &s->uo.g_mTran;
  else if (!strcmp(u, "g_vColor"))
    return &s->uo.g_vColor;
  else if (!strcmp(u, "g_vSpecular"))
    return &s->uo.g_vSpecular;
  else if (!strcmp(u, "g_vLight"))
    return &s->uo.g_vLight;
  else if (!strcmp(u, "g_vLightColor"))
    return &s->uo.g_vLightColor;
  else if (!strcmp(u, "g_vLightDirection"))
    return &s->uo.g_vLightDirection;
  else if (!strcmp(u, "g_vClipPlane"))
    return &s->uo.g_vClipPlane;
  else if (!strcmp(u, "g_vUp"))
    return &s->uo.g_vUp;
  else if (!strcmp(u, "g_vEye"))
    return &s->uo.g_vEye;
  else if (!strcmp(u, "g_vLook"))
    return &s->uo.g_vLook;
  else if (!strcmp(u, "g_fTime"))
    return &s->uo.g_fTime;
  else if (!strcmp(u, "g_fShine"))
    return &s->uo.g_fShine;
  else if (!strcmp(u, "g_fRandom"))
    return &s->uo.g_fRandom;
  else if (!strcmp(u, "g_fAmbient"))
    return &s->uo.g_fAmbient;
  else if (!strcmp(u, "g_fLightPower"))
    return &s->uo.g_fLightPower;
  else if (!strcmp(u, "g_iClipping"))
    return &s->uo.g_iClipping;

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

void kgmVulkan::clearDraws()
{

  kgmList<Draw>::iterator i = m_draws.begin();

  for(; !i.end(); i.next())
  {
    if((*i).vmemory != VK_NULL_HANDLE)
      m_vk.vkFreeMemory(m_device, (*i).vmemory, NULL);

    if((*i).vbuffer != VK_NULL_HANDLE)
      m_vk.vkDestroyBuffer(m_device, (*i).vbuffer, NULL);

    if((*i).imemory != VK_NULL_HANDLE)
      m_vk.vkFreeMemory(m_device, (*i).imemory, NULL);

    if((*i).ibuffer != VK_NULL_HANDLE)
      m_vk.vkDestroyBuffer(m_device, (*i).ibuffer, NULL);
  }

  m_draws.clear();
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
  default:
    kgm_log() << "Vulkan result: Error unknown.\n";
  }
}

#endif
