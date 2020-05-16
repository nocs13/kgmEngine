#include "kgmEvent.h"
#include "kgmLog.h"

kgmEvent::~kgmEvent()
{
#ifdef DEBUG
    kgm_log() << "kgmEvent::~kgmEvent [" << (void*)this << "]\n";
#endif
}
