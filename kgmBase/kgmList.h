#pragma once
#include "kgmBase.h"
#include "kgmArray.h"

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

    _Node*      _prev;
    kgmList<T>* _list;

  public:
    iterator()
    {
      _Ptr = NULL;
      _prev = null;
      _list = null;
    }

    T& operator*()
    {
      return _Ptr->data;
    }

    void operator++(int)
    {
      if(_Ptr) {
        _prev = _Ptr;
        _Ptr = _Ptr->next;
      }
    }

    void operator--()
    {
      if(_Ptr && _Ptr->prev) {
        _Ptr  = _Ptr->prev;
        _prev = _Ptr->prev;
      }
    }

    bool operator!=(iterator i)
    {
      if (i._list != _list)
      {
        //kgm_fatal("Not valid list sources.");
      }

      return (_Ptr != i._Ptr);
    }

    bool operator==(iterator i)
    {
      if (i._list != _list)
        kgm_fatal("Not valid list sources.");

      return (_Ptr == i._Ptr);
    }

    bool next()
    {
      if (_Ptr) {
        _prev = _Ptr;
        _Ptr = _Ptr->next;

        return true;
      }

      return false;
    }

    bool prev()
    {
      if (_Ptr && _Ptr->prev) {
        _Ptr = _Ptr->prev;
        _prev = _Ptr->prev;

        return true;
      }

      return false;
    }

    /*
    bool end()
    {
      if (!_Ptr)
        return true;

      return false;
    }
    */

    bool haveNext()
    {
      if (!_Ptr || !_Ptr->next)
        return false;

      return true;
    }
  };

protected:
  _Node* _First;
  _Node* _Last;

  u32    csize;

  _Node *_first()
  {
    return _First;
  }

  _Node *_last()
  {
    return _Last;
  }

public:
  kgmList():_First(NULL),csize(0)
  {
    _First = _Last = NULL;
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

    if (node == null)
    {
      kgm_fatal("Invalid start node.");
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
      _First = _Last = node;
    }
    else
    {
      node->prev = lnode;

      lnode->next = _Last = node;
    }

    csize++;
  }

  void append(T t)
  {
    add(t);
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
    csize--;

    i._Ptr = next;

    if (!prev)
      _First = i._Ptr;

    if (!next)
      _Last = i._Ptr;

    if (!csize && (_First || _Last))
    {
      kgm_fatal("Invalid iterator for erase.");
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

      if(node == _Last)
      {
        _Last = _Last->prev;
      }

      delete node;

      csize--;
    }

    if (!csize && (_First || _Last))
    {
      kgm_fatal("Invalid node for delete.");
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

      if(node == _Last)
      {
        _Last = _Last->prev;
      }

      delete node;

      csize--;
    }

    if (!csize && (_First || _Last))
    {
      kgm_fatal("Invalid node for delete.");
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

    _First = _Last = 0;
    csize = 0;
  }

  iterator begin()
  {
    iterator i;

    i._Ptr = _First;
    i._list = this;

    return i;
  }

  iterator last()
  {
    iterator i;

    i._Ptr = _Last;
    i._list = this;

    return i;
  }

  iterator end()
  {
    iterator i;

    i._Ptr  = null;
    i._list = this;
    i._prev = _Last;

    return i;
  }

  iterator get(T val)
  {
    iterator i;

    i._Ptr = _First;

    do
    {
      if (i._Ptr->data == val)
        break;

      i._Ptr = i._Ptr->next;
    } while(i.haveNext());

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
