#pragma once
#include "kgmTypes.h"
#include "kgmObject.h"
#include "kgmString.h"
#include <stdlib.h>

class kgmConvert: public kgmObject{
 public:
  static s32 toInteger(kgmString& s){
    return atoi(s.data());
  }

  static f64 toDouble(kgmString& s){
    return atof(s.data());
  }

  static kgmString toString(s32 i){
    char* a = new char[17];
    memset(a, 0, 17);
    sprintf(a, "%i", i);
    a[16] = '\0';
    kgmString s(a, strlen(a));
    delete [] a;
    return s;
  }

  static kgmString toString(f64 i){
    char* a = new char[65];
    memset(a, 0, 65);
    sprintf(a, "%f", i);
    a[64] = '\0';
    kgmString s(a, strlen(a));
    delete [] a;
    return s;
  }
};
