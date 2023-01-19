#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>

#include "util.h"

static NSWindow*  _window = nil;
static id<MTLDevice> _device = nil;
static id<MTLLibrary> _library = nil;

void __kgmPrepareMetal()
{
  if (_window != nil)
    return;
  
  NSRect frame = NSMakeRect(0, 0, 640, 480);
  
  _device = MTLCreateSystemDefaultDevice();

  MTKView* view = [[MTKView alloc] initWithFrame:frame  device:_device];
}

int __kgmInitMetal(void* w)
{
  NSWindow* wnd = (NSWindow*) w;

  if (wnd == nil)
    return -1;

  NSSize size = [[wnd contentView] frame].size;
  
  NSRect frame = NSMakeRect(0, 0, size.width, size.height);

  if (_device == nil)
    _device = MTLCreateSystemDefaultDevice();

  MTKView* view = [[MTKView alloc] initWithFrame:frame  device:_device];

  [wnd setContentView:view];
  
  return 0;
}
