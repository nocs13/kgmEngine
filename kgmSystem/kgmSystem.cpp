#include "kgmSystem.h"
#include "../kgmBase/kgmList.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
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

#ifdef LINUX

struct CPU
{
  int num;
  int id;
  int mhz;
  int  cores;
};

#endif

void kgmSystem::sleep(u32 ms){
#ifdef WIN32
  Sleep(ms);
#else
  usleep(ms * 1000);
#endif
}

bool kgmSystem::getCpuData(int& cpu_count, int& cpu_cores){
#ifdef WIN32
#elif defined LINUX

  kgmList<CPU>  cpus;

  const char* cpu_path = "/proc/cpuinfo";
  struct stat file_stat;

  if(stat(cpu_path, &file_stat)){
    return false;
  }

  FILE* file = fopen(cpu_path, "r");
  if(!file)
    return false;

  //char* info = new char[file_stat.st_size + 1];
  //fread(info, file_stat.st_size, 1, file);

  char* buf = new char[1025];

  CPU cpu = {0};

  while(!feof(file) && !ferror(file))
  {
    memset(buf, 0, 1025);
    fgets(buf, 1024, file);

    if(!strncmp(buf, "processor", 9))
    {
      int num = 0;
      sscanf(buf, "processor : %i", &num);
      if(num != cpu.num)
      {
        cpus.add(cpu);
        cpu.num = num;
      }
    }
    else if(!strncmp(buf, "physical id", 11))
    {
      sscanf(buf, "physical id : %i", &cpu.id);
    }
    else if(!strncmp(buf, "cpu cores", 9))
    {
      sscanf(buf, "cpu cores : %i", &cpu.cores);
    }
    else if(!strncmp(buf, "cpu MHz", 7))
    {
      sscanf(buf, "cpu MHz : %i", &cpu.mhz);
    }
  }

  fclose(file);

  cpus.add(cpu);

  delete [] buf;

  cpu_count = cpus.size();
  cpu_cores = cpu.cores;

  cpus.clear();
#endif
  return true;
}

void kgmSystem::getDesktopDimension(u32& w, u32& h){
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

#ifdef ANDROID
#endif
}

void kgmSystem::getCurrentDirectory(kgmString& s){
  kgmMemory<u8> buf(1024);
  buf.zero();
#ifdef WIN32
  GetCurrentDirectory(1024, buf);
#else
  getcwd(buf, 1024);
#endif
  s = buf;
}

void kgmSystem::setCurrentDirectory(kgmString& s){
  kgmMemory<u8> buf(256);
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
  kgmMemory<u8> buf(256);
  buf.zero();
#ifdef WIN32
  DWORD dw = GetTempPath(MAX_PATH, buf);

  if(dw != NULL)
    s = buf;
#endif

#ifdef LINUX
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

bool kgmSystem::isFile(kgmString& s){
  FILE* file;
  if(!(file = fopen(s, "r")))
    return false;
  fclose(file);
  return true;
}

bool kgmSystem::isDirectory(kgmString& s){
  if(s.length() < 1)
    return false;

#ifdef WIN32
  DWORD attr =  GetFileAttributes(s.data());
  if((attr != 0xffffffff) && (attr & FILE_ATTRIBUTE_DIRECTORY))
    return true;
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
