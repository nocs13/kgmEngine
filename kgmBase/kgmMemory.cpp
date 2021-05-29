#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kgmMemory.h"

#ifdef u_int32_t
typedef u_int32_t uint32;
#else
typedef uint32_t uint32;
#endif

struct _k_Memory
{
  void*  p;
  uint32 s;
  uint32 i;
};

#define _K_MEMORY_CHUNK 512

_k_Memory* _memory = nullptr;

unsigned int _len = 0;
unsigned int _cur = -1;

void _k_memory_init()
{
  if (_memory)
    return;

  _memory = (_k_Memory*) kgm_alloc(sizeof(_k_Memory) * _K_MEMORY_CHUNK);

  _len = _K_MEMORY_CHUNK;
  _cur = -1;
}

unsigned int _k_memory_alloc(size_t size)
{
  if (size < 1)
    return -1;

  if (_cur == _len)
  {
    _memory = (_k_Memory*) kgm_realloc(_memory, sizeof(_k_Memory) * (_K_MEMORY_CHUNK + _len));

    _len += _K_MEMORY_CHUNK;

    for (auto i = _cur; i < _len; i++)
      _memory[i] = {nullptr, 0, (uint32) -1};
  }

  _k_Memory m;

  m.p = kgm_alloc(size);
  m.s = size;
  m.i = _cur;

  _memory[_cur] = m;

  _cur ++;

  while(_cur < _len)
  {
    if (_memory[_cur].i == -1)
      break;

    _cur ++;
  }

  return m.i;
}

void _k_memory_free(unsigned int i)
{
  if (i == -1 || i > _len)
    return;

  if (_memory[i].i == -1)
    return;

  if (_memory[i].p != nullptr)
    kgm_free(_memory[i].p);

  _cur = i;

  _memory[i] = {nullptr, 0, (uint32) -1};
}
