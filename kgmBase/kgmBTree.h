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
  kgmBTree():root(null){}

  void add(K k, D d) {
    if(!root) {
      root = new Node(k, d);

      return;
    }


  }

private:
  void insert(K k, D d) {

  }
};

#endif // KGMBTREE_H
