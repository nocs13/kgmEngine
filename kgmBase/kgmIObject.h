#ifndef KGMIOBJECT
#define KGMIOBJECT

#define KGM_OBJECT(o_class)                                         \
  public:                                                           \
  static unsigned int  cSize(){ return sizeof(class o_class); }     \
  static const char*   cClass(){ return #o_class; }                 \
  virtual unsigned int vSize(){ return sizeof(class o_class); }   \
  virtual const char*  vClass(){ return #o_class; }               \
  private:

class kgmIObject
{
  KGM_OBJECT(kgmIObject);

public:
  kgmIObject()
  {
  }

  virtual ~kgmIObject()
  {
  }

  bool isClass(const char* o)
  {
    return !strcmp(this->vClass(), o);
  }
};

#endif // KGMIOBJECT

