#include "../kgmPhysics/kgmBody.h"

class kgmUnit;

class kgmPBody : public kgmBody
{
  KGM_OBJECT(kgmPBody);

  public:
    kgmPBody(kgmUnit*);
    ~kgmPBody();
};