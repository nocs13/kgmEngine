#pragma once 
#include "kgmArray.h"

typedef unsigned int u32;

template <class T> class kgmList;
template <class T> class kgmArray;

template <class T> 
class kgmList
{
private:
  struct _Node
  {
    T      data;
    _Node  *prev, *next;

    _Node()
    {
      prev = next = 0;
    }

    inline void* operator new(size_t size)
    {
      return kgm_alloc(size);
    }

    inline void  operator delete(void *p)
    {
      if(p) 
        kgm_free(p);
    }
  };

public:
  //// ITERATOR
  class iterator
  {
    friend class kgmList<T>;
    _Node* _Ptr;

  public:
    iterator()
    {
      _Ptr = NULL;
    }

    T& operator*()
    {
      return _Ptr->data;
    }

    void operator++()
    {
      if(_Ptr) _Ptr = _Ptr->next;
    }

    void operator--()
    {
      if(_Ptr) _Ptr = _Ptr->prev;
    }

    bool operator!=(iterator i)
    {
      return (_Ptr != i._Ptr);
    }

    bool operator==(iterator i)
    {
      return (_Ptr == i._Ptr);
    }

    bool next()
    {
      if(_Ptr)
        _Ptr = _Ptr->next;

      if(_Ptr)
        return true;

      return false;
    }

    bool prev()
    {
      if(_Ptr)
        _Ptr = _Ptr->prev;

      if(_Ptr)
        return true;

      return false;
    }

    bool end()
    {
      return (_Ptr == null);
    }

    bool haveNext()
    {
      if (!_Ptr || !_Ptr->next)
        return false;

      return true;
    }
  };

protected:
  _Node* _First;
  u32    csize;

  _Node *_first()
  {
    return _First;
  }

  _Node *_last()
  {
    _Node *node = _First;

    while(node && node->next)
      node = node->next;

    return node;
  }

public:
  kgmList():_First(NULL),csize(0)
  {
    _First = NULL;
    csize  = 0;
  }

  ~kgmList()
  {
    clear();
  }

  T& operator[](unsigned int i)
  {
    _Node *node = _First;

    while(node && i)
    {
      node = node->next;
      i--;
    }

    return node->data;
  }

  int size()
  {
    return csize;
  }

  int length()
  {
    return csize;
  }

  void push_back(T t)
  {
    add(t);
  }

  void add(T t)
  {
    _Node *node = new _Node();

    node->data = t;

    _Node *lnode = _last();

    if(!lnode)
    {
      _First = node;
    }
    else
    {
      node->prev = lnode;
      lnode->next = node;
    }

    csize++;
  }

  void append(T t)
  {
    _Node *node = new _Node();

    node->data = t;

    _Node *lnode = _last();

    if(!lnode)
    {
      _First = node;
    }
    else
    {
      node->prev = lnode;
      lnode->next = node;
    }

    csize++;
  }

  iterator erase(iterator i)
  {
    if(!i._Ptr)
      return i;

    _Node *prev, *next;

    prev = i._Ptr->prev;
    next = i._Ptr->next;

    if(prev)
      prev->next = next;

    if(next)
      next->prev = prev;

    delete i._Ptr;

    if(i._Ptr == _First)
      _First = i._Ptr = next;
    else
      i._Ptr = next;

    csize--;

    if (!csize && _First)
    {
      exit(EXIT_FAILURE);
    }

    return i;
  }

  void erase(u32 i)
  {
    _Node *node = _First;

    if(i >= csize)
      return;

    while(node && i)
    {
      node = node->next;
      i--;
    }

    if(node)
    {
      if(node->prev)
        node->prev->next = node->next;

      if(node->next)
        node->next->prev = node->prev;

      if(node == _First)
      {
        _First = _First->next;
      }

      delete node;

      csize--;
    }

    if (!csize && _First)
    {
      exit(EXIT_FAILURE);
    }
  }

  void erase(T val)
  {
    _Node *node = _First;

    while(node)
    {
      if(node->data == val)
        break;

      node = node->next;
    }

    if(node)
    {
      if(node->prev)
        node->prev->next = node->next;

      if(node->next)
        node->next->prev = node->prev;

      if(node == _First)
      {
        _First = _First->next;
      }

      delete node;

      csize--;
    }

    if (!csize && _First)
    {
      exit(EXIT_FAILURE);
    }
  }

  void clear()
  {
    _Node *node = _First;

    while(node)
    {
      _Node *prev = node;
      node = node->next;

      delete prev;
    }

    _First = 0;
    csize = 0;
  }

  iterator begin()
  {
    iterator i;
    i._Ptr = _First;

    return i;
  }

  iterator get(T val)
  {
    iterator i;
    i._Ptr = _First;

    do {
      if (i._Ptr->data == val)
        break;

      i._Ptr = i._Ptr->next;
    } while(!i.end());

    return i;
  }

  bool has(T val)
  {
    _Node *node = _First;

    while(node)
    {
      if(node->data == val)
        return true;

      node = node->next;
    }

    return false;
  }
};
