#pragma once
#include "kgmTypes.h"
#include "kgmObject.h"
#include "kgmString.h"
#include <stdlib.h>
#include <stdio.h>

class kgmConvert: public kgmObject
{
 public:
  static s32 toInteger(kgmString& s)
  {
    return atoi(s.data());
  }

  static f64 toDouble(kgmString& s)
  {
    return atof(s.data());
  }

  static bool toBoolean(kgmString& s)
  {
    if(s == "true" || s == "on")
      return true;
    else if(s == "false" || s == "off")
      return false;

    return false;
  }

  static kgmString toString(s32 i, bool hex = false)
  {
    char* a = new char[64];
    memset(a, 0, 64);
    if(hex)
      sprintf(a, "%x", i);
    else
      sprintf(a, "%i", i);
    kgmString s(a, strlen(a));
    delete [] a;
    return s;
  }

  static kgmString toString(f64 i)
  {
    char* a = new char[65];
    memset(a, 0, 65);
    sprintf(a, "%f", i);
    a[64] = '\0';
    kgmString s(a, strlen(a));
    delete [] a;
    return s;
  }

  static kgmString toString(bool b)
  {
    if(b == true)
      return kgmString("true");

    return kgmString("false");
  }
};
