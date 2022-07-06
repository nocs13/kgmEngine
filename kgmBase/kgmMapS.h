#pragma once

template <class Key, class Data>
class kgmMapS
{
  struct Node {
    Key  key;
    Data data;

    Node* left;
    Node* right;

    Node(Key k, Data d): left(null), right(null) {
      key  = k;
      data = d;
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
    friend class kgmMapS;

    Node* _Ptr;
    u32   depth;

    kgmMapS<Key, Data>* object;

  public:
    iterator(kgmMapS<Key, Data>* o = null)
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

      Node* right = _Ptr->right;

      if (!right) {
        _Ptr = null;

        return *this;
      } else {
        _Ptr = right;
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
  kgmMapS()
  {
    root = null;
  }

  ~kgmMapS()
  {
    clear();
  }

  void set(Key k, Data d)
  {
    if (!root) {
      root = new Node(k, d);

      return;
    }

    Node* c = root;

    bool e = false;

    c = find(k);

    if (c) {
      c->data = d;

      return;
    } else {
      c = lastright(root);

      Node *n = new Node(k, d);

      n->left = c;
      c->right = n;
    }
  }

  void remove(Key k)
  {
    Node* n = find(k);

    if (!n)
      return;

    Node* r = n->right;
    Node* l = n->left;

    if (r)
      r->left = l;

    if (l)
      l->right = r;

    delete n;
  }

  iterator get(Key k)
  {
    Node* n = find(k);

    iterator i(this);

    if (!n)
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

private:

  Node* find(Key k)
  {
    Node* c = root;

    while (c) {
      if (c->key == k)
        return c;

      c = c->right;
    }

    return null;
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
};
