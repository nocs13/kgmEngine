#pragma once
#include <stdio.h>
#include "kgmString.h"
typedef unsigned int uint;

class kgmFile{
public:
 enum{
  Read   = 1 << 0L,
  Write  = 1 << 1L,
  Create = 1 << 2L,
 };
 
 FILE* m_file;
public:
///// CONSTRUCTOR/DESTRUCTOR
 kgmFile();
 ~kgmFile();
//////FUNCTIONS
 bool open(kgmCString& path, uint mode);//open create,read,write
 void close();
 uint read(void *dst, uint cnt);
 uint write(void *src, uint cnt);
 uint append(void *src, uint cnt);
 uint erase(uint from, uint size);
 uint length();
 uint length(uint len);
 uint position();
 uint seek(uint pos);
 bool eof();
 void* mmap();
 void munmap(void* v);
};


