#pragma once
#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"

class kgmSystem: public kgmObject
{
public:
  static const char* PathDelim;
  static const int   PathDelimSym;

  static void sleep(u32 ms);
  static void getDesktopDimension(u32& w, u32& h);
  static void getCurrentDirectory(kgmString&);
  static void setCurrentDirectory(kgmString&);
  static void getTemporaryDirectory(kgmString&);
  static void getHomeDirectory(kgmString&);
  static void getPathDelim(kgmString&);
  static bool createDirectory(kgmString&);
  static bool getCpuData(int &cpus, int &cores);
  static bool isFile(kgmString&);
  static bool isDirectory(kgmString&);
  static bool splitPath(kgmString path, kgmString& dir, kgmString& file);
  static int  getProcessId();
  static int  getProcessPath(kgmString&);
};
