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

    ~Node()
    {
      if (left)
        delete left;

      if (right)
        delete right;
    }
  };

public:
  friend class iterator;

  class iterator
  {
    friend class kgmMap;

    Node* _Ptr;
    u32   depth;

    kgmMap<Key, Data>* object;

  public:
    iterator(kgmMap<Key, Data>* o = null)
    {
      _Ptr   = NULL;
      object = o;

      if (o)
      {
        Node* s = o->root;

        if (s)
        {
          while(true)
          {
            if (s->left != null)
            {
              s = s->left;
            }
            else if (s->right != null)
            {
              s = s->right;
            }
            else
            {
              break;
            }
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

    iterator operator++()
    {
      if (!_Ptr)
        return *this;

      Node* par = _Ptr->par;

      if (!par) {
        _Ptr = null;

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
    
    bool isValid()
    {
      return (_Ptr != null);
    }

    Key key()
    {
      return _Ptr->key;
    }

    Data data()
    {
      return _Ptr->data;
    }
  };

private:
#define MIN_DEPTH_UPDATE 8

  friend class iterator;

  Node* root;

  u32   t_depth;

public:
  kgmMap()
  {
    root   = null;
    t_depth = MIN_DEPTH_UPDATE;
  }

  ~kgmMap()
  {
    clear();
  }

  void set(Key k, Data d)
  {
    s32 h = kgmHash<Key>(k)();

    if (!root) {
      root = new Node(k, d, h);

      return;
    }

    Node* c = root;

    bool e = false;

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

      if (h < c->hkey) {
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

    if ((rand() % (t_depth)) == 0) {
      t_depth = height();

      if (t_depth < MIN_DEPTH_UPDATE)
        t_depth = MIN_DEPTH_UPDATE;
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

    n->left = n->right = null;

    if (n == root)
      root = null;

    delete n;
  }

  iterator get(Key k)
  {
    s32 h = kgmHash<Key>(k)();
    bool e = false;

    Node* n = find(h, k, e);

    iterator i(this);

    if (!n || !e)
      i._Ptr = null;
    else
      i._Ptr = n;

    return i;
  }

  void clear()
  {
    if (root)
      delete root;

    root = null;
    t_depth = MIN_DEPTH_UPDATE;
  }

  u32 height()
  {
    return height(root);
  }

  iterator begin()
  {
    iterator i(this);

    return i;
  }

  iterator end() {
    iterator i(this);
    i._Ptr = null;

    return i;
  }

  // for testing
  void print()
  {
    print(root);
  }

  void print(Node* p, s32 indent = 0)
  {
    /*if(p != NULL) {
      if(p->left)
        print(p->left, indent + 4);
      if(p->right)
        print(p->right, indent + 4);

      for (int i = 0; i < indent; i++)
        printf(" ");
      printf("%i/%i\n", (int)p->key, (int)p->data);
    }*/
    if (!p)
      return;

    printf("%i %i/%i\n", indent, (int)p->key, (int)p->data);
    indent++;
    print(p->left, indent);
    print(p->right, indent);
  }
  // ***

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
        if (c->left)
          c = c->left;
        else
          break;
      else
        if (c->right)
          c = c->right;
        else
          break;
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
};

#endif // KGMBTREE_H
