#ifndef KGMCALLBACK_H
#define KGMCALLBACK_H

//Class for declare callback func

class kgmObject;

template <class F> class kgmCallback;

template <class F, class... Args>
class kgmCallback<F(Args...)>
{
public:
  typedef F (*Function)(Args...);

private:
  void* object;
  Function   function;

public:
  kgmCallback()
  {
    object = null;
    function = null;
  }

  kgmCallback(void* obj = null, Function fn = null)
  {
    object = obj;
    function = fn;
  }

  F operator()(Args... args)
  {
    return function(args...);
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
