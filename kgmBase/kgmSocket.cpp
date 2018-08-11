#include "kgmSocket.h"

#include <unistd.h>

kgmSocket::kgmSocket(s32 proto, s32 type)
{
  protocol = proto;
  domain = type;

  handle = socket(type, proto, 0);
}

kgmSocket::~kgmSocket()
{
  clear();
}

void kgmSocket::clear()
{
  if(handle > 0)
    close(handle);
}
