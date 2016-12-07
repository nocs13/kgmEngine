#ifndef KGMCALLBACK_H
#define KGMCALLBACK_H

// Callback function should be __stdcall attribute.
template <class T> class kgmFunction {};

template <class Res, class... Args>
  class kgmFunction<Res (Args...)>
{
 public:
  typedef __stdcall Res(*Function)(Args...);

 private:
  Function function;
  
public:
  kgmFunction(Function fn = null)
    : function(fn)
  {
  }

  Res operator()(Args... args)
  {
    return function(args...);
  }

  bool valid()
  {
    return (function != null);
  }
};

template <class... Args>
class kgmFunction<void (Args...)>
{
public:
  typedef __stdcall void(*Function)(Args...);

private:
  Function function;

  public:
  kgmFunction(Function fn = null)
    : function(fn)
  {
  }

  void operator()(Args... args)
  {
    function(args...);
  }

  bool valid()
  {
    return (function != null);
  }
};


template <class Res, class Obj, class... Args>
  class kgmFunction<Res (Obj*, Args...)>
{
  typedef __stdcall Res(*SFunction)(Obj*, Args...);
  typedef __stdcall Res(Obj::*MFunction)(Args...);

  union Pointers
  {
    SFunction f_static;
    MFunction f_member;
  };

  typedef __stdcall Res Callback(Pointers&, Obj&, Args...);

  Pointers pointers;
  Callback *callback;
  
  Obj       *object;

  static Res call_sfunction(Pointers& pts, Obj& obj, Args... args)
  {
    return (*pts.f_static)(&obj, args...);
  }

  static Res call_mfunction(Pointers& pts, Obj& obj, Args... args)
  {
    return (obj.*(pts.f_member))(args...);
  }

 public:
  kgmFunction() : callback(null)
  {
  }
  
  kgmFunction(Res (*func)(Obj*, Args...))
  {
    pointers.f_static = func;
    callback = &call_sfunction;
  }

  kgmFunction(Res (Obj::*func)(Args...))
  {
    pointers.f_member = func;
    callback = &call_mfunction;
  }

  kgmFunction(const kgmFunction& fn)
  {
    pointers = fn.pointers;
    callback = fn.callback;
  }

  kgmFunction& operator=(const kgmFunction& fn)
  {
    pointers = fn.pointers;
    callback = fn.callback;

    return *this;
  }

  Res operator()(Obj& obj, Args... args)
  {
    return (*callback)(pointers, obj, args...);
  }
  
  bool valid()
  {
    return (pointers.f_static != null);
  }
};

#endif // KGMCALLBACK_H
