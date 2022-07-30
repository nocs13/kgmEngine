#include "kgmMemory.h"
#include "../kgmBase/kgmLog.h"

//#undef DEBUG
#undef DEBUG_XXX

#define ADDOBJS 128

static size_t* g_objects = null;
static int     g_o_count = 0;

static size_t* g_news = null;
static int     g_n_count = 0;

void* kgm_alloc(size_t size)
{
  if (size < 1)
    return null;

  void* p = null;

  p = ::malloc(size);

  for (s32 i = 0; i < g_o_count; i++) {
    if (g_objects[i] == null) {
      g_objects[i] = (size_t) p;

      #ifdef DEBUG
      fprintf(stderr, "kgm_memory_alloc [%p, %ld].\n", p, size);
      #endif

      return p;
    }
  }

  s32 len = g_o_count;

  g_objects = (size_t*) ::realloc(g_objects, sizeof(size_t) * (len + ADDOBJS));

  for (s32 i = len; i < (len + ADDOBJS); i++)
    g_objects[i] = null;

  g_objects[len] = (size_t) p;

  g_o_count = len + ADDOBJS;

  #ifdef DEBUG
  fprintf(stderr, "kgm_memory_alloc [%p, %ld].\n", p, size);
  #endif

  return p;
}

void* kgm_realloc(void *p, size_t size)
{
  if (size < 1)
    return p;

  for (s32 i = 0; i < g_o_count; i++) {
    if (g_objects[i] == (size_t) p) {
      g_objects[i] = (size_t) ::realloc(p, size);

      #ifdef DEBUG
      fprintf(stderr, "kgm_memory_realloc [%p, %p, %ld].\n", p, g_objects[i], size);
      #endif

      return (void *) g_objects[i];
    }
  }

  return null;
}

void kgm_free(void* p)
{
  if (!p)
    return;

  for (s32 i = 0; i < g_o_count; i++) {
    if (g_objects[i] == (size_t) p) {
      ::free(p);

      #ifdef DEBUG
      fprintf(stderr, "kgm_memory_free [%p, %d].\n", p, i);
      #endif

      g_objects[i] = null;

      break;
    }
  }
}

template <class T> T* kgm_new()
{
  T* p = new T();

#ifdef DEBUG
  fprintf(stderr, "New memory: %p size: %ld.\n", p, sizeof(T));
#endif

  for (s32 i = 0; i < g_n_count; i++) {
    if (g_news[i] == null) {
      g_news[i] = (size_t) p;

      return p;
    }
  }

  s32 len = g_n_count;

  g_news = (size_t*) ::realloc(g_news, sizeof(size_t) * (len + ADDOBJS));

  for (s32 i = len; i < (len + ADDOBJS); i++)
    g_news[i] = null;

  g_news[len] = (size_t) p;

  g_n_count = len + ADDOBJS;

  return p;
}

template <class T, class... Args> T* kgm_new(Args... args)
{
  T* p = new T(args...);

#ifdef DEBUG
  fprintf(stderr, "New memory: %p size: %ld.\n", p, sizeof(T));
#endif

  for (s32 i = 0; i < g_n_count; i++) {
    if (g_news[i] == null) {
      g_news[i] = (size_t) p;

      return p;
    }
  }

  s32 len = g_n_count;

  g_news = (size_t*) ::realloc(g_news, sizeof(size_t) * (len + ADDOBJS));

  for (s32 i = len; i < (len + ADDOBJS); i++)
    g_news[i] = null;

  g_news[len] = (size_t) p;

  g_n_count = len + ADDOBJS;

  return p;
}

template <class T> void kgm_delete(T* p)
{
#ifdef DEBUG
  fprintf(stderr, "Delete memory: %p.\n", p);
#endif

  if (!p)
    return;

  for (s32 i = 0; i < g_n_count; i++) {
    if (g_news[i] == (size_t) p) {
      delete p;

      g_news[i] = null;

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

  if (!g_news) {
    g_news = (size_t*) ::malloc(sizeof(size_t) * ADDOBJS);

    g_n_count = ADDOBJS;

    for (s32 i = 0; i < ADDOBJS; i++)
      g_news[i] = null;
  }
}

void kgm_memory_cleanup()
{
  #ifdef DEBUG
  fprintf(stderr, "kgm_memory_cleanup.\n");
  #endif

  for (s32 i = 0; i < g_n_count; i++) {
    if (g_news[i] != null) {
      #ifdef DEBUG
      fprintf(stderr, "Memory not deleted [%p, %d].\n", (void*) g_news[i], i);
      #endif

      ::free ((void*) g_news[i]);
      g_news[i] = null;
    }
  }

  for (s32 i = 0; i < g_o_count; i++) {
    if (g_objects[i] != null) {
      #ifdef DEBUG
      fprintf(stderr, "kgm_memory_cleanup [%p, %d].\n", (void*) g_objects[i], i);
      #endif

      ::free((void*) g_objects[i]);
      g_objects[i] = null;
    }
  }

  ::free(g_objects);

  g_objects = null;
  g_o_count = 0;
}
