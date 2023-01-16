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
NSMiniaturizableWindowMask: NSWindowStyleMaskMinia[Oturizable
NSMiniControlSize: NSControlSizeMini
NSMouseEntered: NSEventTypeMouseEntered
NSMouseExited: NSEventTypeMouseExited
NSMouseMoved: NSEventTypeMouseMoved
*/

NSWindow* __mainWnd = nil;

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

@interface KWcontrol: NSObject<NSApplicationDelegate, NSWindowDelegate>
{
}
@end

@implementation KWcontrol         // implementation of window control class
-(void)windowWillClose:(NSNotification *)notification
{
    //[NSApp terminate:self];
    [NSApp stop:self];
}
@end

void* __kgmOpenMainWindow(const char* title, int x, int y, int w, int h)
{
  if (__mainWnd != nil)
    return __mainWnd;
  
  NSRect frame = NSMakeRect(x, y, w, h);
  NSWindow* wnd = [ [NSWindow alloc]              // create the window
		    initWithContentRect: frame
                    styleMask:NSWindowStyleMaskTitled 
                    |NSWindowStyleMaskClosable
		    |NSWindowStyleMaskMiniaturizable
                    |NSWindowStyleMaskResizable						  
                    backing:NSBackingStoreBuffered
                    defer:NO ];
  NSString* str = [NSString stringWithUTF8String:title];
  [wnd setTitle:str];
  [wnd setBackgroundColor:[NSColor blueColor]];

  // create amd initialize the DemoView instance
  NSView* view = [[[KView alloc] initWithFrame:frame] autorelease];

  [wnd setContentView:view ];    // set window's view

  KWcontrol* cwnd = [[[KWcontrol alloc] init] autorelease];
  [wnd setDelegate:cwnd ];       // set window's delegate
  
  [wnd makeKeyAndOrderFront:nil];
  [wnd setIsVisible:YES];

  __mainWnd = wnd;
  
  return (void*) wnd;
}


void __kgmAlert(const char* msg)
{
  NSAlert* alert = [[[NSAlert alloc] init] autorelease];
  NSString* str = [[NSString stringWithUTF8String:msg] autorelease];
  [alert setMessageText:str];
  [alert runModal];
}
