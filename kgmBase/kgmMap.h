#ifndef KGMBTREE_H
#define KGMBTREE_H

#include "kgmHash.h"

template <class Key, class Data>
class kgmMap
{
  struct Node {
    Key  key;
    Data data;

    s32  hkey;

    Node* par;
    Node* left;
    Node* right;

    Node(Key k, Data d, s32 h = 0): par(null), left(null), right(null) {
      key  = k;
      data = d;
      hkey = h;
    }
  };



public:
  //// ITERATOR


  friend class iterator;

  class iterator
  {
    friend class Tree;

    Node* _Ptr;
    u32   depth;

    kgmMap<Key, Data>* object;

  public:
    iterator(kgmMap<Key, Data>* o = null)
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

  friend class iterator;

  Node* root;

  u32   t_depth;

public:
  kgmMap()
  {
    root   = null;
    t_depth = 8;
  }

  void set(Key k, Data d)
  {
    s32 h = kgmHash<Key>(k)();

    if (!root) {
      root = new Node(k, d, h);

      return;
    }

    Node* c = root;

    bool e;

    c = find(h, k, e);

    if (c && e) {
      c->data = d;

      return;
    }

    if (!c)
      c = root;

    while (true) {
      if ((rand() % t_depth) == 0) {
        Node *n = new Node(k, d, h);

        if (c->par) {
          if (c == c->par->left)
            c->par->left = n;
          else
            c->par->right = n;
        }

        if (c->hkey < n->hkey)
          n->left = c;
        else
          n->right = c;

        n->par = c->par;
        c->par = n;

        break;
      }

      if (c->hkey < h) {
        if (!c->left) {
          c->left = new Node(k, d, h);
          c->left->par = c;

          break;
        } else {
          c = c->left;
        }
      } else {
        if (!c->right) {
          c->right = new Node(k, d, h);
          c->right->par = c;

          break;
        } else {
          c = c->right;
        }
      }
    }

    if ((rand() % 10) == 0) {
      t_depth = height();
    }
  }

  void remove(Key k)
  {
    s32 h = kgmHash<Key>(k)();

    bool e;

    Node* n = find(h, k, e);

    if (!n || !e)
      return;

    Node* p = n->par;

    s32 side = 0;

    if (p) {
      if (n == p->left)
        side = -1;
      else
        side =  1;
    }

    if (!n->left && !n->right) {
      if (p) {
        if (side == -1)
          p->left = null;
        else
          p->right = null;
      }
    } else if (n->left && n->right) {
      Node* l = lastleft(n->right);

      l->left = n->left;
      n->left->par = l;

      if (side == -1) {
        p->left = n->right;
        n->right->par = p;
      } else if (side == 1) {
        p->right = n->right;
        n->right->par = p;
      } else {
        root = n->right;
        root->par = null;
      }
    } else if (n->left) {
      if (side == -1) {
        p->left = n->left;
        n->left->par = p;
      } else if (side == 1) {
        p->right = n->left;
        n->left->par = p;
      } else {
        root = n->left;
        root->par = null;
      }
    } else {
      if (side == -1) {
        p->left = n->right;
        n->right->par = p;
      } else if (side == 1) {
        p->right = n->right;
        n->right->par = p;
      } else {
        root = n->right;
        root->par = null;
      }
    }

    if (n == root)
      root = null;

    delete n;
  }

  void reset()
  {

  }

  u32 height()
  {
    return height(root);
  }

private:
  u32 height(Node* n)
  {
    if (!n)
      return 0;

    u32 hl = height(n->left);
    u32 hr = height(n->right);

    return 1 + ((hl > hr)?(hl):(hr));
  }

  Node* find(s32 h, Key k, bool& e)
  {
    Node* c = root;

    e = false;

    while (c) {
      if (h == c->hkey && c->key == k) {
        e = true;

        break;
      }

      if (h < c->hkey)
        c = c->left;
      else
        c = c->right;
    }

    return c;
  }

  Node* lastleft(Node* n)
  {
    if (!n)
      return null;

    while (n->left)
      n = n->left;

    return n;
  }

  Node* lastright(Node* n)
  {
    if (!n)
      return null;

    while (n->right)
      n = n->right;

    return n;
  }

  s32 hash(Key k)
  {
    kgmHash<Key> h(k);

    return (s32) h();
  }

  iterator begin()
  {
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
