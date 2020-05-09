#include "kgmFile.h"
#include "../kgmBase/kgmLog.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
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
  u32 smode = 0;

#ifdef WIN32
#define O_EXCL    _O_EXCL
#define O_RDWR    _O_RDWR
#define O_CREAT   _O_CREAT
#define O_RDONLY  _O_RDONLY
#define O_WRONLY  _O_WRONLY
#define O_BINARY  _O_BINARY
#else   
#define O_BINARY  0
#endif

  if((mode & Write) && (mode & Read))
    smode = O_RDWR;
  else if(mode & Write)
    smode = O_WRONLY;
  else if(mode & Read)
    smode = O_RDONLY;

  if(mode & Create)
    smode |= O_CREAT;
  //smode |= (O_CREAT | O_EXCL);

  smode |= O_BINARY;

#ifdef WIN32
  m_file = _open(path, smode, _S_IREAD | _S_IWRITE);
#else
  m_file = ::open(path, smode, S_IRUSR | S_IWUSR);
#endif

  if(m_file < 1)
  {
    kgm_log() << "Error to open file [" << (char *) path << "], error is " << errno << "\n";

    return false;
  }

  return true;
}

void kgmFile::close()
{
  if(m_file > 0)
  {
#ifdef WIN32
    _close(m_file);
#else
    ::close(m_file);
#endif
  }
  m_file = 0;
}

void kgmFile::flush()
{
#ifdef WIN32
#else
  ::fsync(m_file);
#endif
}

u32 kgmFile::read(void *dst, u32 cnt)
{
#ifdef WIN32
  return _read(m_file, dst, cnt);
#else
  return ::read(m_file, dst, cnt);
#endif
}

u32 kgmFile::write(void *src, u32 cnt)
{
#ifdef WIN32
  return _write(m_file, src, cnt);
#else
  return ::write(m_file, src, cnt);
#endif
}

u32 kgmFile::length()
{
  struct stat state = {0};

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

bool kgmFile::valid()
{
  if(m_file > 0)
    return true;

  return false;
}

bool kgmFile::mmap(kgmMemory<u8> &buf)
{
  u32 size = 0;

  if(!m_file)
    return false;

  size = length();

  if(!size)
    return false;

  if(!buf.alloc(size))
    return false;

  u32 pos = position();

  seek(0);

  read(buf.data(), size);

  seek(pos);

  return true;
}

bool kgmFile::remove_file(kgmString path)
{
  //if(remove(path.data()))
  //  return false;

  return true;
}

bool kgmFile::rename(kgmString oname, kgmString nname)
{
  //if(rename(oname.data(), nname.data()))
  //  return false;

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



