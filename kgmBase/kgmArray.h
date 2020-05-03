#pragma once 
#include "kgmMemory.h"

////////// Array
template <class T> 
class kgmArray: public kgmMemory<T>
{
public:
  kgmArray()
    :kgmMemory<T>()
  {
  }

  kgmArray(u32 len)
    :kgmMemory<T>(len)
  {
  }

  kgmArray(const kgmArray& a)
  {
    kgmMemory<T>::alloc(a.m_data, a.m_length);
  }

  ~kgmArray()
  {
  }

  kgmArray<T>& operator=(const kgmArray<T>& a)
  {
    kgmMemory<T>::clear();

    if(!a.m_length)
      return *this;

    kgmMemory<T>::alloc(a.m_data, a.m_length);

    return *this;
  }

  T& operator[](const s32 i) const {
   return kgmMemory<T>::m_data[i];
  }


  void realloc(u32 len)
  {
    T*   old  = kgmMemory<T>::m_data;

    kgmMemory<T>::m_data   = (T*)kgm_realloc(old, sizeof(T) * len);
    kgmMemory<T>::m_length = len;
  }

  void add(const T& el)
  {
    realloc(kgmMemory<T>::m_length + 1);
    kgmMemory<T>::m_data[kgmMemory<T>::m_length - 1] = el;
  }

  void erase(u32 index)
  {
    //not implemented yet
  }
};

typedef kgmArray<s8>  kgmByteArray;
