#include "kgmSystem.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef LINUX
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <dirent.h>
 #include <X11/Xlib.h>
 #include <X11/keysym.h>
 #include <X11/extensions/xf86vmode.h>
#endif

void kgmSystem::sleep(uint ms){
#ifdef WIN32
  Sleep(ms);
#elif defined LINUX
  usleep(ms);
#endif
}


void kgmSystem::getDesktopDimension(uint& w, uint& h){
#ifdef WIN32
 w = GetDeviceCaps(GetDC(GetDesktopWindow()), HORZRES);
 h = GetDeviceCaps(GetDC(GetDesktopWindow()), VERTRES);
#endif
#ifdef LINUX
 Display* dpy = XOpenDisplay(0);
 int   screen = DefaultScreen(dpy);
 Window  root = RootWindow(dpy, screen);
 XWindowAttributes attr;
 XGetWindowAttributes(dpy, root, &attr);
 
 w = attr.width; h = attr.height;
 XCloseDisplay(dpy);
#endif
}

void kgmSystem::getCurrentDirectory(kgmString& s){
 kgmMemory<char> buf(256);    
 buf.zero();
#ifdef WIN32
 GetCurrentDirectory(256, buf);
#endif
#ifdef LINUX
 getcwd(buf, 256);
#endif
 s = buf;
}

void kgmSystem::setCurrentDirectory(kgmString& s){
 kgmMemory<char> buf(256);    
 buf.zero();
#ifdef WIN32
 SetCurrentDirectory(s);
#endif
#ifdef LINUX
 chdir(s);
#endif
 s = buf;
}

void kgmSystem::getTemporaryDirectory(kgmString& s){
 bool res = false;
 kgmMemory<char> buf(256);    
 buf.zero();
#ifdef WIN32
 DWORD dw = GetTempPath(MAX_PATH, buf);
 if(dw != NULL)
     s = buf;
#endif
#ifdef LINUX
 DIR* dir = opendir("~/tmp");
 if(dir != 0){
  closedir(dir);
  s = (char*)"~/tmp";
 }
#endif
}

bool kgmSystem::isFile(kgmString& s){
 FILE* file;
 if(!(file = fopen(s, "r")))
  return false;
 fclose(file);
 return true;
}

bool kgmSystem::isDirectory(kgmString& s){
#ifdef WIN32
#endif
#ifdef LINUX
 DIR* dir = opendir(s.data());
 if(dir){
  closedir(dir);
  return true;
 }
#endif
 return false;
}
