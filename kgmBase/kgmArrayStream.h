#pragma once

#include "kgmArray.h"

template <class T>
class kgmArrayStream {
  kgmArray<T>* m_data;
  u32          m_position;

public:
  kgmArrayStream(kgmArray<T> &d) {
    m_data = &d;
  }

  bool eof()
  {
    return (m_position >= (m_data->length() - 1));
  }

  u32 read(T* data, u32 size){
    if(!m_data || !m_data->length() || !data || !size)
      return -1;

    if(m_position >= (m_data->length() -1))
      return -1;

    if(m_data->length() < (m_position + size))
      size = m_data->length() - m_position;

    memcpy(data, &m_data->data()[m_position], sizeof(T) * size);
    m_position += size;

    return size;
  }

  u32 write(T* data, u32 size)
  {
    if(!m_data || !m_data->length() || !data || !size)
      return -1;

    if(m_position >= (m_data->length() -1))
      return -1;

    if(m_data->length() < (m_position + size))
      size = m_data->length() - m_position;

    memcpy(&m_data->data()[m_position], data, sizeof(T) * size);
    m_position += size;

    return size;
  }

  u32 seek(u32 size)
  {
    if(!m_data || !m_data->length())
      return -1;

    if(size < m_data->length())
      return (m_position = size);

    return (m_position = m_data->length() - 1);
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
        if(m_data->data()[m_position + rs] == sym[i]){
          loop = false;
          break;
        }
      }

      if(loop)
        rs++;

      if(rs >= cnt)
        return 0;
    }

    memcpy(dst, &m_data->data()[m_position], sizeof(T) * rs);
    m_position += rs;

    return rs;
  }
};