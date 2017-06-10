#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void kgm_del_record(const char* file, int line, void* memory);

struct DbgMemory
{
  void*  memory;
  size_t length;

  char* file;
  int   line;

  DbgMemory *prev, *next;
};

DbgMemory* allocated = NULL;

DbgMemory* GetLastDbgMemory()
{
  if (allocated == NULL)
    return NULL;

  DbgMemory* node = allocated;

  while(node->next != NULL)
    node = node->next;

  return node;
}

void* AddDbgMemory(size_t size, char* file, int line)
{
  void* memory = malloc(size);

  DbgMemory* dbg = (DbgMemory*) malloc(sizeof(DbgMemory));

  dbg->memory = memory;

  dbg->length = size;

  dbg->file = strdup(file);

  dbg->line = line;

  dbg->prev = NULL;

  dbg->next = NULL;

  if (allocated == NULL)
  {
    allocated = dbg;

    return memory;
  }

  DbgMemory* last = GetLastDbgMemory();

  if(last == NULL)
    exit(-1);

  last->next = dbg;
  dbg->prev = last;

  return memory;
}

void DelDbgMemory(void* memory)
{
  if (memory == NULL || allocated == NULL)
    return;

  DbgMemory* node = allocated;

  while(node)
  {
    if ((node)->memory != memory)
    {
      node = node->next;

      continue;
    }

    DbgMemory* current = node;

    if(current->prev)
      current->prev->next=current->next;

    if(current->next)
      current->next->prev=current->prev;

    if(!current->prev && !current->next)
      allocated = NULL;

    free(current->memory);
    free(current->file);
    free(current);

    break;
  }
}

void ClearDbgMemory()
{
  if (allocated == NULL)
    return;

  DbgMemory* node = allocated;

  while(node != NULL)
  {
    DbgMemory* current = node;

    node = node->next;

    kgm_del_record(current->file, current->line, current->memory);

    free(current->memory);
    free(current->file);
    free(current);
  }

  allocated = NULL;
}

void* kgm_malloc(size_t size)
{
  return malloc(size);
}

void kgm_free(void* memory)
{
  free(memory);
}

#define MEM_LOG_FILE "/tmp/kgm_engine_memory.log"

void kgm_add_record(const char* file, int line, void* memory)
{
  FILE* out = fopen(MEM_LOG_FILE, "a");

  if(out)
  {
    fprintf(out, "Allocated memory: %p %d %s\n", memory, line, file);
    fclose(out);
  }
}

void kgm_del_record(const char* file, int line, void* memory)
{
  FILE* out = fopen(MEM_LOG_FILE, "a");

  if(out)
  {
    fprintf(out, "Deallocated memory: %p %d %s\n", memory, line, file);
    fclose(out);
  }
}

void* kgm_dbg_malloc(const char* file, int line, size_t size)
{
  //void* memory = kgm_malloc(size);

  //kgm_add_record(file, line, memory);
  void* memory = AddDbgMemory(size, (char*) file, line);

  return memory;
}

void kgm_dbg_free(const char* file, int line, void* memory)
{
  //kgm_free(memory);

  //kgm_del_record(file, line, memory);
  DelDbgMemory(memory);
}
