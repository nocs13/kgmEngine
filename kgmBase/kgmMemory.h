// kgmMemory.h: interface for the kgmMemory class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "kgmBase.h"

void	kgmClearAllocatedMemory(void);
void*	kgmalloc(unsigned int);
void	kgmfree(void*);

template <class X> bool	kgmnew(X*, unsigned int count = 0);
template <class X> void	kgmdelete(X*, unsigned int count = 0);

void *kgm_malloc(size_t);
void  kgm_free(void*);

void *kgm_dbg_malloc(const char*, int, size_t);
void  kgm_dbg_free(const char*, int, void*);

#undef KGM_DEBUG_MEMORY

#ifdef KGM_DEBUG_MEMORY

#define malloc(A) kgm_dbg_malloc(__FILE__, __LINE__, (A) )
#define free(A) kgm_dbg_free(__FILE__, __LINE__, (A) )

#else

#define malloc(A) kgm_malloc( (A) )
#define free(A) kgm_free( (A) )

#endif

/*void* operator new (size_t size)
{
  return malloc(size);
}

void operator delete (void* pointer)
{
  free(pointer);
}*/


typedef unsigned int u32;

template <class T> 
class kgmMemory
{
protected:
  T   *m_data;
  u32  m_length;
  u32  m_position;  //for seek, read, write

public:
  kgmMemory()
  {
    m_data = null;
    m_length = 0;
    m_position = 0;
  }

  kgmMemory(u32 len)
  {
    alloc(len);
  }

  kgmMemory(const kgmMemory<T>& m)
  {
    alloc(m.m_data, m.m_length);
  }

  ~kgmMemory()
  {
    clear();
  }

  kgmMemory<T>& operator=(const kgmMemory<T>& m)
  {
    clear();

    if(!m.m_length)
      return *this;

    m_data = (T*)malloc(sizeof(T) * m.m_length);
    m_length = m.m_length;
    memcpy(m_data, m.m_data, sizeof(T) * m_length);

    return *this;
  }

  T& operator[](u32 i)
  {
    return m_data[i];
  }

  operator T *()
  {
    return m_data;
  }

  void clear()
  {
    if(m_data)
      free(m_data);

    m_data = null;
    m_length = 0;
    m_position = 0;
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

    m_data = (T*)malloc(sizeof(T) * len);
    m_length = len;

    return true;
  }

  bool alloc(T* data, u32 len)
  {
    clear();

    if(!data || !len)
      return false;

    m_data = (T*)malloc(sizeof(T) * len);
    m_length = len;
    memcpy(m_data, data, sizeof(T) * m_length);

    return true;
  }

  void zero()
  {
    if(!m_data || !m_length)
      return;

    memset(m_data, 0, sizeof(T) * m_length);
  }

  bool eof()
  {
    return (m_position >= (m_length - 1));
  }

  u32 read(T* data, u32 size){
    if(!m_data || !m_length || !data || !size)
      return -1;

    if(m_position == (m_length -1))
      return -1;

    if(m_length < (m_position + size))
      size = m_length - m_position;

    memcpy(data, &m_data[m_position], sizeof(T) * size);
    m_position += size;

    return size;
  }

  u32 write(T* data, u32 size)
  {
    if(!m_data || !m_length || !data || !size)
      return -1;

    if(m_position == (m_length -1))
      return -1;

    if(m_length < (m_position + size))
      size = m_length - m_position;

    memcpy(&m_data[m_position], data, sizeof(T) * size);
    m_position += size;

    return size;
  }

  u32 seek(u32 size)
  {
    if(!m_data || !m_length)
      return -1;

    if(size < m_length)
      return (m_position = size);

    return (m_position = m_length - 1);
  }

  u32 position()
  {
    return m_position;
  }

  u32 reads(T *dst, u32 cnt, T* sym, u16 scnt){
    u32 rs = 0;
    bool loop = true;

    if(!dst || !sym)
      return 0;

    while(loop)
    {
      for(int i = 0; i < scnt; i++)
      {
        if(m_data[m_position + rs] == sym[i]){
          loop = false;
          break;
        }
      }

      if(loop)
        rs++;

      if(rs >= cnt)
        return 0;
    }

    memcpy(dst, &m_data[m_position], sizeof(T) * rs);
    m_position += rs;

    return rs;
  }
};
