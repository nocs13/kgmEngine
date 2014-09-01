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
    x_linked = null;
  }

  kgmVariable(kgmString name, f64 data, void* xptr = null)
  {
    id = name;
    type = TFloat;
    v_float = data;
    x_linked = xptr;
  }

  kgmVariable(kgmString name, kgmString data, void* xptr = null)
  {
    id = name;
    type = TString;
    v_string = data;
    x_linked = xptr;
  }

  kgmVariable(kgmString name, s32 data, void* xptr = null)
  {
    id = name;
    type = TInteger;
    v_integer = data;
    x_linked = xptr;
  }

  kgmVariable(kgmString name, bool data, void* xptr = null)
  {
    id = name;
    type = TBoolean;
    v_boolean = data;
    x_linked = xptr;
  }

  kgmVariable(kgmString name, void* data, void* xptr = null)
  {
    id = name;
    type = TPointer;
    v_pointer = data;
    x_linked = xptr;
  }

  void* getXlinked()
  {
    return x_linked;
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

  void setFloat(f64 val)
  {
    if(type == TFloat)
      v_float = val;

    if(x_linked)
      *((float*)x_linked) = val;
  }

  void setInteger(s32 val)
  {
    if(type == TInteger)
      v_integer = val;

    if(x_linked)
      *((s32*)x_linked) = val;
  }

  void setString(str val)
  {
    if(type == TString)
      v_string = val;

    if(x_linked)
      *((kgmString*)x_linked) = val;
  }

  void setBoolean(bool val)
  {
    if(type == TBoolean)
      v_boolean = val;

    if(x_linked)
      *((bool*)x_linked) = val;
  }

  void setPointer(void* val)
  {
    if(type == TPointer)
      *((s32*)v_pointer) = (s32)val;
  }
};

#endif // KGMVARIABLE_H
