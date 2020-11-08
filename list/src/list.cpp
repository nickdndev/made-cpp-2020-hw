#include "list.h"

namespace task {

template <class T, class Alloc> list<T, Alloc>::iterator::iterator() {
  _current = nullptr;
}

template <class T, class Alloc>
list<T, Alloc>::iterator::iterator(const iterator &other) {
  _current = other._current;
}
template <class T, class Alloc>
list<T, Alloc>::iterator::iterator(list::Node *p) : _current{p} {}

template <class T, class Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator=(
    const task::list<T, Alloc>::iterator &other) {
  _current = other._current;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator++() {
  _current = _current->_next;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator++(int) {
  iterator it = *this;
  _current = _current->_next;
  return it;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator::reference
    list<T, Alloc>::iterator::operator*() const {
  return _current->_value;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator::pointer
    list<T, Alloc>::iterator::operator->() const {
  return &(_current->_value);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator--() {
  _current = _current->_prev;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator--(int) {
  iterator it = *this;
  _current = _current->_prev;
  return it;
}

template <class T, class Alloc>
bool list<T, Alloc>::iterator::operator==(iterator other) const {
  return _current == other._current;
}

template <class T, class Alloc>
bool list<T, Alloc>::iterator::operator!=(iterator other) const {
  return _current != other._current;
}

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator() {
  _current = nullptr;
}

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(const const_iterator &other) {
  _current = other._current;
}

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(const iterator &other) {
  _current = other._current;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator=(
    const task::list<T, Alloc>::const_iterator &other) {
  _current = other._current;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator++() {
  _current = _current->_next;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::const_iterator::operator++(int) {
  const_iterator it = *this;
  _current = _current->_next;
  return it;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator::reference
    list<T, Alloc>::const_iterator::operator*() const {
  return _current->_value;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator::pointer
    list<T, Alloc>::const_iterator::operator->() const {
  return &(_current->_value);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator--() {
  _current = _current->_prev;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::const_iterator::operator--(int) {
  iterator it = *this;
  _current = _current->_prev;
  return it;
}

template <class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator==(const_iterator other) const {
  return _current == other._current;
}

template <class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator!=(const_iterator other) const {
  return _current != other._current;
}

template <class T, class Alloc> list<T, Alloc>::list() : _size(0) {
  _head = _allocator.allocate(1);
  _back = _allocator.allocate(1);
  _head->_prev = nullptr;
  _head->_next = _back;
  _back->_prev = _head;
  _back->_next = nullptr;
}

template <class T, class Alloc>
list<T, Alloc>::list(const Alloc &alloc) : list<T, Alloc>() {
  _allocator = alloc;
}

template <class T, class Alloc>
list<T, Alloc>::list(size_t count, const T &value, const Alloc &alloc)
    : list<T, Alloc>(alloc) {
  while (_size < count) {
    push_back(value);
  }
}

template <class T, class Alloc>
list<T, Alloc>::list(size_t count, const Alloc &alloc) : list<T, Alloc>(alloc) {
  while (_size < count) {
    Node *node = _allocator.allocate(1);
    _allocator.construct(node, _back, _back->_prev);
    _back->_prev = _back->_prev->_next = node;
    _size++;
  }
}

template <class T, class Alloc> list<T, Alloc>::~list() {
  clear();
  _allocator.deallocate(_head, 1);
  _allocator.deallocate(_back, 1);
}

template <class T, class Alloc>
list<T, Alloc>::list(const list &other) : list<T, Alloc>() {
  for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
    push_back(*it);
  }
}

template <class T, class Alloc>
list<T, Alloc>::list(list &&other) : list<T, Alloc>() {
  _size = other._size;
  other._size = 0;

  _head->_next = other._head->_next;
  _head->_next->_prev = _head;

  _back->_prev = other._back->_prev;
  _back->_prev->_next = _back;

  other._head->_next = other._back;
  other._back->_prev = other._head;
}

template <class T, class Alloc>
list<T, Alloc> &list<T, Alloc>::operator=(const list &other) {
  clear();

  for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
    push_back(*it);
  }

  return *this;
}

template <class T, class Alloc>
list<T, Alloc> &list<T, Alloc>::operator=(list &&other) {
  clear();

  _head->_next = other._head->_next;
  _head->_next->_prev = _head;

  _back->_prev = other._back->_prev;
  _back->_prev->_next = _back;

  _size = other._size;

  other._head->_next = other._back;
  other._back->_prev = other._head;

  other._size = 0;
  return *this;
}

template <class T, class Alloc> Alloc list<T, Alloc>::get_allocator() const {
  return _allocator;
}

template <class T, class Alloc> T &list<T, Alloc>::front() {
  return _head->_next->_value;
}

template <class T, class Alloc> const T &list<T, Alloc>::front() const {
  return _head->_next->_value;
}

template <class T, class Alloc> T &list<T, Alloc>::back() {
  return _back->_prev->_value;
}

template <class T, class Alloc> const T &list<T, Alloc>::back() const {
  return _back->_prev->_value;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
  return {_head->_next};
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
  return {_back};
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
  return {_head->_next};
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
  return {_back};
}

template <class T, class Alloc>
typename task::list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
  return {_back->_prev};
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
  return {_head};
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator
list<T, Alloc>::crbegin() const {
  const_reverse_iterator it(cend());
  return it;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const {
  const_reverse_iterator it(cbegin());
  return it;
}

template <class T, class Alloc> bool list<T, Alloc>::empty() const {
  return size() == 0;
}
template <class T, class Alloc> size_t list<T, Alloc>::size() const {
  return _size;
}
template <class T, class Alloc> size_t list<T, Alloc>::max_size() const {
  return _allocator.max_size();
}

template <class T, class Alloc> void list<T, Alloc>::clear() {
  while (!empty())
    pop_front();
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, size_t count, const T &value) {

  iterator iter;
  for (auto i = 0; i < count; i++) {
    iter = insert(pos, value);
  }
  return iter;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator pos) {
  auto *p = pos._current;
  p->_prev->_next = p->_next;
  p->_next->_prev = p->_prev;
  _allocator.destroy(p);
  _allocator.deallocate(const_cast<Node *>(p), 1);
  _size--;
  return iterator(p->_next);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator first,
                                                        const_iterator last) {
  for (auto itr = first; itr != last;)
    itr = erase(itr);

  return {const_cast<Node *>(last._current)};
}

template <class T, class Alloc> void list<T, Alloc>::push_back(const T &value) {
  insert(cend(), value);
}

template <class T, class Alloc> void list<T, Alloc>::push_back(T &&value) {
  insert(cend(), std::move(value));
}

template <class T, class Alloc> void list<T, Alloc>::pop_back() {
  erase(--cend());
}

template <class T, class Alloc>
void list<T, Alloc>::push_front(const T &value) {
  insert(cbegin(), value);
}

template <class T, class Alloc> void list<T, Alloc>::push_front(T &&value) {
  insert(cbegin(), std::move(value));
}

template <class T, class Alloc> void list<T, Alloc>::pop_front() {
  auto bei = cbegin();
  erase(cbegin());
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos,
                                                         T &&value) {
  _size++;
  Node *prev = pos._current->_prev;
  Node *node = _allocator.allocate(1);

  _allocator.construct(node, std::move(value), prev->_next, prev);

  prev->_next->_prev = node;
  prev->_next = node;

  iterator iter;
  iter._current = node;

  return iter;
}

template <class T, class Alloc>
template <class... Args>
typename list<T, Alloc>::iterator
list<T, Alloc>::emplace(typename list<T, Alloc>::const_iterator pos,
                        Args &&... args) {
  Node *prev = pos._current->_prev;

  Alloc data_allocator;
  Node *node = _allocator.allocate(1);
  T *data = data_allocator.allocate(1);
  data_allocator.construct(data, std::forward<Args>(args)...);
  _allocator.construct(node, std::move(*data), prev->_next, prev);
  data_allocator.destroy(data);
  data_allocator.deallocate(data, 1);

  prev->_next->_prev = node;
  prev->_next = node;

  iterator it;
  it._current = pos._current->_prev;

  _size++;
  return it;
}

template <class T, class Alloc>
template <class... Args>
void list<T, Alloc>::emplace_back(Args &&... args) {
  emplace(end(), std::forward<Args>(args)...);
}

template <class T, class Alloc>
template <class... Args>
void list<T, Alloc>::emplace_front(Args &&... args) {
  emplace(begin(), std::forward<Args>(args)...);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos,
                                                         const T &value) {
  Node *prev = pos._current->_prev;
  Node *node = _allocator.allocate(1);
  _allocator.construct(node, value, prev->_next, prev);
  prev->_next->_prev = node;
  prev->_next = node;

  iterator it;
  it._current = node;

  _size++;
  return it;
}

template <class T, class Alloc> void list<T, Alloc>::resize(size_t count) {
  if (size() > count) {
    while (size() > count)
      pop_back();
  } else if (size() < count) {
    while (size() < count)
      push_back(T());
  }
}

template <class T, class Alloc> void list<T, Alloc>::swap(list &other) {
  Node *temp = _head;
  _head = other._head;

  other._head = temp;
  temp = other._back;
  other._back = _back;

  _back = temp;
  size_t s = _size;

  _size = other._size;
  other._size = s;
}

template <class T, class Alloc> void list<T, Alloc>::merge(list &other) {
  if (!other.empty()) {
    _size += other._size;
    other._size = 0;

    Node *node = other._head->_next;
    Node *temp = _head->_next;

    while (node != other._back) {
      while (temp != _back && node->_value > temp->_value)
        temp = temp->_next;

      Node *next = node->_next;
      node->_prev = temp->_prev;
      node->_next = temp;

      temp->_prev->_next = node;
      temp->_prev = node;

      node = next;
    }
    other._head->_next = other._back;
    other._back->_prev = other._head;
  }
}

template <class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list &other) {
  if (!other.empty()) {
    _size += other._size;
    other._size = 0;

    Node *node = const_cast<Node *>(pos._current);
    node->_prev->_next = other._head->_next;
    node->_prev = other._back->_prev;

    other._head->_next->_prev = node->_prev;
    other._back->_prev->_next = node;
    other._head->_next = other._back;
    other._back->_prev = other._head;
  }
}

template <class T, class Alloc>
void task::list<T, Alloc>::remove(const T &value) {
  iterator iter = begin();
  list<iterator> removed;

  while (iter != end()) {
    if (*iter == value) {
      removed.push_back(iter);
    }
    iter++;
  }

  for (auto it = removed.begin(); it != removed.end(); ++it) {
    erase(*it);
  }
}

template <class T, class Alloc> void task::list<T, Alloc>::reverse() {
  if (!empty()) {
    auto current_ptr = _head;
    while (current_ptr != nullptr) {
      std::swap(current_ptr->_next, current_ptr->_prev);
      current_ptr = current_ptr->_prev;
    }
    std::swap(_head, _back);
  }
}

template <class T, class Alloc> void task::list<T, Alloc>::unique() {
  iterator iter = begin();
  while (iter != --end()) {
    if (*iter == *(++iter)) {
      iter = erase(iter);
      iter--;
    }
  }
}

template <class T, class Alloc> void task::list<T, Alloc>::sort() {
  if (size() < 1)
    return;
  else {
    auto current = _head->_next;
    while (current->_next != nullptr) {
      auto index = current->_next;
      while (index != nullptr && index->_next != nullptr) {
        if (current->_value > index->_value) {
          auto temp = current->_value;
          current->_value = index->_value;
          index->_value = temp;
        }
        index = index->_next;
      }
      current = current->_next;
    }
  }
}

} // namespace task