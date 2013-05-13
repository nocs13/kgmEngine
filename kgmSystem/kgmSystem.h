#pragma once
#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmInterface.h"

class kgmSystem: public kgmInterface, public kgmObject
{
public:
 static void sleep(u32 ms);
 static void getDesktopDimension(u32& w, u32& h);
 static void getCurrentDirectory(kgmString&);
 static void setCurrentDirectory(kgmString&);
 static void getTemporaryDirectory(kgmString&);
 static void getHomeDirectory(kgmString&);
 static bool getCpuData(int &cpus, int &cores);
 static bool isFile(kgmString&);
 static bool isDirectory(kgmString&);
};
