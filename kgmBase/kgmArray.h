#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "kgmTypes.h"
#include "../kgmSystem/kgmMemory.h"

////////// Array
template <class T>
class kgmArray
{
protected:
  T   *m_data;
  u32  m_length;

public:
  kgmArray()
  {
    m_data = null;
    m_length = 0;
  }

  kgmArray(u32 len)
  {
    m_data = null;
    m_length = 0;

    alloc(len);
  }

  kgmArray(const kgmArray& a)
  {
    m_data = null;
    m_length = 0;

    alloc(a.m_data, a.m_length);
  }

  ~kgmArray()
  {
    clear();
  }

  kgmArray<T>& operator=(const kgmArray<T>& a)
  {
    clear();

    if(!a.m_length)
      return *this;

    alloc(a.m_data, a.m_length);

    return *this;
  }

  T& operator[](const s32 i) const
  {
   return m_data[i];
  }

  operator T *()
  {
    return m_data;
  }

  void add(const T& el)
  {
    realloc(m_length + 1);

    m_data[m_length - 1] = el;
  }

  void clear()
  {
    if(m_data != nullptr)
      kgm_free(m_data);

    m_data = null;
    m_length = 0;
  }

  bool empty()
  {
    return (m_length == 0);
  }

  int length()
  {
    return m_length;
  }

  T* data()
  {
    return m_data;
  }

  bool alloc(u32 len)
  {
    clear();

    if(!len)
      return false;

    m_data = (T*) kgm_alloc(sizeof(T) * len);
    m_length = len;

    return true;
  }

  bool alloc(T* data, u32 len)
  {
    clear();

    if(!data || !len)
      return false;

    m_data = (T*)kgm_alloc(sizeof(T) * len);
    memcpy(m_data, data, sizeof(T) * len);

    m_length = len;

    return true;
  }

  void realloc(u32 len)
  {
    T*   old  = m_data;

    m_data   = (T*) kgm_realloc(old, sizeof(T) * len);
    m_length = len;
  }

  void zero()
  {
    if(!m_data || !m_length)
      return;

    memset(m_data, 0, sizeof(T) * m_length);
  }
};

typedef kgmArray<s8>  kgmByteArray;
