#ifndef KGMENCRYPTOR_H
#define KGMENCRYPTOR_H

#include "kgmObject.h"

class kgmEncryptor : public kgmObject
{
  KGM_OBJECT(kgmEncryptor);

  s32 m_y, m_x, m_n, m_t, m_e, m_d, m_s;
public:
  kgmEncryptor();

  bool init();
  bool init(s32 pr_x, s32 pr_y, s32 pr_e, s32 pr_d);

  s32 getX();
  s32 getY();
  s32 getE();
  s32 getD();

  s32 encrypt(void* src, s32 size, void** dst);
  s32 decrypt(void* src, s32 size, void** dst);

private:
  bool isPrime(s32 n);
  s32 genPrime();
  s32 dec(s32 a, s32 t);
};

#endif // KGMENCRYPTOR_H
