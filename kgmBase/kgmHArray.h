#ifndef KGMHARRAY_H
#define KGMHARRAY_H

#include "kgmHash.h"
#include "kgmString.h"

#define KGM_HARRAY_DEFAULT_SIZE 63


template <class Key, class Data>
class kgmHArray
{
  struct _Node {
    Key  key;
    Data data;

    _Node *next;
  };

  _Node **root;

  u32    base_size;

public:
  //// ITERATOR

  class iterator;

  friend class iterator;

  class iterator
  {
    friend class kgmHArray<Key, Data>;
    _Node*        _Ptr;
    u32           index;
    kgmHArray<Key, Data>* object;

  public:
    iterator(kgmHArray<Key, Data> *o = null)
    {
      _Ptr   = NULL;
      index  = 0;
      object = null;

      if (o) {
        object = o->object;
        _Ptr   = o->_Ptr;
        index  = o->index;
      }
    }

    iterator(const iterator& it)
    {
      object = it.object;
      _Ptr   = it._Ptr;
      index  = it.index;
    }

    Data& operator*()
    {
      return _Ptr->data;
    }

    void operator++()
    {
      if(_Ptr)
        _Ptr = _Ptr->next;

      if(!_Ptr){
        if(index < object->base_size) {
          while(!object->root[index] && index < object->base_size)
            index++;
          _Ptr = object->root[index];
        }
      }
    }

    bool operator!=(iterator i)
    {
      return (_Ptr != i._Ptr);
    }

    bool operator==(iterator i)
    {
      return (_Ptr == i._Ptr);
    }

    bool isEnd()
    {
      if(!_Ptr && index == (object->base_size - 1))
        return true;

      return false;
    }

  };

public:
  kgmHArray(u32 size = KGM_HARRAY_DEFAULT_SIZE)
  {
    root = new _Node*[size];
    memset(root, 0, size * sizeof(_Node*));

    base_size = size;
  }

  void set(Key key, Data data) {
    kgmHash<Key> hash(key);

    u32 index = hash() % base_size;

    _Node* node = new _Node();

    node->key = key;
    node->data = data;
    node->next = 0;

    if(!root[index])
      root[index] = node;
    else
      getLast(root[index])->next = node;
  }

  Data& get(Key key) const {
    _Node* n = find(key);

    if(!n)
      return Data(0);

    return n->data;
  }


  bool exist(Key key) {
    if(!find(key))
      return false;

    return true;
  }


private:
  void clear()
  {

  }

  _Node* getLast(_Node* n) {
    if (!n)
      return n;

    while(n->next) {
      n = n->next;
    }

    return n;
  }

  _Node* find(Key key) {
    kgmHash<Key> hash(key);

    u32 index = hash() % base_size;

    if(!root[index])
      return 0;

    _Node* n = root[index];

    while(n){
      if(n->key == key)
        return n;

      n = n->next;
    }

    return 0;
  }
};

#endif // KGMHARRAY_H
