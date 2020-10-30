#include "smart_pointers.h"

namespace task {

template <typename T, typename D>
constexpr UniquePtr<T, D>::UniquePtr() noexcept = default;

template <typename T, typename D>
UniquePtr<T, D>::UniquePtr(UniquePtr::pointer p) noexcept : _impl{p} {}

template <typename T, typename D>
UniquePtr<T, D>::UniquePtr(UniquePtr &&up) noexcept
    : _impl{up.release(), std::forward<deleter_type>(up.get_deleter())} {}

template <typename T, typename D> UniquePtr<T, D>::~UniquePtr() noexcept {
  auto _ptr = _impl._impl_ptr();
  auto &_deleter = _impl._impl_deleter();
  if (_ptr)
    _deleter(_ptr);
}
template <typename T, typename D>
UniquePtr<T, D> &UniquePtr<T, D>::operator=(UniquePtr &&up) noexcept {
  reset(up.release());
  auto &_deleter = _impl._impl_deleter();
  _deleter = up.get_deleter();
  return *this;
}
template <typename T, typename D>
typename UniquePtr<T, D>::element_type &UniquePtr<T, D>::operator*() const
    noexcept {
  auto _ptr = _impl._impl_ptr();
  return *_ptr;
}
template <typename T, typename D>
typename UniquePtr<T, D>::pointer UniquePtr<T, D>::operator->() const noexcept {
  auto _ptr = _impl._impl_ptr();
  return _ptr;
}
template <typename T, typename D>
typename UniquePtr<T, D>::pointer UniquePtr<T, D>::get() const noexcept {
  return _impl._impl_ptr();
}
template <typename T, typename D>
typename UniquePtr<T, D>::deleter_type &
UniquePtr<T, D>::get_deleter() noexcept {
  return _impl._impl_deleter();
}
template <typename T, typename D>
typename UniquePtr<T, D>::pointer UniquePtr<T, D>::release() noexcept {
  auto &_ptr = _impl._impl_ptr();
  pointer cp = _ptr;
  _ptr = nullptr;
  return cp;
}
template <typename T, typename D>
void UniquePtr<T, D>::reset(UniquePtr::pointer p) noexcept {
  auto &_ptr = _impl._impl_ptr();
  auto &_deleter = _impl._impl_deleter();
  if (_ptr)
    _deleter(_ptr);
  _ptr = p;
}
template <typename T, typename D>
void UniquePtr<T, D>::swap(UniquePtr &up) noexcept {
  using std::swap;
  swap(_impl, up._impl);
}

template <class T>
constexpr SharedPtr<T>::SharedPtr() noexcept : _ptr{}, _control_block{} {}

template <class T>
template <typename U>
SharedPtr<T>::SharedPtr(U *p)
    : _ptr{p}, _control_block{new ControlBlock<U>{p}} {}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr &sp) noexcept
    : _ptr{sp._ptr}, _control_block{sp._control_block} {
  if (_control_block)
    _control_block->inc_ref();
}

template <class T>
template <typename U>
SharedPtr<T>::SharedPtr(SharedPtr<U> &&sp) noexcept
    : _ptr{sp._ptr}, _control_block{sp._control_block} {
  sp._ptr = nullptr;
  sp._control_block = nullptr;
}
template <class T>
template <typename U>
SharedPtr<T>::SharedPtr(const WeakPtr<U> &wp)
    : _ptr{wp._ptr}, _control_block{wp._control_block} {
  _control_block->inc_ref();
}
template <class T> SharedPtr<T>::~SharedPtr() {
  if (_control_block)
    _control_block->dec_ref();
}
template <class T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &sp) {
  SharedPtr{sp}.swap(*this);
  return *this;
}

template <class T> SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&sp) {
  SharedPtr{std::move(sp)}.swap(*this);
  return *this;
}

template <class T> void SharedPtr<T>::swap(SharedPtr &sp) noexcept {
  using std::swap;
  swap(_ptr, sp._ptr);
  swap(_control_block, sp._control_block);
}

template <class T> void SharedPtr<T>::reset() noexcept {
  SharedPtr{}.swap(*this);
}

template <class T> template <typename U> void SharedPtr<T>::reset(U *p) {
  SharedPtr{p}.swap(*this);
}

template <class T>
typename SharedPtr<T>::element_type *SharedPtr<T>::get() const noexcept {
  { return _ptr; }
}
template <class T>
typename SharedPtr<T>::element_type &SharedPtr<T>::operator*() const noexcept {
  return *_ptr;
}
template <class T>
typename SharedPtr<T>::element_type *SharedPtr<T>::operator->() const noexcept {
  return _ptr;
}
template <class T> long SharedPtr<T>::use_count() const noexcept {
  return (_control_block) ? _control_block->use_count() : 0;
}

template <class T>
constexpr WeakPtr<T>::WeakPtr() noexcept : _ptr{}, _control_block{} {}

template <class T>
template <class U>
WeakPtr<T>::WeakPtr(const SharedPtr<U> &sp) noexcept
    : _ptr{sp._ptr}, _control_block{sp._control_block} {
  if (_control_block)
    _control_block->inc_wref();
}

template <class T>
WeakPtr<T>::WeakPtr(WeakPtr const &wp) noexcept
    : _ptr{wp._ptr}, _control_block{wp._control_block} {
  if (_control_block)
    _control_block->inc_wref();
}
template <class T>
template <typename U>
WeakPtr<T>::WeakPtr(WeakPtr<U> &&sp) noexcept
    : _ptr{sp._ptr}, _control_block{sp._control_block} {
  sp._ptr = nullptr;
  sp._control_block = nullptr;
}

template <class T>
template <class U>
WeakPtr<T>::WeakPtr(const WeakPtr<U> &wp) noexcept
    : _ptr{wp._ptr}, _control_block{wp._control_block} {
  if (_control_block)
    _control_block->inc_wref();
}

template <class T> WeakPtr<T>::~WeakPtr() {
  if (_control_block)
    _control_block->dec_wref();
}
template <class T>
WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr &wp) noexcept {
  WeakPtr{wp}.swap(*this);
  return *this;
}

template <class T>
template <class U>
WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr<U> &wp) noexcept {
  WeakPtr{wp}.swap(*this);
  return *this;
}
template <class T>
template <typename U>
WeakPtr<T> &WeakPtr<T>::operator=(const SharedPtr<U> &sp) noexcept {
  WeakPtr{sp}.swap(*this);
  return *this;
}

template <class T> void WeakPtr<T>::swap(WeakPtr &wp) noexcept {
  using std::swap;
  swap(_ptr, wp._ptr);
  swap(_control_block, wp._control_block);
}
template <class T> long WeakPtr<T>::use_count() const noexcept {
  return (_control_block) ? _control_block->use_count() : 0;
}
template <class T> bool WeakPtr<T>::expired() const noexcept {
  return (_control_block) ? _control_block->expired() : false;
}
template <class T> SharedPtr<T> WeakPtr<T>::lock() const noexcept {
  if (expired())
    return SharedPtr<T>();
  else
    return SharedPtr<T>(*this);
}

} // namespace task