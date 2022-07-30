// kgmArchive.h: interface for the kgmArchive class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <stdio.h>
#include <string.h>

#include "kgmList.h"
#include "kgmFile.h"
#include "kgmMemory.h"

#define   KGMPAK_SIG  "kgmpak"
#define   KGMPAK_VER  0x0101

class kgmArchive{
 //kgmArchive descriptor: kgmArchive
 /*
 10 - bytes: kgmArchive
 1  - bytes: minor version
 1  - bytes: major vesion
 64 - bytes: archived file name
 4  - bytes: archived file size
 N  - bytes: archived file data N size defend from size
 * * *
 */

 /*
 struct Node {
  unsigned char name[64];
  unsigned int  size;
  unsigned int	offset;
 };

 FILE*	m_archive;
 FILE*	m_file;

 std::vector<Node> m_nodes;
 */

  struct Header{
    char  signature[6];
    short version;
    u32   entries;
  };

  struct Entry{
    char name[64];    //entry file path
    u32  size;        //entry file size
    u32  offset;      //entry file offset from TOC end
    bool valid;
  };

public:

  kgmList<Entry> toc;
  Header         head;
  bool           changed;

  kgmFile        archive;
  kgmString      path;

  u32            start;
public:
  kgmArchive();
  virtual ~kgmArchive();

  void close()
  {
    if(changed)
      flush();

    toc.clear();
    archive.close();
    changed = false;
  }

  bool open(kgmString pakf)
  {
    if(archive.valid())
      return false;

    memset(&head, 0, sizeof(head));

    if(pakf.length() < 1)
      return false;

    if(!archive.open(pakf, kgmFile::Write))
      return false;

    archive.seek(0);
    archive.read(head.signature, 6);
    archive.read(&head.version, 2);
    archive.read(&head.entries, 4);

    if(memcmp(head.signature, KGMPAK_SIG, 6) ||
       (head.version != KGMPAK_VER))
    {
      archive.close();

      return false;
    }

    for(u32 i = 0; i < head.entries; i++)
    {
      Entry entry = {{0}};
      archive.read(entry.name,    64);
      archive.read(&entry.size,   4);
      archive.read(&entry.offset, 4);
      entry.valid = true;

      toc.add(entry);
    }

    path    = pakf;
    changed = false;
    start   = archive.position();

    return true;
  }

  bool create(kgmString pakf){
    memset(&head, 0, sizeof(head));
    memcpy(head.signature, KGMPAK_SIG, 6);
    head.version = KGMPAK_VER;

    if(!archive.open(pakf, kgmFile::Create))
      return false;

    archive.write(head.signature, 6);
    archive.write(&head.version, 2);
    archive.write(&head.entries, 4);

    toc.clear();
    changed = false;

    return true;
  }

  bool flush()
  {
    kgmString narch_s = path + "~~";
    kgmFile   narch;
    int       entries = 0;

    for(int i = 0; i < toc.size(); i++)
    {
      Entry e = toc[i];

      if(!e.valid)
        continue;

      entries ++;
    }

    if(!narch.open(narch_s, kgmFile::Create))
      return false;

    u32 t_offset  = sizeof(Header) + 72 * entries;
    narch.write(&head.signature, 6);
    narch.write(&head.version,   2);
    narch.write(&entries,      4);

    for(int i = 0; i < toc.size(); i++)
    {
      Entry e = toc[i];

      if(!e.valid)
        continue;

      narch.write(e.name,    sizeof(e.name));
      narch.write(&e.size,   4);
      narch.write(&t_offset, 4);
      t_offset += e.size;
    }

    for(int i = 0; i < toc.size(); i++)
    {
      Entry e = toc[i];

      if(!e.valid)
        continue;

      kgmArray<u8> m;
      m.alloc(e.size);
      archive.seek(e.offset);
      archive.read(m.data(), m.length());
      narch.write(m.data(), m.length());
      m.clear();
    }

    narch.flush();
    narch.seek(0);
    archive.seek(0);
    archive.length(0);
    archive.flush();

    char  buf[64];
    int   res = 0;

    do
    {
      res = narch.read(buf, 64);

      if (res)
        res = archive.write(buf, res);
    }while(res);

    narch.close();
    kgmFile::remove_file(narch_s);

    archive.seek(0);
    toc.clear();

    archive.read(head.signature, 6);
    archive.read(&head.version, 2);
    archive.read(&head.entries, 4);

    if(memcmp(head.signature, KGMPAK_SIG, 6) ||
       (head.version != KGMPAK_VER))
    {
      archive.close();

      return false;
    }

    for(u32 i = 0; i < head.entries; i++)
    {
      Entry entry = {{0}};
      archive.read(entry.name,    sizeof(entry.name));
      archive.read(&entry.size,   4);
      archive.read(&entry.offset, 4);
      entry.valid = true;

      toc.add(entry);
    }

    changed = false;

    return true;
  }

  bool append(kgmString f)
  {
    kgmFile  ff;

    if(!archive.valid() || (f.length() < 1))
      return false;

    //check if alredy exist this name
    for(int i = 0; i < toc.size(); i++)
    {
      if(!strcmp(f, toc[i].name))
        return false;
    }

    if(!ff.open(f, 0))
      return false;

    char delim = '/';

    #ifdef WIN32
    delim = '\\';
    #endif

    char *pn = strrchr((char*)f, delim);

    if(pn)
      pn++;
    else
      pn = f;

    Entry e = {{0}};
    strcpy(e.name, pn);
    e.size = ff.length();
    e.valid = true;
    e.offset = archive.length();
    u8 buf[64];

    archive.seek(archive.length());

    while(int res = ff.read(buf, 64))
    {
      res = archive.write(buf, res);
    }

    toc.add(e);
    archive.flush();

    changed = true;

    return true;
  }

  bool erase(kgmString id)
  {
    u32 i = 0;

    if(!id.length() || !archive.valid())
      return false;

    for(i = 0; i < (u32) toc.size(); i++)
    {
      if(id == (const char*)toc[i].name)
      {
        toc[i].valid = false;
        changed = true;
        break;
      }
    }

    return true;
  }

  bool copy(kgmString id, kgmArray<u8>& m)
  {
    Entry *pe = null;

    if(!id.length() || !archive.valid() || changed)
      return false;

    for(u32 i = 0; i < (u32) toc.size(); i++)
    {
      if(!strcmp(id, toc[i].name))
        pe = &toc[i];
    }

    if(!pe)
      return false;

    m.alloc(pe->size);
    archive.seek(pe->offset);
    archive.read(m.data(), pe->size);

    return true;
  }

  bool copy(u32 index, kgmArray<u8>& m)
  {
    Entry *pe = 0;

    if(!archive.valid() || changed)
      return false;

    if(index >= (u32) toc.size())
      return false;

    pe = &toc[index];

    if(!pe)
      return false;

    m.alloc(pe->size);
    archive.seek(pe->offset);
    archive.read(m.data(), pe->size);
    archive.seek(0);

    return true;
  }

  u32 count()
  {
    return toc.size();
  }

  void info(u32 i, kgmString& name, u32& size, u32& offset)
  {
    if(i == 0 || i >= (u32) toc.size())
      return;

    name   = toc[i].name;
    size   = toc[i].size;
    offset = toc[i].offset;
  }
};
