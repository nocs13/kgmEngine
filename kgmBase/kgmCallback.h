#ifndef KGMCALLBACK_H
#define KGMCALLBACK_H

// Callback function should be __stdcall attribute.
template <class Res, class... Args>
class kgmCallback
{
public:
  typedef Res(*Function)(Args...);

  Function  function;

public:
  kgmCallback(Function fn = null)
  {
    function = fn;
  }

  virtual Res operator()(Args... args)
  {
    return (*function)(args...);
  }
};

template <class Res, class Obj, class... Args>
class kgmCallbackMethod: public kgmCallback<Res, Args...>
{
public:
  typedef Res(Obj::*Function)(Args...);

  Function  function;
  Obj       object;

public:
  kgmCallback(Obj ob = null, Function fn = null)
  {
    function = fn;
    object   = ob;
  }

  virtual Res operator()(Args... args)
  {
    return (object->*function)(args...);
  }
};

/*
class kgmObject;

template <class Obj>
class kgmCallback<void (Obj::*), kgmObject*>
{
public:
  typedef __stdcall void (Obj::*Function)();

private:
  Obj object;

  Function  function;

public:
  kgmCallback(): object(null), function(null)
  {
    object = null;
    function = null;
  }

  kgmCallback(Obj obj = null, Function fn = null)
  {
    object = obj;
    function = fn;
  }

  void operator()()
  {
    (Obj::*function)();
  }
};
*/
#endif // KGMCALLBACK_H
