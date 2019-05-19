#ifndef KGMSORT_H
#define KGMSORT_H

template <class T>
class kgmSort
{
public:

  //Return true if a < b
  typedef bool (*Compare)(T& a, T& b);

  static bool Check(T& a, T& b)
  {
    return (a < b);
  }

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

    sort(data, len);
  }

protected:
  void sort(T* data, u32 len)
  {
    Compare compare;

    if (check)
      compare = check;
    else
      compare = compare_default;

    for (u32 i = 0; i < (len - 1); i++) {
      for (u32 j = i + 1; j < len; j++) {
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

template <class T>
class kgmSortMerge
{
  typedef bool (*Compare)(T& a, T& b);

  Compare check;

public:
  kgmSortMerge(T* data, u32 len, Compare compare = nullptr)
  {
    if (compare)
      check = compare;

    sort(data, 0, len - 1);
  }

  void merge(int arr[], int l, int m, int r)
  {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
      L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
      R[j] = arr[m + 1+ j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2)
    {
      if (L[i] <= R[j])
      {
        arr[k] = L[i];
        i++;
      }
      else
      {
        arr[k] = R[j];
        j++;
      }
      k++;
    }

    while (i < n1)
    {
      arr[k] = L[i];
      i++;
      k++;
    }

    while (j < n2)
    {
      arr[k] = R[j];
      j++;
      k++;
    }
  }

  void sort(int arr[], int l, int r)
  {
    if (l < r)
    {
      int m = l+(r-l)/2;

      sort(arr, l, m);
      sort(arr, m+1, r);

      merge(arr, l, m, r);
    }
  }
};

template <class T>
class kgmSortHeap
{
  typedef bool (*Compare)(T& a, T& b);

  Compare check;

public:
  kgmSortHeap(T* data, u32 len, Compare compare = nullptr)
  {
    if (compare)
      check = compare;
    else
      check = Check;

    sort(data, len);
  }

  void sort(T arr[], u32 n)
  {
    for (u32 i = n / 2 - 1; i >= 0; i--)
      heapify(arr, n, i);

    for (u32 i = n - 1; i >= 0; i--)
    {
      T t = arr[0];
      arr[0] = arr[i];
      arr[i] = t;

      heapify(arr, i, 0);
    }
  }

  void heapify(T arr[], u32 n, u32 i)
  {
    u32 largest = i;

    u32 l = 2 * i + 1;
    u32 r = 2 * i + 2;

    //if (l < n && arr[l] > arr[largest])
    if (l < n && check(arr[largest], arr[l]))
      largest = l;

    //if (r < n && arr[r] > arr[largest])
    if (l < n && check(arr[largest], arr[r]))
      largest = r;

    if (largest != i)
    {
      T t = arr[i];
      arr[i] = arr[largest];
      arr[largest] = t;

      heapify(arr, n, largest);
    }
  }

  static bool Check(T& a, T& b)
  {
    return (a <= b);
  }
};

template <class T>
class kgmSortShell
{
  typedef bool (*Compare)(T& a, T& b);

  Compare check;

public:
  kgmSortShell(T* data, u32 len, Compare compare = nullptr)
  {
    if (compare)
      check = compare;

    sort(data, len);
  }

  void sort(T arr[], u32 n)
  {
    u32 i,j,k;

    T t;

    for (k = n/2; k > 0; k /= 2)
    {
      for (i = k; i < n; i++)
      {
        t = arr[i];

        for (j = i; j >= k; j -= k)
        {
          if (t < arr[j - k])
            arr[j] = arr[j - k];
          else
            break;
        }

        arr[j] = t;
      }
    }
  }
};

template <class T>
class kgmSortQuick
{
  typedef bool (*Compare)(T& a, T& b);

  Compare check;

public:
  kgmSortQuick(T* data, u32 len, Compare compare = nullptr)
  {
    if (compare)
      check = compare;
    else
      check = Check;

    sort(data, len);
  }

  void sort(T arr[], u32 n)
  {
    quicksort(arr, 1, n);
  }

  void quicksort(T* arr, u32 low, u32 hight)
  {
    u32 p_index;

    if (low < hight)
    {
      p_index = partition(arr, low, hight);

      quicksort(arr, low, hight - 1);
      quicksort(arr, low + 1, hight);
    }
  }

  s32 partition(T* arr, s32 low, s32 hight)
  {
    s32 pivot = hight;

    s32 i = (low - 1);

    for (u32 j = low; j <= (hight - 1); j++)
    {
      if (check(arr[j], arr[pivot]))
      {
        i++;

        swap(arr, i, j);
      }
    }

    swap(arr, i + 1, hight);
  }

  void swap(T* arr, s32 a, s32 b)
  {
    T t = arr[a];

    arr[a] = arr[b];
    arr[b] = t;
  }

  static bool Check(T& a, T& b)
  {
    return (a <= b);
  }
};

#endif // KGMSORT_H
