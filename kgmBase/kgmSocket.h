#pragma once
#ifndef KGMSOCKET_H
#define KGMSOCKET_H

#include "kgmObject.h"
#include "kgmString.h"

#ifdef WIN32
#include <ws2tcpip.h>

#define AF_LOCAL AF_UNSPEC
#else
#include <sys/socket.h>
#endif

class kgmSocket : public kgmObject
{
  KGM_OBJECT(kgmSocket);

public:
  enum
  {
    ProtoTcp = SOCK_STREAM,
    ProtoUdp = SOCK_DGRAM,
  };

  enum
  {
    InetIp4   = AF_INET,
    InetIp6   = AF_INET6,
    InetLocal = AF_LOCAL
  };

private:
  s32 handle;
  s32 domain;
  s32 protocol;

public:
  kgmSocket(s32 proto = ProtoTcp, s32 type = InetIp4);
  ~kgmSocket();

  void clear();
  bool connect(kgmString ip, u16 port);
  bool listen(kgmString ip, u16 port);
  u32  read(void* buf, u32 size);
  u32  write(void* buf, u32 size);
};

#endif // KGMSOCKET_H
