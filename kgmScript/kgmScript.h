#pragma once
#include "../kgmMain/kgmString.h"


class kgmScript{
public:
 virtual bool load(kgmString scr) = 0;
 virtual bool run(kgmString str) = 0;
 virtual bool set(kgmString id, void* val) = 0;
 virtual bool call(kgmString fname, kgmString fmt, ...) = 0;
};
