#include "../kgmAI/kgmIAI.h"
#include "../kgmSystem/kgmThread.h"

class kgmGameAI: public kgmIAI, public kgmObject
{
  KGM_OBJECT(kgmGameAI);

  kgmList<Unit>     m_units;
  kgmList<UnitType> m_types;

  kgmThread::Thread m_thread;
  kgmThread::Mutex  m_mutex;

  bool m_active = false;

public:
  kgmGameAI(kgmIGame* g);
  ~kgmGameAI();

  bool start();
  bool finish();
  
  void update();
  bool addType(kgmString type);
  bool addState(kgmString type, State state);
  bool addInput(kgmString type, Input inpout);
  bool addUnit(kgmUnit* unit);

private:
  static int fn_thread(void* m);
};