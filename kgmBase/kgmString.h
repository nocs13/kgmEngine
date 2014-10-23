#pragma once
#include "kgmArray.h"

#define KGM_TSTRING_MAX_COPY 1048576

template <class T>
class kgmTString: public kgmArray<T>
{
public:
  kgmTString()
    :kgmArray<T>()
  {
  }

  kgmTString(const T* str)
  {
    if(!str)
      return;

    u32 len = 0;

    while(str[len] && (len < KGM_TSTRING_MAX_COPY))
      len++;

    alloc((T*)str, len);
  }

  kgmTString(const T* str, u32 len)
  {
    alloc((T*)str, len);
  }

  kgmTString(const kgmTString<T>& str)
  {
    alloc(str.m_data, str.m_length);
  }

  ~kgmTString()
  {
    kgmArray<T>::clear();
  }

  //operators
  kgmTString<T>& operator=(const T* str)
  {
    kgmArray<T>::clear();

    if(!str)
      return *this;

    u32 len = 0;

    while(str[len] && (len < KGM_TSTRING_MAX_COPY))
      len++;

    alloc((T*)str, len);

    return *this;
  }

  kgmTString<T>& operator=(const kgmTString<T>& str)
  {
    kgmArray<T>::clear();
    alloc(str.m_data, str.m_length);

    return *this;
  }

  kgmTString<T>& operator+=(const kgmTString<T>& s)
  {
    int len = kgmArray<T>::m_length + s.m_length;

    if(len <= kgmArray<T>::m_length)
      return *this;

    T* str = (T*)malloc(sizeof(T) * (len + 1));
    memcpy(str, kgmArray<T>::m_data, sizeof(T) * kgmArray<T>::m_length);
    memcpy(&str[kgmArray<T>::m_length], s.m_data, sizeof(T) * s.m_length);
    kgmArray<T>::clear();
    kgmArray<T>::m_data   = str;
    kgmArray<T>::m_length = len;
    kgmArray<T>::m_data[kgmArray<T>::m_length] = T(0);

    return *this;
  }

  kgmTString<T> operator+(const kgmTString<T>& s)
  {
    kgmTString<T> r = (*this);
    r += s;

    return r;
  }

  T&  operator[](u32 i)
  {
    return kgmArray<T>::m_data[i];
  }

  bool operator==(const kgmTString<T>& s)
  {
    if(!kgmArray<T>::m_data || !s.m_data)
      return false;

    return !memcmp(kgmArray<T>::m_data, s.m_data, sizeof(T) * (kgmArray<T>::m_length + 1));
  }

  bool operator==(const T* s)
  {
    if(!kgmArray<T>::m_data || !s)
      return false;

    return !memcmp(kgmArray<T>::m_data, s, sizeof(T) * (kgmArray<T>::m_length + 1));
  }

  /////////// type cast
  operator T*() const
  {
    return (T*)kgmArray<T>::m_data;
  }

  operator const T*() const
  {
    return (const T*)kgmArray<T>::m_data;
  }

  ////////////////////
  void alloc(u32 len)
  {
    kgmArray<T>::clear();
    kgmArray<T>::m_length = len;
    kgmArray<T>::m_data   = (T*)malloc(sizeof(T) * (kgmArray<T>::m_length + 1));
    kgmArray<T>::m_data[kgmArray<T>::m_length] = T(0);
  }

  void alloc(T* m, u32 len)
  {
    kgmArray<T>::clear();
    kgmArray<T>::m_length = len;
    kgmArray<T>::m_data = (T*)malloc(sizeof(T) * (kgmArray<T>::m_length + 1));
    memcpy(kgmArray<T>::m_data, m, kgmArray<T>::m_length);
    kgmArray<T>::m_data[kgmArray<T>::m_length] = T(0);
  }

  void toUpper()
  {
    if(!kgmArray<T>::m_data)
      return;

    const T diff = (T)'a' - (T)'A';

    for(u32 i = 0; i <= kgmArray<T>::m_length; i++)
      if(kgmArray<T>::m_data[i] >= 'A' && kgmArray<T>::m_data[i] <= 'Z')
        kgmArray<T>::m_data[i] += diff;
  }

  void toLower()
  {
    if(!kgmArray<T>::m_data)
      return;

    const T diff = (T)'a' - (T)'A';

    for(u32 i = 0; i <= kgmArray<T>::m_length; i++)
      if(kgmArray<T>::m_data[i] >= 'a' && kgmArray<T>::m_data[i] <= 'z')
        kgmArray<T>::m_data[i] -= diff;
  }

  bool exist(T& t)
  {
    for(u32 i = 0; i <= kgmArray<T>::m_length; i++)
      if(kgmArray<T>::m_data[i] == t)
        return true;

    return false;
  }

  int index(T& t)
  {
    for(u32 i = 0; i <= kgmArray<T>::m_length; i++)
      if(kgmArray<T>::m_data[i] == t)
        return i;

    return -1;
  }

  T* pointer(u32 i)
  {
    if(i > kgmArray<T>::m_length)
      return null;

    return &kgmArray<T>::m_data[i];
  }

  kgmTString<T> tokenize(kgmTString<T>& tok, unsigned int& pos)
  {
    //not implemented
    kgmTString<T> r;
    bool istok = false;
    unsigned int  i = 0;

    return r;
  }

  //string between symbols
  kgmTString<T> string(T ssym, T esym)
  {
    kgmTString<T> r;
    int istart;
    int iend;
    int size;

    istart = find(ssym, 0);

    if(istart < 0)
      return false;

    istart++;
    iend = find(esym, istart);

    if(iend < 0)
      return false;

    size = iend - istart;
    r.alloc(pointer(istart), size);

    return r;
  }

  kgmTString<T>& fromSym(T sym)
  {
    alloc(1);
    kgmArray<T>::m_data[0] = (T)sym;

    return *this;
  }

  //kgmTString<T>& operator=(double str){
  //}


  //int split(T sym, kgmList<>)
protected:
  //find symbol position
  int find(T sym, int start)
  {
    if(start > kgmArray<T>::m_length)
      return -1;

    for(int i = start; i <= kgmArray<T>::m_length; i++)
      if(kgmArray<T>::m_data[i] == sym)
        return i;

    return -1;
  }

public:
  static void sReplace(T* s, T ss, T es)
  {
    T* c = s;

    do
    {
      if(*c == ss) *c = es;
    }
    while(*c != (T)0);
  }

  static bool toHexString(kgmTString<T>& hexStr, int8* buf, int32 bufLen)
  {
    if(!buf || !bufLen)
      return false;

    return true;
  }
};


typedef kgmTString<char>  kgmCString;
typedef kgmTString<short> kgmWString;

#ifndef KGM_UNICODE_STRING
typedef kgmTString<char>		kgmString;
#else
typedef kgmTString<short>		kgmString;
#endif

//string between symbols
/*inline void str_syms(char* src, char* dst, char sch, char ech){
 char* pst = strchr(src, sch);
 if(!pst) return;
 char* pen = strchr(++pst, ech);
 if(!pen) return;
 memcpy(dst, pst, (int)(pen - pst));
}

//close string
inline void str_close(char* str){
 char* ptr_r = strchr(str, '\r');
 char* ptr_n = strchr(str, '\n');
 if(ptr_r) *ptr_r = '\0';
 if(ptr_n) *ptr_n = '\0';
}
*/
