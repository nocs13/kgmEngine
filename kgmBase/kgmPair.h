#ifndef KGMPAIR_H
#define KGMPAIR_H

template <class K, class V> struct kgmPair
{
  K _key;
  V _value;

  kgmPair(K k, V v)
  {
    _key = k;
    _value = v;
  }

  ~kgmPair()
  {

  }

  bool operator==(K key){
    if (key == _key)
      return true;

    return false;
  }

  K& key() {
    return _key;
  }

  V& value() {
    return value;
  }
};

#endif // KGMPAIR_H
