#pragma once
#include <stdio.h>
#include "kgmString.h"
typedef unsigned int u32;

class kgmFile{
public:
  enum
  {
    Read   = 1 << 0L,
    Write  = 1 << 1L,
    Create = 1 << 2L,
  };

  FILE* m_file;
public:
  //CONSTRUCTOR-DESTRUCTOR
  kgmFile();
  ~kgmFile();

  //FUNCTIONS
  bool open(kgmCString& path, u32 mode); //open create,read,write
  void close();
  void flush();
  u32 read(void *dst, u32 cnt);
  u32 write(void *src, u32 cnt);
  u32 append(void *src, u32 cnt);
  u32 erase(u32 from, u32 size);
  u32 length();
  u32 length(u32 len);
  u32 position();
  u32 seek(u32 pos);
  bool eof();
  void* mmap();
  void munmap(void* v);

  //STATIC
  static bool remove_file(kgmString path);
  static bool rename(kgmString oname, kgmString nname);
};


