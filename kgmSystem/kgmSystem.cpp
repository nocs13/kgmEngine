#include "kgmSystem.h"
#include "../kgmBase/kgmList.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#include <sys/stat.h>
#endif

#ifdef LINUX
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#endif

#ifdef ANDROID
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & _S_IFMT) == _S_IFDIR)
#endif

#ifdef WIN32
  static const char delim = '\\';
#else
  static const char delim = '/';
#endif

#ifdef WIN32
  const char* kgmSystem::PathDelim = "\\";
  const int   kgmSystem::PathDelimSym = '\\';
#else
  const char* kgmSystem::PathDelim = "/";
  const int   kgmSystem::PathDelimSym = '/';
#endif


void kgmSystem::sleep(u32 ms)
{
#ifdef WIN32
  Sleep(ms);
#else
  usleep(ms * 1000);
#endif
}

void kgmSystem::getDesktopDimension(u32& w, u32& h)
{
#ifdef WIN32
  HWND desktop = GetDesktopWindow();
  HDC  hdc     = GetDC(desktop);
  w = GetDeviceCaps(hdc, HORZRES);
  h = GetDeviceCaps(hdc, VERTRES);
  ReleaseDC(desktop, hdc);
  CloseHandle(desktop);
#endif

#ifdef LINUX
  Display* dpy = XOpenDisplay(0);
  int   screen = DefaultScreen(dpy);
  Window  root = RootWindow(dpy, screen);
  XWindowAttributes attr;
  XGetWindowAttributes(dpy, root, &attr);

  w = attr.width;
  h = attr.height;
  XCloseDisplay(dpy);
#endif

#ifdef ANDROID
#endif
}

void kgmSystem::getCurrentDirectory(kgmString& s)
{
  kgmMemory<u8> buf(1024);
  buf.zero();

#ifdef WIN32
  GetCurrentDirectory(1024, (LPSTR)buf.data());
#else
  getcwd((s8*)buf.data(), 1024);
#endif

  s = (const char*)buf.data();
}

void kgmSystem::setCurrentDirectory(kgmString& s)
{
  kgmMemory<u8> buf(256);
  buf.zero();
#ifdef WIN32
  SetCurrentDirectory(s);
#endif
#ifdef LINUX
  chdir(s);
#endif
  s = (const char*)buf.data();
}

void kgmSystem::getTemporaryDirectory(kgmString& s)
{
#ifdef WIN32
  char buf[MAX_PATH] = {0};

  DWORD dw = GetTempPath(MAX_PATH, (LPSTR)buf);

  if(dw != 0)
    s = (const char*)buf;
#else
  DIR* dir = opendir("~/tmp");

  if(dir != 0)
  {
    closedir(dir);
    s = (char*)"~/tmp";
  }
  else
  {
     s = "/tmp";
  }
#endif
}

void kgmSystem::getHomeDirectory(kgmString& s){
#ifdef WIN32
  if(getenv("HOMEPATH"))
    s = getenv("HOMEPATH");
  else
    s = getenv("HOME");
#endif
#ifdef LINUX
  struct passwd *pw = getpwuid(getuid());
  s = pw->pw_dir;
#endif
}

void kgmSystem::getPathDelim(kgmString &s)
{
#ifdef WIN32
  s = "\\";
#else
  s = "/";
#endif
}

kgmString kgmSystem::getPathDelim()
{
  kgmString s;

#ifdef WIN32
  s = "\\";
#else
  s = "/";
#endif

  return s;
}

bool kgmSystem::isFile(kgmString& s)
{
  struct stat file_stat;

  if(stat(s, &file_stat) != 0)
    return false;

  if(S_ISREG(file_stat.st_mode))
    return true;

  return false;
}

bool kgmSystem::isDirectory(kgmString& s)
{
  struct stat file_stat;

  if(stat(s, &file_stat) != 0)
    return false;

  if(S_ISDIR(file_stat.st_mode))
    return true;

  return false;
}

bool kgmSystem::splitPath(kgmString path, kgmString& dir, kgmString& file)
{
  s8* sym = strrchr(path.data(), delim);

  if(sym != null)
  {
    if(isFile(path))
    {
      file = (++sym);
      dir.alloc(path.data(), u32(sym - path.data() - 1));

      return true;
    }
    else
    {
      dir = path;

      return false;
    }
  }
  else
  {
    if(isFile(path))
      file = path;
    else
      dir = path;\

    return false;
  }

  return false;
}

int kgmSystem::getProcessId()
{
#ifdef WIN32
  return (int) (size_t) GetModuleHandle(NULL);
#else
  return (int)getpid();
#endif
}

int kgmSystem::getProcessPath(kgmString &path)
{
  char c_path[1024] = {0};

#ifdef WIN32
  GetModuleFileName(NULL, c_path, sizeof(c_path));
  path = c_path;

  return (int)path.length();
#else
  readlink("/proc/self/exe", c_path, sizeof(c_path));
  path = c_path;

  return (int)path.length();
#endif

  return 0;
}
/*
#ifdef _WIN32
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

int getNumCores() {
#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif MACOS
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}*/
int kgmSystem::getCpuConcurrency()
{
  int num = 1;

#ifdef WIN32

  SYSTEM_INFO info;
  GetSystemInfo(&info);

  num = (int) info.dwNumberOfProcessors;
#else

#ifdef _SC_NPROCESSORS_ONLN

  num = sysconf(_SC_NPROCESSORS_ONLN);

  if (num > 0)
    return num;
  else
    fprintf(stderr, "Could not determine number of CPUs online: %d\n", num);

#endif

  num = sysconf(_SC_NPROCESSORS_CONF);

#endif

  return num;
}

bool kgmSystem::getDesktopWorkaround(u32 &x, u32 &y, u32 &w, u32 &h)
{
  bool result = true;

#ifdef WIN32
#else
  Display *display;
  Atom     actual_type;
  u32      actual_format;
  u32      nitems;
  u32      bytes;
  u64      *data;
  u32      status;

  display = XOpenDisplay(0);

  if (display)
  {
    status = XGetWindowProperty(
          display,
          RootWindow(display, 0),
          XInternAtom(display, "_NET_WORKAREA", True),
          0,
          (~0L),
          False,
          AnyPropertyType,
          &actual_type,
          (int *) &actual_format,
          (long unsigned int*) &nitems,
          (long unsigned int*) &bytes,
          (unsigned char**) &data);

    if (status == Success)
    {
      x = data[0];
      y = data[1];
      w = data[2];
      h = data[3];

      XFree(data);
    }

    XCloseDisplay(display);
  }

#endif

  return result;
}
