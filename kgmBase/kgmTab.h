#pragma once 
#include "kgmBase.h"

typedef unsigned int u32;
////////// Hash Tab

template<class Key, class Value> 
class kgmTab
{
private:
  struct _Node
  {
    Key    m_key;
    Value  m_value;
    _Node* m_next;
    _Node* m_prev;
  };

  _Node* m_nodes;
  u32    csize;

public:
   class iterator
   {
    friend class kgmTab<Key, Value>;
    _Node* _Ptr;

   public:
    iterator()
    {
      _Ptr = 0;
    }

    void operator++()
    {
     if(_Ptr) _Ptr = _Ptr->m_next;
    }

    void operator--()
    {
     if(_Ptr) _Ptr = _Ptr->m_prev;
    }

    bool operator!=(iterator i)
    {
     return (_Ptr != i._Ptr);
    }

    bool operator==(iterator i)
    {
     return (_Ptr == i._Ptr);
    }

    Key& key()
    {
     return _Ptr->m_key;
    }

    Value& value()
    {
     return _Ptr->m_value;
    }
   };

public:
  kgmTab()
  {
    m_nodes = null;
    csize   = 0;
  }

  ~kgmTab()
  {
  }

  Value& operator[](Key key)
  {
    _Node* node = _find(key);

    return node->m_value;
  }

  void clear()
  {
    for(int i = length(); i > 0; i--)
    {
      _Node* node = _index(i-1);

      delete node;
    }

    m_nodes = null;

    csize   = 0;
  }

  int length()
  {
    int    len  = 0;

    _Node* node = m_nodes;

    while(node)
    {
      len++;
      node = node->m_next;
    }

    return len;
  }

  bool get(u32 index, Key& key, Value& value)
  {
    _Node* node = _index(index);

    if(node == 0)
      return false;

    key = node->m_key;

    value = node->m_value;

    return true;
  }

  bool get(Key key, Value& v)
  {
    _Node* node = _find(key);

    if(node == 0)
      return false;

    v = node->m_value;

    return true;
  }

  Key key(u32 index)
  {
    return _index(index)->m_key;
  }

  Value value(u32 index)
  {
    return _index(index)->m_value;
  }

  void add(Key key, Value value)
  {
    _Node* node = new _Node;

    node->m_key   = key;
    node->m_value = value;
    node->m_next  = null;
    node->m_prev  = null;

    if(!m_nodes)
    {
      m_nodes = node;
    }
    else
    {
      _last()->m_next = node;

      node->m_prev = _last();
    }
  }

  bool hasKey(Key key)
  {
    _Node* node = _find(key);

    if(node == null)
      return false;

    return true;
  }

  iterator begin()
  {
   iterator i;

   i._Ptr = m_nodes;

   return i;
  }

  iterator end()
  {
   iterator i;

   i._Ptr = 0;

   return i;
  }

  iterator erase(iterator i)
  {
    if(!i._Ptr)
      return i;

    _Node *prev, *next;

    prev = i._Ptr->m_prev;

    next = i._Ptr->m_next;

    if(prev)
      prev->m_next = next;

    if(next)
      next->m_prev = prev;

    delete i._Ptr;

    if(i._Ptr == m_nodes)
      m_nodes = i._Ptr = next;
    else
      i._Ptr = next;

    return i;
  }

private:
  _Node* _last()
  {
    _Node* node = m_nodes;

    if(node == 0) return 0;

    while(node->m_next != 0)
      node = node->m_next;

    return node;
  }

  _Node* _index(u32 i)
  {
    _Node* node = m_nodes;

    u32 index = 0;

    while(node){
      if(i == index)
        return node;
      node = node->m_next;
      index++;
    }

    return 0;
  }

  _Node* _find(Key& key)
  {
    _Node* node = m_nodes;

    while(node)
    {
      if(node->m_key == key)
        return node;

      node = node->m_next;
    }

    return 0;
  }
};
