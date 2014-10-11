#ifndef KGMCALLBACK_H
#define KGMCALLBACK_H

// Callback function should be __stdcall attribute.

template <class Res, class Obj, class... Args> class kgmCallback
{
public:
  typedef __stdcall Res(*Function)(Args...);

private:
  typedef __stdcall Res(*Fn)(Args...);
  typedef __stdcall Res(*Fno)(Obj, Args...);

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

  kgmCallback(Obj obj = null, Function fn = null)
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
