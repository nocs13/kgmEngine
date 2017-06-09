#pragma once
#include "kgmList.h"
#include "kgmIObject.h"

class kgmObject: public kgmIObject
{
  KGM_OBJECT(kgmObject);

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
      for(int i = 0; i < signals.length(); i++)
        signals[i]->disconnect(this);
    }

    friend class Signal<Args...>;

    virtual void add(Signal<Args...>* e)
    {
      signals.add(e);
    }

    virtual void remove(Signal<Args...>* e)
    {
      signals.erase(e);
    }

    virtual void call(Args... args) = 0;

  protected:
    kgmList<Signal<Args...>*> signals;
  };

  template <class T, class... Args>
  class Slot: public AbstractSlot<Args...>
  {
  public:
    typedef void(T::*FN)(Args...);

  public:
    Slot()
    {
      this->t = null;
      this->f = null;
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

    void reset()
    {
      this->t = null;
      this->f = null;
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

  private:
    T* t;
    FN f;
  };

  template<class... Args>
  class Signal
  {
    kgmList<AbstractSlot<Args...>*> list;

  public:
    Signal()
    {

    }

    ~Signal()
    {
      for(int i = 0; i < list.length(); i++)
        list[i]->remove(this);
    }

    void connect(AbstractSlot<Args...> *s)
    {
      if(!s)
        return;

      list.add(s);

      s->add(this);
    }

    void disconnect(AbstractSlot<Args...> *s)
    {
      list.erase(s);
    }

    void operator()(Args... args)
    {
      for(int i = 0; i < list.length(); i++)
        list[i]->call(args...);
    }
  };

private:

public:
  kgmObject()
  {
  }

  kgmObject(const kgmObject& o)
  {
  }

  virtual ~kgmObject()
  {
  }

  virtual bool isClass(const char* o)
  {
    return !strcmp(this->vClass(), o);
  }
};
