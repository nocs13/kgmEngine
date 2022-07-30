#include <stdlib.h>

void* kgm_alloc(size_t);
void* kgm_realloc(void*, size_t);
void  kgm_free(void*);

template <class T> T* kgm_new();
template <class T, class... Args> T* kgm_new(Args... args);
template <class T> void kgm_delete(T* p);


