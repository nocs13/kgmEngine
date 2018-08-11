#pragma once
#include "kgmTypes.h"
#include "kgmString.h"

struct kgmHashFunction
{
  static u32  simple(u8* buf, s32 len)
  {
    u32 hash = 0;

    if(!buf || !len)
      return 0;

    for(s32 i = 0; i < len; i++)
    {
      hash &= (((hash << 5) - hash) + buf[i]);
    }

    return hash;
  }

  static u32 h37(u8* str, u32 len)
  {
    u32 hash = 2139062143;

    for(u32 i = 0; i < len; i++)
       hash = 37 * hash + str[i];

    return hash;
  }

  static u32 faq6(u8* str, s32 len)
  {
    u32 hash = 0;

    for(s32 i = 0; i < len; i++)
    {
      hash += (u8) str[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
  }

  static u32 rot13(u8* str, s32 len)
  {
    u32 hash = 0;

    for(s32 i = 0; i < len; i++)
    {
      hash += (u8) str[i];
      hash -= (hash << 13) | (hash >> 19);
    }

    return hash;
  }

  // recomended

  static u32 ly(u8* str, s32 len)
  {
    u32 hash = 0;

    for(s32 i = 0; i < len; i++)
      hash = (hash * 1664525) + (u8)(str[i]) + 1013904223;

    return hash;
  }

  // recomended

  static u32 rs(u8* str, s32 len)
  {
    static const u32 b = 378551;
    u32 a = 63689;
    u32 hash = 0;

    for(s32 i = 0; i < len; i++)
    {
      hash = hash * a + (u8)(str[i]);
      a *= b;
    }

    return hash;
  }
};

template <class K>
class kgmHash
{
public:
  enum Type
  {
    HfnNone,
    HfnSimple,
    HfnH37,
    HfnFaq6,
    HfnRot13,
    HfnLy,
    HfnRs
  };

private:
  u32  hash;
  Type type;

public:
  kgmHash(K key)
  {
    type = HfnNone;

    if (type != HfnNone)
    {
      switch((u32) type)
      {
      case HfnSimple:
        hash = kgmHashFunction::simple((u8*) &key, sizeof(key));
        return;
      case HfnH37:
        hash = kgmHashFunction::h37((u8*) &key, sizeof(key));
        return;
      case HfnFaq6:
        hash = kgmHashFunction::faq6((u8*) &key, sizeof(key));
        return;
      case HfnRot13:
        hash = kgmHashFunction::rot13((u8*) &key, sizeof(key));
        return;
      case HfnLy:
        hash = kgmHashFunction::ly((u8*) &key, sizeof(key));
        return;
      case HfnRs:
        hash = kgmHashFunction::rs((u8*) &key, sizeof(key));
        return;
      }
    }

    hash = kgmHashFunction::rs((u8*)&key, sizeof(key));
  }

  u32 operator()()
  {
    return hash;
  }
};

template <>
class kgmHash<kgmString>
{
public:
  enum Type
  {
    HfnNone,
    HfnSimple,
    HfnH37,
    HfnFaq6,
    HfnRot13,
    HfnLy,
    HfnRs
  };

private:
  u32  hash;
  Type type;

public:
  kgmHash(kgmString& key)
  {
    type = HfnNone;

    if (type != HfnNone)
    {
      switch((u32) type)
      {
      case HfnSimple:
        hash = kgmHashFunction::simple((u8*)key.data(), key.length());
        return;
      case HfnH37:
        hash = kgmHashFunction::h37((u8*)key.data(), key.length());
        return;
      case HfnFaq6:
        hash = kgmHashFunction::faq6((u8*)key.data(), key.length());
        return;
      case HfnRot13:
        hash = kgmHashFunction::rot13((u8*)key.data(), key.length());
        return;
      case HfnLy:
        hash = kgmHashFunction::ly((u8*)key.data(), key.length());
        return;
      case HfnRs:
        hash = kgmHashFunction::rs((u8*)key.data(), key.length());
        return;
      }
    }

    hash = kgmHashFunction::rs((u8*)key.data(), key.length());
  }

  u32 operator()()
  {
    return hash;
  }
};

template <>
class kgmHash<const char*>
{
public:
  enum Type
  {
    HfnNone,
    HfnSimple,
    HfnH37,
    HfnFaq6,
    HfnRot13,
    HfnLy,
    HfnRs
  };

private:
  u32  hash;
  Type type;

public:
  kgmHash(const char* key)
  {
    type = HfnNone;

    if (type != HfnNone)
    {
      switch((u32) type)
      {
      case HfnSimple:
        hash = kgmHashFunction::simple((u8*)key, strlen(key));
        return;
      case HfnH37:
        hash = kgmHashFunction::h37((u8*)key, strlen(key));
        return;
      case HfnFaq6:
        hash = kgmHashFunction::faq6((u8*)key, strlen(key));
        return;
      case HfnRot13:
        hash = kgmHashFunction::rot13((u8*)key, strlen(key));
        return;
      case HfnLy:
        hash = kgmHashFunction::ly((u8*)key, strlen(key));
        return;
      case HfnRs:
        hash = kgmHashFunction::rs((u8*)key, strlen(key));
        return;
      }
    }

    hash = kgmHashFunction::rs((u8*)key, strlen(key));
  }

  u32 operator()()
  {
    return hash;
  }
};
