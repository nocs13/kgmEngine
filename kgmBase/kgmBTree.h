#ifndef KGMBTREE_H
#define KGMBTREE_H

template <class K, class D>
class kgmBTree
{
  struct Node {
    K     key;
    D     data;

    Node* par;
    Node* left;
    Node* right;

    Node(K k, D d): par(null), left(null), right(null) {
      key  = k;
      data = d;
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
    Node*            _Ptr;
    u32              depth;
    kgmBTreey<K, D>* object;

  public:
    iterator(kgmBTreey<K, D>* o = null)
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
      if(!_Ptr)
        return true;

      return false;
    }
  };

public:
  kgmBTree():root(null){}

  void add(K k, D d) {
    if (!root) {
      root = new Node(k, d);

      return;
    }

    Node* n = root;

    while (true) {
      if (key == n->key) {
        n->data = data;
        return;
      } else if (key > n->key) {
        if (!n->right) {
          n->right = new Node(k, d);
          n->right->par = n;
          break;
        } else {
          n = n->right;
        }
      } else {
        if (!n->left) {
          n->left = new Node(k, d);
          n->left->par = n;
          break;
        } else {
          n = n->left;
        }
      }
    }
  }

  void remove(K k) {

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

private:
  void insert(K k, D d) {

  }
};

#endif // KGMBTREE_H
