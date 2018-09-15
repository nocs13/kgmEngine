#ifndef KGMGAMEEXTENTION_H
#define KGMGAMEEXTENTION_H

#include "../kgmBase/kgmInterface.h"

typedef kgmGameExtention* (*KGMGETGAMEEXTENTION)();

class kgmGameExtention: public kgmInterface
{

};

#endif // KGMGAMEEXTENTION_H
