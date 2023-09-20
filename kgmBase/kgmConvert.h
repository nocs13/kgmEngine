#pragma once
#include "kgmTypes.h"
#include "kgmObject.h"
#include "kgmString.h"
#include <stdlib.h>
#include <stdio.h>

class kgmConvert: public kgmObject
{
 public:
  static s32 toInteger(kgmString& s, bool hex = false)
  {
    if (hex)
      return strtol(s.data(), NULL, 16);

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
      snprintf(a, 63, "%x", i);
    else
      snprintf(a, 63, "%i", i);
    kgmString s(a, strlen(a));
    delete [] a;

    return s;
  }

  static kgmString toString(u32 i, bool hex = false)
  {
    char* a = new char[64];
    memset(a, 0, 64);

    if(hex)
      snprintf(a, 63, "%x", i);
    else
      snprintf(a, 63, "%u", i);

    kgmString s(a, strlen(a));
    delete [] a;

    return s;
  }

  static kgmString toString(f64 i)
  {
    char* a = new char[64];
    memset(a, 0, 64);
    snprintf(a, 63, "%f", i);
    a[63] = '\0';
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

  static kgmString toString(void *p)
  {
    char* a = new char[24];

    memset(a, 0, 24);
    snprintf(a, 23, "%p", p);

    kgmString s(a, strlen(a));
    delete [] a;

    return s;
  }
};
