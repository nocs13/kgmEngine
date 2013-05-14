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
 1	- bytes: minor version
 1	- bytes: major vesion
 64	- bytes: archived file name
 4	- bytes: archived file size
 N	- bytes: archived file data N size defend from size
   * * *
*/

  /*
 struct Node{
  unsigned	char	name[64];
  unsigned	int		size;
  unsigned  int		offset;
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
    char name[128];   //entry file path
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
    if(archive.m_file)
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
    
    for(int i = 0; i < head.entries; i++)
    {
      Entry entry = {0};
      archive.read(entry.name,    128);
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
    
    if(!archive.open(pakf, kgmFile::Write))
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

    if(!narch.open(narch_s, kgmFile::Write | kgmFile::Create))
      return false;

    kgmList<Entry> n_toc;

    for(int i = 0; i < toc.size(); i++)
    {
      Entry e   = toc[i];
      u32   off = 0;

      if(!e.valid)
        continue;

      n_toc.add(e);
    }

    u32 t_offset  = sizeof(Header) + sizeof(Entry) * n_toc.size();
    u32 t_entries = n_toc.size();
    narch.write(&head.signature, 6);
    narch.write(&head.version,   2);
    narch.write(&t_entries,      4);

    for(int i = 0; i < n_toc.size(); i++)
    {
      Entry e = n_toc[i];
      narch.write(e.name,    128);
      narch.write(&e.size,   4);
      narch.write(&t_offset, 4);
      u32 cpos = narch.position();
      narch.seek(t_offset);
      kgmMemory<char> m;
      copy(e.name, m);
      narch.write(m.data(), m.length());
      m.clear();
      t_offset += m.length();
      narch.seek(cpos);
    }

    changed = false;

    archive.length(0);
    archive.seek(0);
    narch.seek(0);

    char* buf = new char[4096];
    int   res = 0;

    while(res = narch.read(buf, 4096))
      archive.write(buf, res);

    narch.close();
    kgmFile::remove(narch_s);

    archive.seek(0);
    toc.clear();
    head.entries = n_toc.size();

    for(int i = 0; i < n_toc.size(); i++)
      toc.add(n_toc[i]);

    return true;
  }
  
  bool append(kgmString f)
  {
    kgmFile  ff;

    if(!archive.m_file || (f.length() < 1))
      return false;

    //check if alredy exist this name
    for(int i = 0; i < toc.size(); i++)
    {
      if(!strcmp(f, toc[i].name))
        return false;
    }

    if(!ff.open(f, 0))
      return false;

    char *pn = strrchr((char*)f, '\\');

    if(pn)
      pn++;
    else
      pn = f;
    
    Entry e;
    strcpy(e.name, pn);
    e.size = ff.length();
    e.valid = true;
    e.offset = archive.length();
    archive.seek(archive.length());
    char *pmem = new char[e.size];

    if(!pmem)
      return false;
    
    archive.seek(archive.length());
    ff.read(pmem, e.size);
    archive.write(pmem, e.size);
    delete [] pmem;
    
    toc.add(e);
    changed = true;

    return true;
  }
  
  bool erase(kgmString id)
  {
    u32 i = 0;
    Entry *pe = 0;

    if(!id.length() || !archive.m_file)
      return false;

    for(i = 0; i < toc.size(); i++)
    {
      if(id == (const char*)toc[i].name)
      {
        toc[i].valid = false;
        break;
      }
    }

    return true;
  }
  
  bool copy(kgmString id, kgmMemory<char>& m){
    Entry *pe = 0;
    
    if(!id.length() || !archive.m_file || changed)
      return false;
    
    for(u32 i = 0; i < toc.size(); i++)
    {
      if(!strcmp(id, toc[i].name))
        pe = &toc[i];
    }
    
    if(!pe)
      return false;
    
    m.alloc(pe->size);
    archive.seek(pe->offset + sizeof(Header) + sizeof(Entry) * toc.length());
    archive.read(m.data(), pe->size);
    
    return true;
  }
};

