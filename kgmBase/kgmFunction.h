#ifndef KGMFUNCTION_H
#define KGMFUNCTION_H

#include "kgmPointer.h"

// Callback function should be __stdcall attribute.
template <typename>
class kgmFunction;

template <typename Return, typename... Args>
class kgmFunction<Return(Args...)>
{
  class ICall {
  public:
    virtual Return invoke(Args...) = 0;
  };

  template<typename T>
  class Call: public ICall
  {
    T m_t;
  public:
    Call(const T& t)
      : m_t(t)
    {
      
    }

    Return invoke(Args... args)
    {
      return m_t(args...);
    }
  };

  //ICall* m_call = nullptr;
  kgmUPointer<ICall> m_call;
  
public:
  template <typename T>
  kgmFunction& operator=(T t)
  {
    m_call = kgmUPointer<ICall>(new Call<T>(t));
    
    return *this;
  }

  Return operator()(Args... args)
  {
    assert(m_call.valid());

    return m_call->invoke(args...);
  }
};

/*
template <typename... Args>
class kgmFunction<void(Args...)>
{
  class ICall {
  public:
    virtual void invoke(Args...) = 0;
  };

  template<typename T>
  class Call: public ICall
  {
    T m_t;
  public:
    Call(const T& t)
      : m_t(t)
    {

    }

    void invoke(Args... args)
    {
      m_t(args...);
    }
  };

  ICall* m_call = nullptr;
public:

  template <typename T>
  kgmFunction& operator=(T t)
  {
    return *this;
  }

  void operator()(Args... args)
  {
    assert(m_call);

    m_call->invoke(args...);
  }
};
*/

#endif // KGMFUNCTION_H
