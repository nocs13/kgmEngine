#pragma once 
#include "kgmBase.h"

#include "kgmHash.h"
#include "kgmString.h"

#define KGM_TAB_DEFAULT_SIZE 77

// Key hashing based virtual array.

template <class Key, class Data>
class kgmTab
{
  struct _Node {
    Key  key;
    Data data;

    _Node *next;
  };

  _Node **root;

  u32    set_count;
  u32    base_size;
  u32    data_count;

public:
  //// ITERATOR

  class iterator;

  friend class iterator;

  class iterator
  {
    friend class kgmTab<Key, Data>;
    _Node*        _Ptr;
    u32           index;
    kgmTab<Key, Data>* object;

  public:
    iterator(kgmTab<Key, Data> *o = null)
    {
      _Ptr   = NULL;
      index  = 0;
      object = null;

      if (o) {
        object = o;
        _Ptr   = o->root[0];
        index  = 0;

        if(!_Ptr)
          ++(*this);
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
      if(_Ptr && _Ptr->next) {
        _Ptr = _Ptr->next;

        return;
      }

      if(index < object->base_size) {
        while(!object->root[index] && index < (object->base_size - 1))
          index++;

        _Ptr = object->root[index];

        index++;
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

    bool end()
    {
      return isEnd();
    }

    bool valid()
    {
      if (!object)
        return false;

      return true;
    }

    void erase()
    {
      _Node* tmp = _Ptr;

      if(_Ptr && _Ptr->next) {
        delete _Ptr;

        _Ptr = tmp;

        return;
      }

      if(index < object->base_size) {
        while(!object->root[index] && index < object->base_size)
          index++;
        _Ptr = object->root[index];
      }
    }

    Key key()
    {
      return _Ptr->key;
    }

    Data& data()
    {
      return _Ptr->data;
    }

    bool isEnd()
    {
      if(!_Ptr)
        return true;

      return false;
    }
  };

public:
  kgmTab(u32 size = KGM_TAB_DEFAULT_SIZE)
  {
    if ((size % 2) == 0)
      size++;

    root = new _Node*[size];
    memset(root, 0, size * sizeof(_Node*));

    base_size = size;
    set_count = 0;

    data_count = 0;
  }

  ~kgmTab()
  {
    clear();

    delete [] root;
  }

  iterator operator[](Key k)
  {
    return get(k);
  }

  void clear()
  {
    if (!root || data_count < 1)
      return;

    for (u32 i = 0; i < base_size; i++) {
      if (root[i]) {
        _Node *c = root[i];

        while(c) {
          _Node* n = c->next;

          delete c;

          data_count--;

          c = n;
        }

        root[i] = null;
      }
    }

    if (base_size > KGM_TAB_DEFAULT_SIZE) {
      delete [] root;

      root = new _Node*[KGM_TAB_DEFAULT_SIZE];
      memset(root, 0, KGM_TAB_DEFAULT_SIZE * sizeof(_Node*));
    }

    base_size = KGM_TAB_DEFAULT_SIZE;
    set_count = 0;

    data_count = 0;
  }

  void set(Key key, Data data) {
    iterator i = get(key);

    if (i.valid() && !i.isEnd()) {
      i._Ptr->data = data;

      return;
    }

    kgmHash<Key> hash(key);

    u32 index = hash() % base_size;

    _Node* node = new _Node();

    node->key = key;
    node->data = data;
    node->next = 0;

    if (!root[index])
      root[index] = node;
    else
      getLast(root[index])->next = node;

    set_count++;

    data_count ++;

    if (set_count > (base_size / 3)) {
      u8 fk = fillkind();

      if (fk > 90) {
        rehash(base_size + base_size / 3);
      }

      set_count = 0;
    }
  }

  iterator get(Key key) {
    kgmHash<Key> hash(key);

    u32 index = hash() % base_size;

    if(!root[index])
      return iterator();

    _Node* n = root[index];

    while (n) {
      if(n->key == key) {
        iterator i(this);
        i.index = index;
        i._Ptr  = n;

        return i;
      }

      n = n->next;
    }

    return iterator();
  }

  bool exist(Key key) {
    if(!find(key))
      return false;

    return true;
  }

  void rehash(u32 size) {
    if ((size % 2) == 0)
      size++;

    _Node** croot = new _Node*[size];
    memset(croot, 0, size * sizeof(_Node*));

    for (u32 i = 0; i < base_size; i++) {
      if (root[i]) {
        kgmHash<Key> hash(root[i]->key);

        u32 index = hash() % size;

        croot[index] = root[i];
      }
    }

    delete [] root;

    root = croot;

    base_size = size;
  }

  iterator begin()
  {
    iterator i(this);

    return i;
  }

  iterator erase(iterator i)
  {
    if (i.isEnd() || !i._Ptr || i.object != this)
      return i;

    i.erase();

    if (data_count > 0)
      data_count--;

    return i;
  }

  u32 length()
  {
    return data_count;
  }

private:
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

  u8 fillkind()
  {
    u32 c = 0;

    for (u32 i = 0; i < base_size; i++)
      if (root[i])
        c++;

    return (100 * ((f32)c / (f32)(base_size)));
  }
};
