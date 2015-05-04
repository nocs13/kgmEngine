#include "kgmMain.h"

template <class T> 
class kgmPointer
{
  T* m_pointer;

  unsigned int* m_references;

public:

  kgmPointer()
  {
    m_pointer = null;
    m_references = null;
  }
};
