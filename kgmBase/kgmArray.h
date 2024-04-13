#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "kgmBase.h"
#include "../kgmSystem/kgmMemory.h"

////////// Array
template <class T>
class kgmArray
{
private:
  s32 m_reserve;

protected:
  T   *m_data;
  s32  m_length;

public:
  kgmArray()
  {
    m_data = null;
    m_length = 0;
    m_reserve = 0;
  }

  kgmArray(u32 len)
  {
    m_data = null;
    m_length = 0;
    m_reserve = 0;

    alloc(len);
  }

  kgmArray(const kgmArray& a)
  {
    m_data = null;
    m_length = 0;
    m_reserve = 0;

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
    if (i >= m_length)
      kgm_fatal("Invalid array index income.");

    return m_data[i];
  }

  operator T *()
  {
    return m_data;
  }

  void add(const T& el)
  {
    if (m_length == 0 && m_reserve == 0)
    {
      reserve(16);
    }
    else if (m_length == m_reserve)
    {
      reserve(m_length + m_length / 4);
    }

    m_data[m_length] = el;
    m_length++;
  }

  void clear()
  {
    if(m_data != nullptr)
      kgm_free(m_data);

    m_data = null;
    m_length = m_reserve = 0;
  }

  bool empty()
  {
    return (m_length == 0);
  }

  s32 length()
  {
    return m_length;
  }

  T* data()
  {
    return m_data;
  }

  bool alloc(s32 len)
  {
    clear();

    if(!len)
      return false;

    m_data = (T*) kgm_alloc(sizeof(T) * len);
    m_length = m_reserve = len;

    return true;
  }

  bool alloc(T* data, s32 len)
  {
    clear();

    if(!data || !len)
      return false;

    m_data = (T*)kgm_alloc(sizeof(T) * len);
    memcpy(m_data, data, sizeof(T) * len);

    m_length = m_reserve = len;

    return true;
  }

  void realloc(s32 len)
  {
    T*   old  = m_data;

    m_data   = (T*) kgm_realloc(old, sizeof(T) * len);
    m_length = m_reserve = len;
  }

  void reserve(s32 size)
  {
    T* old  = m_data;
    m_data   = (T*) kgm_realloc(old, sizeof(T) * size);

    m_reserve = size;

    if (m_length > m_reserve)
      m_length = m_reserve;
  }

  void zero()
  {
    if(!m_data || !m_length)
      return;

    memset(m_data, 0, sizeof(T) * m_length);
  }
};

typedef kgmArray<s8>  kgmByteArray;
