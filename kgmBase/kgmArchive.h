// kgmArchive.h: interface for the kgmArchive class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include <stdio.h>
#include <string.h>

#include "kgmList.h"
#include "kgmFile.h"
#include "kgmMemory.h"

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
    u32  numEntries;
  };
  
  struct Entry{
    char name[64];
    u32 size;
    u32 offset;
    bool valid;
 };
  
public:

  Header head;
  kgmList<Entry> entry;
  bool changed;
  
  kgmFile archive;
public:
  kgmArchive();           
  virtual ~kgmArchive();

  void close()
  {
    entry.clear();
    archive.close();
  }
  
  bool open(kgmCString& pakf)
  {
    Header head = {0};
    if(pakf.length() < 1)
      return false;
    if(!archive.open(pakf, 0))
      return false;
    archive.seek(0);
    archive.read(head.signature, 6);
    archive.read(&head.version, 2);
    archive.read(&head.numEntries, 4);
    if(memcmp(head.signature, "kgmpak", 6) || 
       (head.version != 0x0101)){
      archive.close();
      return false;
    }
    
    scan();
    return true;
  }

  bool create(kgmCString& pakf){
    Header head = {0};
    head.version = 0x0101;
    memcpy(head.signature, "kgmpak", 6);
    
    if(!archive.open(pakf, 0))
      return false;
    archive.write(head.signature, 6);
    archive.write(&head.version, 2);
    archive.write(&head.numEntries, 4);
    archive.close();
    open(pakf);
    return true;
  }

  u32 scan(){
    if(!archive.m_file)
      return 0;
    entry.clear();
    archive.seek(12);
    while(archive.position() < archive.length()){
      Entry e;
      archive.read(e.name, 64);
      archive.read(&e.size, 4);
      e.offset = archive.position();
      e.valid = true;
      archive.seek(archive.position() + e.size);
      entry.add(e);
    }
    return entry.size();
  }
  
  bool append(kgmCString& f)
  {
    kgmFile  ff;
    if(!archive.m_file || (f.length() < 1))  
      return false;
    //check if alredy exist this name
    for(int i = 0; i < entry.size(); i++)
      if(!strcmp(f, entry[i].name))
	return false;
    if(!ff.open(f, 0))
      return false;
    char *pn = strrchr((char*)f, '\\');
    if(pn) pn++;
    else   pn = f;
    
    Entry e;
    strcpy(e.name, pn);
    e.size = ff.length();
    e.valid = true;
    e.offset = archive.length();
    archive.seek(archive.length());
    archive.write(e.name, 64);
    archive.write(&e.size, 4);
    e.offset = archive.position();
    char *pmem = new char[e.size];
    if(!pmem)
      return false;
    
    archive.seek(archive.length());
    ff.read(pmem, e.size);
    archive.write(pmem, e.size);
    delete pmem;
    
    entry.add(e);
    return true;
  }
  
  bool erase(char *id)
  {
    u32 i = 0;
    Entry *pe = 0;
    if(!id || !archive.m_file)
      return false;
    for(i = 0; i < entry.size(); i++){
      if(!strcmp(id, entry[i].name)){
	pe = &entry[i];
	break;
      }
    }
    if(!pe)
      return false;
    archive.seek(pe->offset - 68);
    u32 DP = archive.position();
    if(i == (entry.size() - 1)){
      archive.length(DP);
      scan();
      return true;
    }
    for(i++; i < entry.size(); i++){
      Entry *ce = &entry[i];
      char *pmem = (char*)malloc(sizeof(char) * ce->size + 68);
      u32 SP = ce->offset - 68;
      archive.seek(SP);
      archive.read(pmem, ce->size + 68);
      archive.seek(DP);
      archive.write(pmem, ce->size + 68);
      DP = archive.position();
      free(pmem);
    }
    archive.length(DP);
    scan();
    return true;
  }
  
  void* mmap(char *id, u32 *psize)
  {
    if(!id || !archive.m_file || !psize)
      return 0;
    Entry *pe = 0;
    for(u32 i = 0; i < entry.size(); i++){
      if(!strcmp(id, entry[i].name))
	pe = &entry[i];
    }
    if(!pe)
      return 0;
    void* mem = malloc(sizeof(char) * pe->size);
    if(!mem)
      return 0;
    archive.seek(pe->offset);
    archive.read(mem, pe->size);
    *psize = pe->size;
    return mem;
  }

  void munmap(void* mem)
  {
    if(mem)
  free(mem);
  }

  bool copy(char *id, kgmMemory<char>& m){
    Entry *pe = 0;
    
    if(!id || !archive.m_file)
      return false;
    
    for(u32 i = 0; i < entry.size(); i++){
      if(!strcmp(id, entry[i].name))
	pe = &entry[i];
    }
    
    if(!pe)
      return false;
    
    m.alloc(pe->size);
    archive.seek(pe->offset);
    archive.read(m.data(), pe->size);
    
    return true;
  }
};

