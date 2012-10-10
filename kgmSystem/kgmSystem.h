#pragma once
#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmInterface.h"

class kgmSystem: public kgmInterface, public kgmObject
{
public:
 static void sleep(uint ms);
 static void getDesktopDimension(uint& w, uint& h);
 static void getCurrentDirectory(kgmString&);
 static void setCurrentDirectory(kgmString&);
 static void getTemporaryDirectory(kgmString&);
 static bool  getCpuData(int &cpus, int &cores);
 static bool isFile(kgmString&);
 static bool isDirectory(kgmString&);
};
