#ifndef KGMVARIABLE_H
#define KGMVARIABLE_H

#pragma once

#include "kgmObject.h"
#include "kgmString.h"
#include "kgmConvert.h"

class kgmVariable
{
  typedef kgmString str;

public:

  enum Type
  {
    TNone = 0,
    TFloat,
    TString,
    TInteger,
    TBoolean,
    TPointer
  };

private:
  kgmString id;
  Type      type;

  f64    v_float;
  str    v_string;
  s32    v_integer;
  bool   v_boolean;
  void*  v_pointer;

  void*  x_linked;  //user data pointer, which linked to update automatically

public:
  kgmVariable()
  {
    type = TNone;
  }

  kgmVariable(kgmString name, f64 data)
  {
    id = name;
    type = TFloat;
    v_float = data;
  }

  kgmVariable(kgmString name, kgmString data)
  {
    id = name;
    type = TString;
    v_string = data;
  }

  kgmVariable(kgmString name, s32 data)
  {
    id = name;
    type = TInteger;
    v_integer = data;
  }

  kgmVariable(kgmString name, bool data)
  {
    id = name;
    type = TBoolean;
    v_boolean = data;
  }

  kgmVariable(kgmString name, void* data)
  {
    id = name;
    type = TPointer;
    v_pointer = data;
  }

  Type getType()
  {
    return type;
  }

  kgmString getName()
  {
    return id;
  }

  kgmString toString()
  {
    switch(type)
    {
    case TFloat:
      return kgmConvert::toString(v_float);
    case TInteger:
      return kgmConvert::toString(v_integer);
    case TBoolean:
      return kgmConvert::toString((s32)v_boolean);
    case TPointer:
      return kgmConvert::toString((s32)v_pointer);
    case TString:
      return v_string;
    }
  }

  kgmString getString()
  {
    return v_string;
  }

  s32 getInteger()
  {
    return v_integer;
  }

  f64 getFloat()
  {
    return v_float;
  }

  bool getBoolean()
  {
    return v_boolean;
  }

  void* getPointer()
  {
    return v_pointer;
  }
};

#endif // KGMVARIABLE_H
