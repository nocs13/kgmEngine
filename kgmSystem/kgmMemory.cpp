#include "kgmMemory.h"
#include "../kgmBase/kgmLog.h"

//#undef DEBUG
#undef DEBUG_XXX

#define ADDOBJS 128

static size_t* g_objects = null;
static int     g_o_count = 0;
static int     g_m_alloc = 0;

void* kgm_alloc(size_t size)
{
#ifdef DEBUG_XXX
  fprintf(stderr, "kgm_alloc [%ld].\n", size);
#endif

  if (size < 1)
    return null;

  void* p = null;

  p = ::malloc(size);
  g_m_alloc++;

#ifdef DEBUG_XXX
  //fprintf(stderr, "          pointer [%p] size [%ld].\n", p, size);
#endif

  for (s32 i = 0; i < g_o_count; i++) {
    if (g_objects[i] == null) {
      g_objects[i] = (size_t) p;

      return p;
    }
  }

  s32 len = g_o_count;

  g_objects = (size_t*) ::realloc(g_objects, sizeof(size_t) * (len + ADDOBJS));

  for (s32 i = len; i < (len + ADDOBJS); i++)
    g_objects[i] = null;

  g_objects[len] = (size_t) p;

  g_o_count = len + ADDOBJS;

  return p;
}

void* kgm_realloc(void *p, size_t size)
{
#ifdef DEBUG_XXX
  fprintf(stderr, "kgm_realloc [%p, %ld].\n", p, size);
#endif

  if (size < 1)
    return p;

  for (s32 i = 0; i < g_o_count; i++) {
    if (g_objects[i] == (size_t) p) {
      g_objects[i] = (size_t) ::realloc(p, size);

      return (void *) g_objects[i];
    }
  }

  return null;
}

void kgm_free(void* p)
{
#ifdef DEBUG_XXX
  fprintf(stderr, "kgm_free [%p].\n", p);
#endif

  if (!p)
    return;

  for (s32 i = 0; i < g_o_count; i++) {
    if (g_objects[i] == (size_t) p) {
      ::free(p);
      g_m_alloc--;

#ifdef DEBUG_X
      fprintf(stderr, "        [%p] released at [%d].\n", p, i);
#endif

      p = null;
      g_objects[i] = null;

      break;
    }
  }
}

void kgm_memory_init()
{
#ifdef DEBUG
  fprintf(stderr, "kgm_memory_init.\n");
#endif

  if (!g_objects) {
    g_objects = (size_t*) ::malloc(sizeof(size_t) * ADDOBJS);

    g_o_count = ADDOBJS;

    for (s32 i = 0; i < ADDOBJS; i++)
      g_objects[i] = null;
  }
}

void kgm_memory_cleanup()
{
#ifdef DEBUG
  fprintf(stderr, "kgm_memory_cleanup actual allocs %d.\n", g_m_alloc);
#endif

  for (s32 i = 0; i < g_o_count; i++) {
    if (g_objects[i] != null) {

#ifdef DEBUG
      fprintf(stderr, "        [%p] releasing at [%d].\n", (void*) g_objects[i], i);
#endif
      kgmObject* o = (kgmObject*) g_objects[i];
      ::free((void*) g_objects[i]);
      g_m_alloc--;
      g_objects[i] = null;
    }
  }

  ::free(g_objects);

#ifdef DEBUG
  fprintf(stderr, "kgm_memory_cleanup actual allocs %d.\n", g_m_alloc);
#endif

  g_objects = null;
  g_o_count = 0;
}
