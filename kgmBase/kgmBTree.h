#ifndef KGMBTREE_H
#define KGMBTREE_H

template <class K, class D>
class kgmBTree
{
  struct Node {
    K     key;
    D     data;

    Node* left;
    Node* right;

    Node(K k, D d):left(null), right(null) {
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
    friend class kgmHArray<Key, Data>;
    Node*            _Ptr;
    u32              depth;
    kgmBTreey<K, D>* object;

  public:
    iterator(kgmBTreey<K, D>* o = null)
    {
      _Ptr   = NULL;
      depth  = 0;
      object = null;

      if (o) {
        object = o;
        _Ptr   = o->root;
        depth  = 0;

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
      if(_Ptr) {
        _Ptr = _Ptr->next;

        return;
      }

      if(index < object->base_size) {
        while(!object->root[index] && index < (object->base_size - 1))
          index++;
        _Ptr = object->root[index];
      }
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
      if(!_Ptr && index == (object->base_size - 1))
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
          break;
        } else {
          n = n->right;
        }
      } else {
        if (!n->left) {
          n->left = new Node(k, d);
          break;
        } else {
          n = n->left;
        }
      }
    }
  }

private:
  void insert(K k, D d) {

  }
};

#endif // KGMBTREE_H
