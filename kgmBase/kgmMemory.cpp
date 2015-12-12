#include <stdio.h>
#include <stdlib.h>

inline void* kgm_malloc(size_t size)
{
  return malloc(size);
}

inline void kgm_free(void* memory)
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
  void* memory = kgm_malloc(size);

  kgm_add_record(file, line, memory);

  return memory;
}

void* kgm_dbg_free(const char* file, int line, void* memory)
{
  kgm_free(memory);

  kgm_del_record(file, line, memory);
}
