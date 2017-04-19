#ifndef KGMBTREE_H
#define KGMBTREE_H

#include "kgmHash.h"

template <class Key, class Data>
class kgmMap
{
  struct Node {
    Key  key;
    Data data;

    s32  hash;

    Node* par;
    Node* left;
    Node* right;

    Node(Key k, Data d, s32 h): par(null), left(null), right(null) {
      key  = k;
      data = d;
      hash = h;
    }
  };

  Node* root;

public:
  //// ITERATOR

  class iterator;

  friend class iterator;

  class iterator
  {
    friend class kgmBTree<Key, Data>;

    Node* _Ptr;
    u32   depth;

    kgmMap<Key, Data>* object;

  public:
    iterator(kgmMap<K, D>* o = null)
    {
      _Ptr   = NULL;
      object = o;

      if (o) {
        Node* s = o->root;

        while(true) {
          if(s->left != nullptr) {
            s = s->left;
          } else if(s->right != nullptr) {
            s = s->right;
          } else {
            break;
          }
        }

        _Ptr = s;
      }
    }

    iterator(const iterator& it)
    {
      object = it.object;
      _Ptr   = it._Ptr;
    }

    Data& operator*()
    {
      return _Ptr->data;
    }

    void operator++()
    {
      if (!_Ptr)
        return *this;

      Node* par = _Ptr->par;

      if (!par) {
        _Ptr = nullptr;

        return *this;
      }

      if (_Ptr == par->left && par->right) {
        _Ptr = par->right;
      } else {
        _Ptr = par;

        return *this;
      }

      while (true) {
        if (_Ptr->left)
          _Ptr = _Ptr->left;
        else if (_Ptr->right)
          _Ptr = _Ptr->right;
        else
          break;
      }

      return *this;
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
      return (!_Ptr);
    }
  };

public:
  kgmMap():root(null){}

  void set(Key k, Data d) {
    kgmHash<Key> hash(k);
    s32 h = (s32)hash();

    if (!root) {
      root = new Node(k, d, h);

      return;
    }

    Node* n = root;

    while (true) {
      if (h == n->hash && k == n->key) {
        n->data = d;
        return;
      } else if (h >= n->hash) {
        if (!n->right) {
          n->right = new Node(k, d, h);
          n->right->par = n;
          break;
        } else {
          n = n->right;
        }
      } else {
        if (!n->left) {
          n->left = new Node(k, d, h);
          n->left->par = n;
          break;
        } else {
          n = n->left;
        }
      }
    }
  }

  void remove(Key k) {

  }

  iterator begin() {
    iterator i(this);

    return i;
  }

  iterator end() {
    iterator i(this);
    i._Ptr = nullptr;

    return i;
  }
};

#endif // KGMBTREE_H
