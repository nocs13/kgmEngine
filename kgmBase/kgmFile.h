#pragma once
#include <stdio.h>
#include "kgmString.h"
#include "kgmObject.h"

class kgmFile: public kgmObject
{
  KGM_OBJECT(kgmFile)
public:
  enum
  {
    Read   = 1 << 0L,
    Write  = 1 << 1L,
    Create = 1 << 2L,
  };

private:
  s32 m_file;

public:
  kgmFile();
  ~kgmFile();

  bool open(kgmCString& path, u32 mode = Read);
  void close();
  void flush();
  u32  read(void *dst, u32 cnt);
  u32  write(void *src, u32 cnt);
  u32  append(void *src, u32 cnt);
  u32  erase(u32 from, u32 size);
  u32  length();
  u32  length(u32 len);
  u32  position();
  u32  seek(u32 pos);
  bool eof();
  bool valid();
  bool mmap(kgmMemory<u8> &buf);

  static bool remove_file(kgmString path);
  static bool rename(kgmString oname, kgmString nname);
  static bool make_directory(kgmString&);
};


