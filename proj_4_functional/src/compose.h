

template <class F, class G> auto compose(F f, G g) {
  return [f, g](auto x) { return f(g(x)); };
}

template <class F, typename... Fs> auto compose(F f, Fs &&... fs) {
  return compose(f, compose(fs...));
}