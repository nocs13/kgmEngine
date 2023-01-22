#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

void updateFrame();

static NSAutoreleasePool *__pool = NULL;

@interface KTimer : NSObject {
	
	NSTimer *timer;
}
- (void) fire: (NSTimer *) theTimer; 
- (void) begin;
- (void) stop;
@end

@implementation KTimer
-(id) init {
  return self;
}

- (void) fire: (NSTimer *) theTimer {
  updateFrame();
}

- (void) begin {
	
  NSTimeInterval time = 1.0 / 60.0;
  
  if (timer) { 
    [self stop];
  }
	
  timer = [NSTimer timerWithTimeInterval: time 
				  target: self 
				selector: @selector(fire:) 
				userInfo: nil 
				 repeats: YES];
  
  [[NSRunLoop currentRunLoop] addTimer: timer forMode: NSDefaultRunLoopMode];		
  [[NSRunLoop currentRunLoop] addTimer: timer forMode: NSEventTrackingRunLoopMode];
}

- (void) stop {
	
	[timer invalidate]; 
	timer = nil;
}

@end

void __kgmStartApp()
{
  // create the autorelease pool
  __pool = [[NSAutoreleasePool alloc] init];

  // create the application object 
  NSApp = [NSApplication sharedApplication];
}

void __kgmRunApp()
{
  KTimer* t = [[KTimer alloc] autorelease];

  // start frame update timer.
  [t begin];
  
  // run the main event loop
  [NSApp run];
  
  //NSApplicationMain(0, nil);

  [t stop];
}

int __kgmExitApp()
{
  [NSApp release];      // release the app 
  [__pool release];       // release the pool
  
  return(EXIT_SUCCESS);
}

void __kgmLog(const char* log)
{
  NSLog(@"kgmLog: %s.\n", log);
}
