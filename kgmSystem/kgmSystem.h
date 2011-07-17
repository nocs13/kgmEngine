#pragma once
#include "../kgmMain/kgmTypes.h"
#include "../kgmMain/kgmString.h"

class kgmSystem
{
public:
 static void sleep(uint ms);
 static void getDesktopDimension(uint& w, uint& h);
 static void getCurrentDirectory(kgmString&);
 static void setCurrentDirectory(kgmString&);
 static void getTemporaryDirectory(kgmString&);
 static bool isFile(kgmString&);
 static bool isDirectory(kgmString&);
};
