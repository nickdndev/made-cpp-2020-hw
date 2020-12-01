
template <typename F> class functor {

public:
  F f;

  functor(F f) : f(f) {}

  template <typename I> decltype(auto) operator()(I value) {
    return f(value);
  }
};

template <typename F, typename G> class composite {
public:
  F f;
  G g;

  composite(F _f, G _g) : f(_f), g(_g) {}

  template <typename I> decltype(auto) operator()(I value) {
    return g(f(value));
  }
};

template <typename F> decltype(auto) compose(F f) { return functor<F>(f); }

template <typename F, typename G> decltype(auto) compose(F f, G g) {
  return composite<F, G>{f, g};
}

template <typename F, typename... Fs> decltype(auto) compose(F f, Fs &&... fs) {
  return compose(f, compose(fs...));
}
