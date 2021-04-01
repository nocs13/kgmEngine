#ifndef KGMPOINTER_H
#define KGMPOINTER_H

// Shared pointer.

template <class T>
class kgmPointer
{
  T*   pointer = 0;
  int* counter = 0;

  template <class A, class B>
  friend  kgmPointer<A> kgm_ptr_cast(kgmPointer<B>& var);
public:

  kgmPointer()
  {
    pointer = 0L;
    counter = 0;
  }

  kgmPointer(T* ptr)
  {
    if(ptr == 0L)
      return;

    counter = new int;

    *counter = 1;

    pointer = ptr;
  }

  kgmPointer(const kgmPointer<T> &object)
  {
    pointer = object.pointer;
    counter = object.counter;

    if(pointer)
      (*counter)++;
  }

  ~kgmPointer()
  {
    if(counter == 0)
      return;

    (*counter)--;

    if((*counter) > 0)
      return;

    *counter = 0;
    
    delete counter;

    if(pointer)
      delete pointer;

    counter = 0;
    pointer = 0;
  }

  /*
  void operator=(const T& data)
  {
    (*pointer) = data;
  }
  */

  void operator=(const kgmPointer<T>& ptr)
  {
    if(pointer)
    {
      (*counter)--;

      if((*counter) < 1)
      {
        delete pointer;
        delete counter;

        pointer = 0;
        counter = 0;
      }
    }

    pointer = ptr.pointer;
    counter = ptr.counter;

    if(pointer)
      (*counter)++;
  }

  T* operator->() const
  {
    return pointer;
  }

  operator T*() const
  {
    return pointer;
  }

  bool valid()
  {
    if(pointer != 0 && (*counter) > 0)
      return true;

    return false;
  }

  void reset()
  {
    if(counter == 0)
      return;

    (*counter)--;

    if((*counter) < 1)
    {
      delete counter;

      if(pointer)
        delete pointer;
    }

    counter = 0;
    pointer = 0;
  }
};

template <class A, class B>
kgmPointer<A> kgm_ptr_cast(kgmPointer<B>& var)
{
  kgmPointer<A> cast;

  if(var.pointer != null)
  {
    cast.pointer = (A*) var.pointer;
    cast.counter = var.counter;

    (*var.counter)++;
  }

  return cast;
}

// Unique pointer.

template <class T>
class kgmUPointer
{
  T*   pointer = null;

public:

  kgmUPointer()
  {
    pointer = null;
  }

  kgmUPointer(T* ptr)
  {
    if(!ptr)
      return;

    if (pointer)
      delete pointer;

    pointer = ptr;
  }

  kgmUPointer(const kgmUPointer<T> &o)
  {
    if(!o.pointer)
      return;

    pointer = o.pointer;
    o.pointer = null;
  }

  ~kgmUPointer()
  {
    if(pointer)
      delete pointer;

    pointer = null;
  }

  void operator=(const kgmUPointer<T>& o)
  {
    if(!o.pointer)
      return;

    if (pointer)
      delete pointer;

    pointer = o.pointer;
    
    o.pointer = null;
  }

  T* operator->() const
  {
    return pointer;
  }

  T* operator*()
  {
    return pointer;
  }

  bool valid()
  {
    if(pointer)
      return true;

    return false;
  }

  void reset()
  {
    if(pointer)
      delete pointer;
    
    pointer = 0;
  }
};

#define kgm_ptr  kgmPointer
#define kgm_uptr kgmUPointer

#endif // KGMPOINTER_H
