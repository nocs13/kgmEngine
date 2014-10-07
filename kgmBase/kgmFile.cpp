#include "kgmFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#ifdef WIN32
#include <windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif


kgmFile::kgmFile()
{
  m_file = 0;
}

kgmFile::~kgmFile()
{
  close();
}

bool kgmFile::open(kgmCString& path, u32 mode)
{
  u32 smode;

#ifdef WIN32
#define O_EXCL    _O_EXCL
#define O_RDWR    _O_RDWR
#define O_CREAT   _O_CREAT
#define O_RDONLY  _O_RDONLY
#define O_WRONLY  _O_WRONLY
#define O_BINARY  _O_BINARY
#defind open      _open
#defind close     _close
#defind read      _read
#defind write     _write
#endif

  if((mode & Write) && (mode & Read))
    smode = O_RDWR;
  else if(mode & Write)
    smode = O_WRONLY;
  else if(mode & Read)
    smode = O_RDONLY;

  if(mode & Create)
    smode |= (O_CREAT | O_EXCL);

#ifdef WIN32
  smode |= O_BINARY;
#endif

  m_file = ::open(path, smode);
  
  if(!m_file)
    return false;

  return true;
}

void kgmFile::close()
{
  if(m_file)
    ::close(m_file);

  m_file = 0;
}

void kgmFile::flush()
{
  ::fsync(m_file);
}

u32 kgmFile::read(void *dst, u32 cnt)
{
  return ::read(m_file, dst, cnt);
}

u32 kgmFile::write(void *src, u32 cnt)
{
  return ::write(m_file, src, cnt);
}

u32 kgmFile::length()
{
  struct stat state;

  if(fstat(m_file, &state) != 0)
    return -1;

  return (u32)state.st_size;
}

u32 kgmFile::length(u32 len)
{
#ifdef WIN32
  HANDLE hFile = (HANDLE)_get_osfhandle(m_file);

  if(hFile)
  {
    SetFilePointer(hFile, (LONG)len, NULL, FILE_BEGIN);

    if(SetEndOfFile(hFile))
      return len;
    else
      return 0;
  }
#else
  if(m_file)
    return ftruncate(m_file, len);
#endif

  return 0;
}

u32 kgmFile::position()
{
  return (u32)::lseek(m_file, SEEK_CUR, 0);
}

u32 kgmFile::seek(u32 pos)
{
  if(!m_file)
    return -1;

  return ::lseek(m_file, SEEK_SET, pos);
}

bool kgmFile::eof()
{
  return (position() == length());
}

void* kgmFile::mmap()
{
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

void kgmFile::munmap(void* v)
{
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

bool kgmFile::make_directory(kgmString& path)
{
  int res = 0;

#ifdef WIN32
  res = CreateDirectory(path.data(), NULL);
#else
  res = mkdir(path.data(), 0777);
#endif

  if(res)
    return false;

  return true;
}



