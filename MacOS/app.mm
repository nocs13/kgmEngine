#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

static NSAutoreleasePool *__pool = NULL;

void __kgmStartApp()
{
  // create the autorelease pool
  __pool = [[NSAutoreleasePool alloc] init];

  // create the application object 
  NSApp = [NSApplication sharedApplication];
}

void __kgmRunApp()
{
  // run the main event loop
  [NSApp run];
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
