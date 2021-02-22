#ifndef KGMTUPLE_H
#define KGMTUPLE_H

template <class F, class... R>
struct kgmTuple: public kgmTuple<R...> {
  kgmTuple(F f, R... r): kgmTuple<R...>(r...), f(f) { }

  F f;
};

template <class F>
struct kgmTuple<F> {
  kgmTuple(F f): f(f) {}

  F f;
};

template <int i, class F, class... R>
struct kgmTuple_GetImpl {
  static auto value(const kgmTuple<F, R...>* t) -> decltype(kgmTuple_GetImpl<i - 1, R...>::value(t)) {
    return kgmTuple_GetImpl<i - 1, R...>::value(t);
  }
};

template <class F, class... R>
struct kgmTuple_GetImpl<0, F, R...> {
  static F value(const kgmTuple<F, R...>* t) {
    return t->f;
  }
};

template <int i, class F, class... R>
auto kgmTuple_Get(const kgmTuple<F, R...>& t) -> decltype(kgmTuple_GetImpl<i, F, R...>::value(&t)) {
  return kgmTuple_GetImpl<i, F, R...>::value(&t);
}

#endif // KGMTUPLE_H
