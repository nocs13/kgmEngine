// kgmMemory.h: interface for the kgmMemory class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "kgmMain.h"
#include "kgmObject.h"
#include <stdlib.h>
#include <malloc.h>

typedef unsigned int u32;

template <class T> 
class kgmMemory{
protected:
 T    *m_data;
 u32  m_length;
 u32  m_position;  //for seek, read, write
public:

 kgmMemory(){ 
  m_data = 0; 
  m_length = 0; 
  m_position = 0;
 }

 kgmMemory(u32 len){
  alloc(len);
 }

 kgmMemory(const kgmMemory<T>& m){
  alloc(m.m_data, m.m_length);
 }

 virtual ~kgmMemory()
 {
  clear();
 }

 kgmMemory<T>& operator=(const kgmMemory<T>& m){
  clear();
  if(!m.m_length)
   return *this;
  m_data = new T[m.m_length];
  m_length = m.m_length;
  memcpy(m_data, m.m_data, sizeof(T) * m_length);
  return *this;
 }

 T& operator[](u32 i){
  return m_data[i];
 }

 operator T *(){
  return m_data;
 }

 void clear(){
  if(m_data)
   delete [] m_data;
  m_data = 0;
  m_length = 0;
  m_position = 0;
 }
 
 bool empty(){
  return (m_length == 0);
 }

 int length(){
  return m_length;
 }

 T* data(){
  return m_data;
 }

 bool alloc(u32 len){
  clear();
  if(!len)
   return false;
  m_data = new T[len];
  m_length = len;
  return true;
 }

 bool alloc(T* data, u32 len){
  clear();
  if(!data || !len)
   return false;
  m_data = new T[len];
  m_length = len;
  memcpy(m_data, data, sizeof(T) * m_length);
  return true;
 }
 
 void zero(){
  if(!m_data || !m_length)     
   return;
  memset(m_data, 0, sizeof(T) * m_length);
 } 

 bool eof(){
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

 u32 write(T* data, u32 size){
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

 u32 seek(u32 size){
  if(!m_data || !m_length)
   return -1;
  if(size < m_length)
   return (m_position = size);
  return (m_position = m_length - 1); 
 }

 u32 position(){
  return m_position;
 }

 u32 reads(T *dst, u32 cnt, T* sym, u16 scnt){
   u32 rs = 0;
   bool loop = true;

   if(!dst || !sym)
     return 0;

   while(loop){
     for(int i = 0; i < scnt; i++){
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

