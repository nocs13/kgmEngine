#ifndef KGMCALLBACK_H
#define KGMCALLBACK_H

// Callback function should be __stdcall attribute.
template <class Ret, class Obj, class... Args>
class kgmCallback
{
public:
  typedef Ret(*Function)(Args...);

private:
  typedef Ret(*Fn)(Args...);
  typedef Ret(*Fno)(Obj, Args...);

  Obj object;

  union
  {
    Fn  function;
    Fno functiono;
  };

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

  Ret operator()(Args... args)
  {
    if(object)
      return (*functiono)(object, args...);

    return (*function)(args...);
  }

  bool hasObject()
  {
    return (object != null);
  }

  bool hasFunction()
  {
    return (function != null);
  }

  void* getObject()
  {
    return object;
  }
};
/*
template <class Ret, class... Args>
class kgmCallback
{
public:
  typedef Ret(*Function)(Args...);

  Function  function;

public:
  kgmCallback(Function fn = null)
  {
    function = fn;
  }

  virtual Ret operator()(Args... args)
  {
    return (*function)(args...);
  }
};

template <class Ret, class Obj, class... Args>
class kgmCallbackMethod: public kgmCallback<Ret, Args...>
{
public:
  typedef Ret(Obj::*Function)(Args...);

  Function  function;
  Obj       object;

public:
  kgmCallbackMethod(Obj ob = null, Function fn = null)
  {
    function = fn;
    object   = ob;
  }

  Ret operator()(Args... args)
  {
    return (object->*function)(args...);
  }
};

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
