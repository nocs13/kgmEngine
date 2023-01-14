#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include "utils.h"

/*
NSAlphaShiftKeyMask: NSEventModifierFlagCapsLock
NSAlternateKeyMask: NSEventModifierFlagOption
NSAnyEventMask: NSEventMaskAny
NSApplicationDefined: NSEventTypeApplicationDefined
NSBorderlessWindowMask: NSWindowStyleMaskBorderless
NSCenterTextAlignment: NSTextAlignmentCenter
NSClosableWindowMask: NSWindowStyleMaskClosable
NSCommandKeyMask: NSEventModifierFlagCommand
NSCompositeCopy: NSCompositingOperationCopy
NSControlKeyMask: NSEventModifierFlagControl
NSDeviceIndependentModifierFlagsMask: NSEventModifierFlagDeviceIndependentFlagsMask
NSFlagsChanged: NSEventTypeFlagsChanged
NSFullScreenWindowMask: NSWindowStyleMaskFullScreen
NSFullSizeContentViewWindowMask: NSWindowStyleMaskFullSizeContentView
NSFunctionKeyMask: NSEventModifierFlagFunction
NSHelpKeyMask: NSEventModifierFlagHelp
NSKeyDown: NSEventTypeKeyDown
NSKeyUp: NSEventTypeKeyUp
NSLeftMouseDown: NSEventTypeLeftMouseDown
NSLeftMouseDownMask: NSEventMaskLeftMouseDown
NSLeftMouseDragged: NSEventTypeLeftMouseDragged
NSLeftMouseUp: NSEventTypeLeftMouseUp
NSMiniaturizableWindowMask: NSWindowStyleMaskMiniaturizable
NSMiniControlSize: NSControlSizeMini
NSMouseEntered: NSEventTypeMouseEntered
NSMouseExited: NSEventTypeMouseExited
NSMouseMoved: NSEventTypeMouseMoved
*/

@interface KView : NSView    // interface of KView class
{                               // (subclass of NSView class)
}
- (void)drawRect:(NSRect)rect;  // instance method interface
@end

@implementation KView        // implementation of KView class
- (void)drawRect:(NSRect)rect   // instance method implementation
{
}
-(void)windowWillClose:(NSNotification *)notification
{
    [NSApp terminate:self];
}
@end

void* __kgmOpenWindow(const char* title, int x, int y, int w, int h)
{
  NSRect frame = NSMakeRect(0, 0, 200, 200);
//  NSWindow* wnd  = [[[NSWindow alloc] initWithContentRect:frame
//                      styleMask:NSBorderlessWindowMask
//                      backing:NSBackingStoreBuffered
//                      defer:NO] autorelease];
  NSWindow* wnd = [ [NSWindow alloc]              // create the window
		    initWithContentRect: frame
                    styleMask:NSWindowStyleMaskTitled 
                    |NSWindowStyleMaskClosable
		    |NSWindowStyleMaskMiniaturizable
                    backing:NSBackingStoreBuffered
                    defer:NO ];
  NSString* str = [NSString stringWithUTF8String:title];
  [wnd setTitle:str];
  [wnd setBackgroundColor:[NSColor blueColor]];

  // create amd initialize the DemoView instance
  NSView* view = [[[KView alloc] initWithFrame:frame] autorelease];

  [wnd setContentView:view ];    // set window's view
  //[wnd setDelegate:view ];       // set window's delegate  
  [wnd makeKeyAndOrderFront:NSApp];
  [wnd setIsVisible:true];
  
  return (void*) wnd;
}
