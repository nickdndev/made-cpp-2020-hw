#pragma once

#include "src/ptr.cpp"
#include <atomic>
namespace task {


template <typename T> class DefaultDelete {
public:
  constexpr DefaultDelete() noexcept = default;

  template <typename U> DefaultDelete(const DefaultDelete<U> &) noexcept {}

  void operator()(T *p) const { delete p; }
};


template <typename T, typename D = DefaultDelete<T>> class ControlBlock {
public:
  using element_type = T;
  using deleter_type = D;

  ControlBlock(T *p) : _impl{p} {}

  ControlBlock(T *p, D d) : _impl{p, d} {}

  ~ControlBlock() {}

  void inc_ref() noexcept { ++_use_count; }

  void inc_wref() noexcept { ++_weak_use_count; }

  void dec_ref() noexcept {
    auto _ptr = _impl._impl_ptr();
    auto &_deleter = _impl._impl_deleter();
    if (--_use_count == 0) {
      if (_ptr)
        _deleter(_ptr);
      dec_wref();
    }
  }

  void dec_wref() noexcept {
    if (--_weak_use_count == 0) {
      delete this;
    }
  }

  long use_count() const noexcept { return _use_count; }

  bool unique() const noexcept { return _use_count == 1; }

  long weak_use_count() const noexcept {
    return _weak_use_count - ((_use_count > 0) ? 1 : 0);
  }

  bool expired() const noexcept { return _use_count == 0; }

  void *get_deleter() noexcept {
    return reinterpret_cast<void *>(std::addressof(_impl._impl_deleter()));
  }

private:
  std::atomic<long> _use_count{1};
  std::atomic<long> _weak_use_count{1};
  Ptr<T, D> _impl;
};
template <typename T, typename D = DefaultDelete<T>> class UniquePtr {
public:
  using pointer = T *;
  using element_type = T;
  using deleter_type = D;

  constexpr UniquePtr() noexcept;

  explicit UniquePtr(pointer p) noexcept;

  UniquePtr(UniquePtr &&up) noexcept;

  ~UniquePtr() noexcept;

  UniquePtr &operator=(UniquePtr &&up) noexcept;

  element_type &operator*() const noexcept;

  pointer operator->() const noexcept;
  pointer get() const noexcept;

  deleter_type &get_deleter() noexcept;

  pointer release() noexcept;
  void reset(pointer p) noexcept;

  void swap(UniquePtr &up) noexcept;

private:
  Ptr<T, D> _impl;
};
template <typename T> class SharedPtr;
template <typename T> class WeakPtr;

template <typename T, bool = std::is_array<T>::value,
    bool = std::is_void<T>::value>
class shared_ptr_access {
public:
  using element_type = T;

  element_type &operator*() const noexcept {
    assert(_get() != nullptr);
    return *_get();
  }

  element_type *operator->() const noexcept {
    assert(_get() != nullptr);
    return _get();
  }

private:
  element_type *_get() const noexcept {
    return static_cast<const SharedPtr<T> *>(this)->get();
  }
};

template <class T> class SharedPtr {
public:
  template <typename U> friend class SharedPtr;

  template <typename U> friend class WeakPtr;

  using element_type = typename shared_ptr_access<T>::element_type;
  using weak_type = WeakPtr<T>;

  constexpr SharedPtr() noexcept;

  template <typename U> explicit SharedPtr(U *p);

  SharedPtr(const SharedPtr &sp) noexcept;

  template <typename U> SharedPtr(SharedPtr<U> &&sp) noexcept;

  template <typename U> explicit SharedPtr(const WeakPtr<U> &wp);

  ~SharedPtr();
  SharedPtr &operator=(const SharedPtr &);

  SharedPtr &operator=(SharedPtr &&);

  template <typename U> SharedPtr &operator=(SharedPtr<U> &&) noexcept;

  void swap(SharedPtr &sp) noexcept;

  void reset() noexcept;

  template <typename U> void reset(U *p);

  element_type *get() const noexcept;

  element_type &operator*() const noexcept;

  element_type *operator->() const noexcept;

  long use_count() const noexcept;

private:
  element_type *_ptr;
  ControlBlock<T> *_control_block;
};

template <class T> class WeakPtr {
public:
  template <typename U> friend class SharedPtr;

  template <typename U> friend class WeakPtr;

  using element_type = typename std::remove_extent<T>::type;

  constexpr WeakPtr() noexcept;

  template <class U>
  WeakPtr(SharedPtr<U> const &sp) noexcept;

  WeakPtr(WeakPtr const &wp) noexcept;

  template <class U>
  WeakPtr(WeakPtr<U> const &wp) noexcept;

  template <typename U> WeakPtr(WeakPtr<U> &&sp) noexcept;

  ~WeakPtr();

  WeakPtr &operator=(const WeakPtr &wp) noexcept;

  template <typename U>
  WeakPtr &operator=(const WeakPtr<U> &wp) noexcept;

  template <typename U>
  WeakPtr &operator=(const SharedPtr<U> &sp) noexcept;

  void swap(WeakPtr &wp) noexcept;

  long use_count() const noexcept;

  bool expired() const noexcept;

  SharedPtr<T> lock() const noexcept;

private:
  element_type *_ptr;
  ControlBlock<T> *_control_block;
};

// Your template function definitions may go here...

} // namespace task

#include "smart_pointers.tpp"