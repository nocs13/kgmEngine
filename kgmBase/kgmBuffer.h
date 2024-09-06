#pragma once

#include "kgmArray.h"
#include "kgmObject.h"

template <class T>
class kgmBuffer: public kgmObject
{
  KGM_OBJECT(kgmBuffer)

  kgmArray<T> m_array;

  public:

  kgmBuffer();

  kgmBuffer(const kgmBuffer<T>& v)
  {
    m_array = v.m_array;
  }

  kgmBuffer(kgmArray<T>& a)
  {
    m_array = a;
  }

  kgmArray<T>& array()
  {
    return m_array;
  }
};