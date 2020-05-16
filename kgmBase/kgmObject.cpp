#include "kgmObject.h"
#include "kgmLog.h"

kgmObject::kgmObject()
{
}

kgmObject::kgmObject(const kgmObject& o)
{
}

kgmObject::~kgmObject()
{
#ifdef DEBUG
    kgm_log() << "kgmObject::~kgmObject [" << (void*)this << "]\n";
#endif
}

bool kgmObject::isClass(const char* o)
{
  return !strcmp(this->vClass(), o);
}
