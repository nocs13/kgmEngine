#ifndef KGMSORT_H
#define KGMSORT_H

template <class T>
class kgmSort
{
public:

  //Return true if a < b
  typedef bool (*Compare)(T& a, T& b);

protected:
  T*  data;
  u32 len;
  Compare check;

public:
  kgmSort(T* array, u32 length, Compare compare = nullptr)
  {
    data  = array;
    len   = length;
    check = compare;

    sort_simple(data, len);
  }

protected:
  void sort_simple(T* data, u32 len)
  {
    Compare compare;

    if (check)
      compare = check;
    else
      compare = compare_default;

    for (u32 i = 0; i < (length - 1); i++) {
      for (u32 j = i + 1; j < length; j++) {
        if (compare(data[j], data[i])) {
          T t = data[j];
          data[j] = data[i];
          data[i] = t;
        }
      }
    }
  }

  static bool compare_default(T& a, T& b)
  {
    if (a < b)
      return true;

    return false;
  }
};

#endif // KGMSORT_H
