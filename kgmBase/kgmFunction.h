#ifndef KGMFUNCTION_H
#define KGMFUNCTION_H

// Callback function should be __stdcall attribute.
template <class T> class kgmFunction {};

/*template <class Res, class... Args>
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

template <class Obj>
class kgmFunction<void (Obj*)>
{
  typedef __stdcall void (*SFunction)(Obj*);
  typedef __stdcall void (Obj::*MFunction)();

  union Pointers
  {
    SFunction f_static;
    MFunction f_member;
  };

  typedef __stdcall void Callback(Pointers&, Obj&);

  Pointers pointers;
  Callback *callback;

  Obj       *object;

  static void call_sfunction(Pointers& pts, Obj& obj)
  {
    (*pts.f_static)(&obj);
  }

  static void call_mfunction(Pointers& pts, Obj& obj)
  {
    (obj.*(pts.f_member))();
  }

 public:
  kgmFunction() : callback(null), object(null)
  {
  }

  kgmFunction(void (*func)(Obj*))
  {
    pointers.f_static = func;
    callback = &call_sfunction;
  }

  kgmFunction(void (Obj::*func)())
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

  void operator()(Obj& obj)
  {
    (*callback)(pointers, obj);
  }

  bool valid()
  {
    return (pointers.f_static != null);
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
  kgmFunction() : callback(null), object(null)
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
*/

#endif // KGMFUNCTION_H
