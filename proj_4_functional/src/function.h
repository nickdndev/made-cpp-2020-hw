
#include <array>
#include <cstddef>
#include <memory>
#include <utility>
#include <variant>

template <typename> class function;

template <typename R, typename... Args> class function<R(Args...)> {

private:
  class container;

  template <typename Functor> class function_container;
  std::unique_ptr<container> _executor;

public:
  function() noexcept : _executor(nullptr) {}

  function(nullptr_t) noexcept : _executor(nullptr) {}

  ~function() {}

  template <typename Functor> function(Functor functor) : _executor() {
    _executor = std::make_unique<function_container<Functor>>(functor);
  }

  function(function const &other) : _executor() {
    _executor = other._executor->copy();
  }

  function(function &&other) : _executor() {
    _executor = std::move(other._executor);
  }

  function &operator=(function const &other) {
    function(other).swap(*this);
    return *this;
  }

  function &operator=(function &&other) {
    function(other).swap(*this);
    return *this;
  }

  R operator()(Args... __args) const {
    return _executor->invoke(std::forward<Args>(__args)...);
  }

  operator bool() const noexcept { return _executor != nullptr; }

  void swap(function &other) noexcept { std::swap(_executor, other._executor); }

private:
  class container {
  public:
    container() = default;

    virtual ~container() = default;

    virtual R invoke(Args &&... args) = 0;

    virtual std::unique_ptr<container> copy() const = 0;
  };

  template <typename Functor> class function_container : public container {
  public:
    explicit function_container(Functor functor)
        : container(), _functor(functor) {}

    explicit function_container(std::remove_reference_t<Functor> &&functor)
        : container(), _functor(functor) {}

    R invoke(Args &&... args) override {
      return _functor(std::forward<Args>(args)...);
    }

    std::unique_ptr<container> copy() const override {
      return std::make_unique<function_container<Functor>>(_functor);
    }

  private:
    Functor _functor;
  };
};
