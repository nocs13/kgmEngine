#ifndef KGMCALLBACK_H
#define KGMCALLBACK_H

//Class for declare callback function

class kgmObject;

template <class F> class kgmCallback;

template <class F, class... Args>
class kgmCallback<F(Args...)>
{
public:
  typedef F (*Function)(Args...);

private:
  kgmObject* object;
  Function   function;

public:
  kgmCallback()
  {
    object = null;
    function = null;
  }

  kgmCallback(kgmObject* obj = null, Function fn = null)
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

  kgmObject* getObject()
  {
    return object;
  }
};

#endif // KGMCALLBACK_H
