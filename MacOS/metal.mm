#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>

#include "util.h"

static NSWindow*  _window = nil;
static id<MTLDevice> _device = nil;
static id<MTLLibrary> _library = nil;
static id<MTLRenderPipelineState> _pipelineState = nil;
static id<MTLBuffer> _vertexBuffer = nil;
static id<MTLBuffer> _uniformBuffer = nil;
static id<MTLCommandQueue> _commandQueue = nil;

void __kgmPrepareMetal()
{
  NSLog(@"Preparing metal.");
  
  if (_window != nil) {
    NSLog(@"Have window seems metal already initialized.");
    
    return;
  }

  NSError* error = nil;
  
  NSRect frame = NSMakeRect(0, 0, 640, 480);
  
  _device = MTLCreateSystemDefaultDevice();

  MTKView* view = [[MTKView alloc] initWithFrame:frame  device:_device];

  // Metal setup: Library
    
  NSString* librarySrc = [NSString stringWithContentsOfFile:@"shaders/base.metal" encoding:NSUTF8StringEncoding error:&error];
  
  if(!librarySrc) {
    NSLog(@"Failed to read shaders");
    
    [NSException raise:@"Failed to read shaders" format:@"%@", [error localizedDescription]];
  }

  _library = [_device newLibraryWithSource:librarySrc options:nil error:&error];
  
  if(!_library) {
    NSLog(@"Failed to compile shaders");
    
    [NSException raise:@"Failed to compile shaders" format:@"%@", [error localizedDescription]];
  }


  // Metal setup: Pipeline

  id<CAMetalDrawable> drawable = [view currentDrawable];
  MTLRenderPipelineDescriptor* pipelineDesc = [[MTLRenderPipelineDescriptor alloc] init];
  pipelineDesc.vertexFunction = [_library newFunctionWithName:@"vertexFunction"];
  pipelineDesc.fragmentFunction = [_library newFunctionWithName:@"fragmentFunction"];
  pipelineDesc.colorAttachments[0].pixelFormat = drawable.texture.pixelFormat;

  _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineDesc error:&error];
  
  if(!_pipelineState) {
    [NSException raise:@"Failed to create pipeline state" format:@"%@", [error localizedDescription]];
  }

  _commandQueue = [_device newCommandQueue];
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
