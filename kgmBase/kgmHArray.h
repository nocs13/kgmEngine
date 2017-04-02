#ifndef KGMHARRAY_H
#define KGMHARRAY_H

#include "kgmHash.h"
#include "kgmString.h"

#define KGM_HARRAY_DEFAULT_SIZE 63


template <class K, class T>
class kgmHArray
{
  static u32 dataLength(K* data)
  {
    return sizeof(K);
  }

  static u32 dataLength(kgmString* data)
  {
    if(!data)
      return 0;

    return data->length();
  }

  static u32 dataLength(const char* data)
  {
    if(!data)
      return 0;

    return strlen(data);
  }

  static void* dataCore(K* data)
  {
    return data;
  }

  static void* dataCore(kgmString* data)
  {
    if(!data)
      return 0;

    return data->data();
  }

  struct _Node {
    K key;
    T data;

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
    friend class kgmHArray<K, T>;
    _Node*        _Ptr;
    u32           index;
    kgmHArray<K, T>* object;

  public:
    iterator(kgmHArray<K, T> *o = null)
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

    T& operator*()
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

  void set(K key, T data) {
    u32 hash = kgmHash::rs(dataCore(&key), dataLength(&key));

    hash %= base_size;

    _Node* node = new _Node();

    node->key = key;
    node->data = data;
    node->next = 0;

    if(!root[hash])
      root[hash] = node;
    else
      getLast(root[hash])->next = node;
  }

  T& get(K key) const {
    _Node* n = find(key);

    if(!n)
      return T(0);

    return n->data;
  }


  bool exist(K key) {
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

  _Node* find(K key) {
    u32 hash = kgmHash::rs(dataCore(key), dataLength(key));

    hash %= base_size;

    if(!root[hash])
      return 0;

    _Node* n = root[hash];

    while(n){
      if(n->key == key)
        return n;

      n = n->next;
    }

    return 0;
  }
};

#endif // KGMHARRAY_H
