#include "list.h"

namespace task {

template <class T, class Alloc> list<T, Alloc>::list() {
  count = 0;

  Node *nodes = allocator.allocate(2);
  allocator.construct(nodes);
  allocator.construct(nodes + 1);
  first = nodes;
  last = nodes + 1;
  first->next = nodes;
  last->prev = first;
}
template <class T, class Alloc> list<T, Alloc>::~list() {}

template <class T, class Alloc> void list<T, Alloc>::push_back(T &&value) {
  insert(cend(), std::move(value));
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos,
                                                         const T &value) {

  auto *p = pos.current;
  count++;

  Node *node = allocator.allocate(1);
  allocator.construct(node, p->prev, p);

  p->prev->next = node;
  p->prev = node;
  return iterator(node);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(list::const_iterator pos, T &&value) {

  auto *p = pos.current;
  count++;

  Node *node = allocator.allocate(1);
  allocator.construct(node, value, p->prev, p);

  p->prev->next = node;
  p->prev = node;
  return iterator(node);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
  if (!empty()) {
    iterator itr{first->next};
    return itr;
  }
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
  return list::iterator();
}
template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
  const_iterator itr{last};
  return itr;
}
template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
  if (!empty()) {
    const_iterator const_itr{first->next};
    return const_itr;
  }
}

template <class T, class Alloc> size_t list<T, Alloc>::size() const {
  return count;
}
template <class T, class Alloc> void list<T, Alloc>::pop_front() {

  auto bei = cbegin();
  erase(cbegin());
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(list::const_iterator pos) {
  auto *p = pos.current;
  // typename iterator value{p->next};
  p->prev->next = p->next;
  p->next->prev = p->prev;
  // delete p;

  allocator.deallocate(p, 1);
  count--;
  return iterator(p->next);
}
template <class T, class Alloc> bool list<T, Alloc>::empty() const {
  return size() == 0;
}
template <class T, class Alloc> T &list<T, Alloc>::front() { return *begin(); }

template <class T, class Alloc> list<T, Alloc>::iterator::iterator() {}

template <class T, class Alloc>
list<T, Alloc>::iterator::iterator(const list<T, Alloc>::iterator &) {}
template <class T, class Alloc>
list<T, Alloc>::iterator::iterator(list::Node *p) : current{p} {}

template <class T, class Alloc>
typename list<T, Alloc>::iterator::reference
list<T, Alloc>::iterator::operator*() const {
  return current->data;
}

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator() : current{nullptr} {}

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(const list::const_iterator &) =
default;

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(list::Node *p) : current{p} {}
} // namespace task