#pragma once
#include "kgmList.h"
#include "kgmInterface.h"


#define KGM_OBJECT(o_class)                                         \
  public:                                                           \
  static unsigned int  cSize(){ return sizeof(class o_class); }     \
  static const char*   cClass(){ return #o_class; }                 \
  virtual unsigned int vSize(){ return sizeof(class o_class); }     \
  virtual const char*  vClass(){ return #o_class; }                 \
  private:

class kgmApp;

class kgmObject
{
  KGM_OBJECT(kgmObject)

  friend class kgmApp;

protected:
  template <class... Args> class Signal;

  class BaseSlot
  {
  public:
    virtual ~BaseSlot(){}
  };

  template <class... Args>
  class AbstractSlot: public BaseSlot
  {
  protected:
    virtual ~AbstractSlot()
    {
      for (int i = 0; i < signals.length(); i++)
        signals[i]->disconnect(this);
    }

    friend class Signal<Args...>;

    virtual void add(Signal<Args...>* e)
    {
      if (signals.has(e) == false)
        signals.add(e);
    }

    virtual void remove(Signal<Args...>* e)
    {
      signals.erase(e);
    }

    virtual void call(Args... args) = 0;
    virtual void call(kgmObject* sender, Args... args) = 0;

  protected:
    kgmList< Signal<Args...>* > signals;
    kgmObject*                sender;
  };

  template <class T, class... Args>
  class Slot: public AbstractSlot<Args...>
  {
  public:
    typedef void(T::*FN)(Args...);
    typedef void(T::*FNS)(kgmObject*, Args...);

  public:
    Slot()
    {
      this->t      = null;
      this->f      = null;
      this->sender = null;
    }

    bool connect(T* t, FN f, Signal<Args...> *s)
    {
      if (this->t != null || this->f != null)
        return false;

      this->t = t;
      this->f = f;

      AbstractSlot<Args...>::add(s);
      s->connect(this);

      return true;
    }

    bool connect(T* t, FNS f, kgmObject* S, Signal<Args...> *s)
    {
      if (this->t != null || this->f != null)
        return false;

      this->t = t;
      this->f = (FN) f;

      s->sender(S);

      AbstractSlot<Args...>::add(s);
      s->connect(this);

      return true;
    }

    void reset()
    {
      this->t      = null;
      this->f      = null;
      this->sender = null;
    }

  private:
    Slot(const Slot&)
    {

    }

    void operator=(const Slot&)
    {

    }

    friend class Signal<Args...>;

    virtual void call(Args... args)
    {
      if(!t || !f)
        return;

      (t->*f)(args...);
    }

    virtual void call(kgmObject* sender, Args... args)
    {
      if(!t || !f)
        return;

      FNS ff = (FNS) f;
      (t->*ff)(sender, args...);
    }

  private:
    T* t;
    FN f;
  };

  template<class... Args>
  class Signal
  {
    kgmList<AbstractSlot<Args...>*> list;
    kgmObject* sobject = null;

  public:
    Signal()
    {

    }

    Signal(kgmObject *s)
    {
      sobject = s;
    }

    ~Signal()
    {
      for(int i = 0; i < list.length(); i++)
        list[i]->remove(this);
    }

    void sender(kgmObject* s)
    {
      sobject = s;
    }

    kgmObject* sender() const
    {
      return sobject;
    }

    void connect(AbstractSlot<Args...> *s)
    {
      if(!s || list.has(s))
        return;

      list.add(s);

      s->add(this);
    }

    void disconnect(AbstractSlot<Args...> *s)
    {
      if (s)
        s->remove(this);

      list.erase(s);
    }

    void operator()(Args... args)
    {
      for(int i = 0; i < list.length(); i++)
      {
        if(sobject)
        {
          list[i]->call(sobject, args...);
        }
        else
        {
          list[i]->call(args...);
        }
      }
    }
  };

private:
  u32 m_references = 0;

private:
  static void* kgm_object_alloc(size_t size);
  static void kgm_object_free(void* p);

protected:
  virtual ~kgmObject();
  static void kgm_objects_cleanup();

public:
  kgmObject();
  kgmObject(const kgmObject& o);

  bool isClass(const char* o);

  u32 references() const
  {
    return m_references;
  }

  void increment()
  {
    m_references++;
  }

  void release()
  {
    if(m_references > 0)
      m_references--;

    if (m_references == 0)
      delete this;
  }

  void assign(kgmObject** o)
  {
    if (!o)
      return;

    if (*o)
      (*o)->release();

    *o = this;

    increment();
  }

  void* operator new(size_t size)
  {
    return kgm_object_alloc(size);
  }

  void operator delete(void* p)
  {
    kgm_object_free(p);
  }

  static void Release(kgmObject* o);
};
