#ifndef KGMPAIR_H
#define KGMPAIR_H

template <class T1, class T2> struct kgmPair
{
  T1 key;
  T2 value;

  kgmPair(T1 k, T2 v)
  {
    key = k;
    value = v;
  }

  ~kgmPair()
  {

  }
};

#endif // KGMPAIR_H
