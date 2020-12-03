
template <typename F, typename G, typename... Args>
function<F(Args...)> compose(function<F(G)> &f, function<G(Args...)> &g) {
  return [f, g](auto... arg) { return f(g(arg...)); };
}

template <typename F, typename G, typename C, typename... Args>
function<F(C)> compose(const function<F(G)> &f, const function<G(C)> &g,
                       Args... fs) {
  function<F(C)> f_g_function = [f, g](auto x) { return f(g(x)); };
  return compose(f_g_function, fs...);
}

template <typename F, typename... Args>
function<F(Args...)> compose(function<F(Args...)> &f) {
  return f;
}