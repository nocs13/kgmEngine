#ifndef KGMHARRAY_H
#define KGMHARRAY_H

#define KGM_HARRAY_DEFAULT_SIZE 63

template <class T>
class kgmHArray
{
  struct Node {
    void* key;
    u32   key_len;
    T     data;

    Node *next;
  };

  Node *root;

public:
  kgmHArray(u32 size = KGM_HARRAY_DEFAULT_SIZE)
  {
    root = new Node[size];
    memset(root, 0, size * sizeof(Node));
  }
};

#endif // KGMHARRAY_H
