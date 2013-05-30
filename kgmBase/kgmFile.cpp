#include "kgmFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#ifdef WIN32
#else
#include <unistd.h>
#include <sys/types.h>
#endif


kgmFile::kgmFile(){
  m_file = 0;
}

kgmFile::~kgmFile(){
  close();
}

//FUNCTIONS
bool kgmFile::open(kgmCString& file, u32 mode){
  kgmString smode;

  if(mode & kgmFile::Create)
    smode = "w+b";
  else if(mode & kgmFile::Write)
    smode = "r+b";
  else
    smode = "rb";

  m_file = fopen(file, smode);

  if(!m_file)
    return false;

  return true;
}

void kgmFile::close(){
  if(m_file)
    fclose(m_file);
  m_file = 0;
}

void kgmFile::flush()
{
  fflush(m_file);
}

u32 kgmFile::read(void *dst, u32 cnt){
  return fread(dst, 1, cnt, m_file);
}

u32 kgmFile::write(void *src, u32 cnt){
  return fwrite(src, 1, cnt, m_file);
}

u32 kgmFile::append(void *src, u32 cnt){
  return 0;
}

u32 kgmFile::erase(u32 from, u32 size){
  return 0;
}

u32 kgmFile::length(){
  long size = 0;
  long cpos = 0;
  cpos = ftell(m_file);
  fseek(m_file, 0, SEEK_END);
  size = ftell(m_file);
  fseek(m_file, cpos, SEEK_SET);
  return (u32)size;
}

u32 kgmFile::length(u32 len){
#ifdef WIN32
  return _chsize_s(m_file->_fileno, len);
#elif defined ANDROID
  if(m_file && m_file->_file)
    return ftruncate(m_file->_file, len);
#else
  if(m_file && m_file->_fileno)
    return ftruncate(m_file->_fileno, len);
#endif

  return 0;
}

u32 kgmFile::position(){
  return (u32)ftell(m_file);
}

u32 kgmFile::seek(u32 pos){
  if(!m_file)
    return -1;

  return fseek(m_file, pos, SEEK_SET);
}

bool kgmFile::eof(){
  return feof(m_file);
}

void* kgmFile::mmap(){
  u32 size = 0;

  if(!m_file)
    return 0;
  size = length();
  if(!size)
    return 0;
  char* v = (char*)malloc(sizeof(char) * size);
  if(!v)
    return 0;

  seek(0);
  read(v, size);
  seek(0);
  return v;
}

void kgmFile::munmap(void* v){
  if(v)
    free(v);
}

bool kgmFile::remove_file(kgmString path)
{
  if(remove(path.data()))
    return false;

  return true;
}

bool kgmFile::rename(kgmString oname, kgmString nname)
{
  if(rename(oname.data(), nname.data()))
    return false;

  return true;
}


