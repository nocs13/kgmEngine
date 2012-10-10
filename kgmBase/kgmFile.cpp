#include "kgmFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


kgmFile::kgmFile(){
 m_file = 0;
}

kgmFile::~kgmFile(){
 close();
}

//////FUNCTIONS
bool kgmFile::open(kgmCString& file, uint mode){
 if(mode & 0x01)
  m_file = fopen(file, "r+b");
 else
  m_file = fopen(file, "w+b");
 if(!m_file)
  return false;
 return true;
}

void kgmFile::close(){
 if(m_file)
  fclose(m_file);
 m_file = 0;
}

uint kgmFile::read(void *dst, uint cnt){
 return fread(dst, 1, cnt, m_file);
}

uint kgmFile::write(void *src, uint cnt){
 return fwrite(src, 1, cnt, m_file);
}

uint kgmFile::append(void *src, uint cnt){
 return 0;
}

uint kgmFile::erase(uint from, uint size){
 return 0;
}

uint kgmFile::length(){
 long size = 0;
 long cpos = 0;
 cpos = ftell(m_file);
 fseek(m_file, 0, SEEK_END);
 size = ftell(m_file);
 fseek(m_file, cpos, SEEK_SET);
 return (uint)size;
}

uint kgmFile::length(uint len){
 return 0;
}

uint kgmFile::position(){
 return (uint)ftell(m_file);
}

uint kgmFile::seek(uint pos){
 return fseek(m_file, pos, SEEK_SET);
}

bool kgmFile::eof(){
 return feof(m_file);
}
void* kgmFile::mmap(){
 uint size = 0;

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



