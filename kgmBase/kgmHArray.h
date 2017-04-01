#ifndef KGMHARRAY_H
#define KGMHARRAY_H

#include "kgmHash.h"

#define KGM_HARRAY_DEFAULT_SIZE 63

template <class T>
class kgmHArray
{
  struct _Node {
    void* key;
    s32   key_len;
    T     data;

    _Node *next;
  };

  _Node **root;

  u32    base_size;

public:
  //// ITERATOR
  class iterator
  {
    friend class kgmHArray<T>;
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
      if(_Ptr)
        _Ptr = _Ptr->next;
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
  };

public:
  kgmHArray(u32 size = KGM_HARRAY_DEFAULT_SIZE)
  {
    root = new Node*[size];
    memset(root, 0, size * sizeof(Node*));

    base_size = size;
  }

  void set(void* key, s32 key_len, T data) {
    u8 hash = kgmHash::rs(key, key_len);

    hash %= base_size;

    Node* node = new Node();

    node->key = key;
    node->key_len = key_len;
    node->data = data;
    node->next = 0;

    if(!root[hash])
      root[hash] = node;
    else
      getLast(root[hash])->next = node;
  }

  T& get(void* key, s32 key_len) const {
    Node* n = find(key, key_len);

    if(!n)
      return T(0);

    return n->data;
  }


  bool exist(void* key, s32 key_len) {
    u8 hash = kgmHash::rs(key, key_len);

    hash %= base_size;

    if(!root[hash])
      return false;
    else
      getLast(root[hash])->next = node;
  }


private:
  void clear()
  {

  }

  Node* getLast(Node* n) {
    if (!n)
      return n;

    while(n->next) {
      n = n->next;
    }

    return n;
  }

  Node* find(void* key, s32 key_len) {
    u8 hash = kgmHash::rs(key, key_len);

    hash %= base_size;

    if(!root[hash])
      return 0;

    Node* n = root[hash];

    while(n){
      if(n->key_len == key_len && !memcmp(key, n->key, key_len))
        return n;
      n = n->next;
    }

    return 0;
  }

};

#endif // KGMHARRAY_H
