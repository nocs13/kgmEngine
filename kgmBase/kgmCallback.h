#ifndef KGMCALLBACK_H
#define KGMCALLBACK_H

//Class for declare callback func

class kgmObject;

template <class F> class kgmCallback;

template <class Res, class Obj, class... Args>
class kgmCallback<Res (Obj, Args...)>
{
public:
  typedef Res(*Function)(Args...);

  typedef Res(*Fn)(Args...);
  typedef Res(*Fno)(Obj, Args...);

private:
  Obj object;

  union
  {
    Fn  function;
    Fno functiono;
  };

public:
  kgmCallback()
  {
    object = null;
    function = null;
  }

  kgmCallback(Obj obj = null, Fn fn = null)
  {
    object = obj;
    function = fn;
  }

  Res operator()(Args... args)
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

#endif // KGMCALLBACK_H
