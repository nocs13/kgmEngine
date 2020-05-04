#include "kgmEncryptor.h"
#include "kgmTime.h"
#include <math.h>

#define MAX_S32  2147483647
#define MIN_S32 -2147483648

kgmEncryptor::kgmEncryptor()
{
  m_x = m_y = m_e = m_d = m_n = m_t = m_s = 0;
}

bool kgmEncryptor::init(s32 pr_x, s32 pr_y, s32 pr_e, s32 pr_d)
{
  m_x = pr_x;
  m_y = pr_y;
  m_e = pr_e;
  m_d = pr_d;

  return true;
}

bool kgmEncryptor::init()
{
  s32 x = 0, y = 0;

  while(!x)
    x = genPrime();

  while(!y || x == y)
    y = genPrime();

  s32 n = x * y;

  s32 t = (x - 1) * (y - 1);

  s32 e[128] = {0};
  s32 d[128] = {0};

  int k = 0;

  for (s32 i = 2; i < t; i++)
  {
    if (t % i == 0)
      continue;

    if (!isPrime(i))
      continue;

    if (i == x || i == y)
      continue;

    e[k] = i;

    s32 td = dec(i, t);

    if (td > 0)
    {
      d[k] = td;

      k++;
    }

    if (k == 127)
      break;
  }

  s32 ii = rand() % k;

  s32 ee = e[ii];
  s32 dd = d[ii];

  m_x = x;
  m_y = y;
  m_n = n;
  m_t = t;
  m_e = ee;
  m_d = dd;

  /*
  const char* src = "Hello World!, how are you";
  s32 dst_i[128] = {0};
  s8  dst_c[128] = {0};

  for (s32 i = 0; i < strlen(src); i++)
  {
    s32 c = src[i];

    c = c;// - 96;

    s32 k = 1;

    for (s32 j = 0; j < ee; j++)
    {
      k = k * c;
      k = k % n;
    }

    dst_i[i] = k;// + 96;
  }

  for (s32 i = 0; i < strlen(src); i++)
  {
    s32 c = dst_i[i];

    c = c;// - 96;

    s32 k = 1;

    for (s32 j = 0; j < dd; j++)
    {
      k = k * c;
      k = k % n;
    }

    dst_c[i] = k;// + 96;
  }
  */

  return true;
}

s32 kgmEncryptor::encrypt(void* src, s32 size, void** dst)
{
  if (!src || !size)
    return 0;

  s8* s = (s8*) src;
  s16* pe = (s16*) malloc(size * 2);

  for (s32 i = 0; i < size; i++)
  {
    s16 c = s[i];

    s32 k = 1;

    for (s32 j = 0; j < m_e; j++)
    {
      k = k * c;
      k = k % m_n;
    }

    pe[i] = (s16) k;
  }

  *dst = pe;

  return 2 * size;
}

s32 kgmEncryptor::decrypt(void* src, s32 size, void** dst)
{
  s16 *s = (s16*) src;

  size /= 2;

  u8* pd = (u8*) malloc(size);

  for (s32 i = 0; i < size; i++)
  {
    s16 c = s[i];

    s32 k = 1;

    for (s32 j = 0; j < m_d; j++)
    {
      k = k * c;
      k = k % m_n;
    }

    pd[i] = (u8) k;
  }

  *dst = pd;

  return size;
}

bool kgmEncryptor::isPrime(s32 n)
{
  s32 h = static_cast<u32>(sqrt(n));

  if (n < 3)
    return false;

  for(s32 i = 2; i <= h; i++)
  {
    if(n % i == 0)
      return false;
  }

  return true;
}


s32 kgmEncryptor::genPrime()
{
  s32 f = MAX_S32 / 2;

  srand(kgmTime::getTicks());

  s32 i = rand() % 100;

  while(i < f)
  {

    if (isPrime(i))
    {
      s32 m = rand() % 10;

      if (!m)
      {
        return i;
      }
    }

    i++;
  }

  return 0;
}

s32 kgmEncryptor::dec(s32 a, s32 t)
{
  s32 k = 1;

  while(1)
  {
    k = k + t;

    if (k % a == 0)
      return k / a;
  }

  return 0;
}
