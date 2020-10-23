#include "list.h"

namespace task {

template <class T, class Alloc>
void list<T, Alloc>::init() // init. private function
{
  count = 0;

  Node *nodes = allocator.allocate(2);
  allocator.construct(nodes);
  allocator.construct(nodes + 1);
  first = nodes;
  last = nodes + 1;
  first->next = last;
  last->prev = first;
}
template <class T, class Alloc> list<T, Alloc>::list() { init(); }

template <class T, class Alloc>
list<T, Alloc>::list(size_t count, const Alloc &alloc) {
  init();
  for (size_t i = 0; i < count; i++)
    push_back(T());
}

template <class T, class Alloc>
list<T, Alloc>::list(size_t count, const T &value, const Alloc &alloc) {
  init();
  for (size_t i = 0; i < count; i++)
    push_back(value);
}
template <class T, class Alloc> list<T, Alloc>::~list() {}

template <class T, class Alloc> void list<T, Alloc>::push_back(T &&value) {
  insert(cend(), std::move(value));
}

template <class T, class Alloc> void list<T, Alloc>::push_back(const T &value) {
  insert(cend(), value);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos,
                                                         const T &value) {

  auto *p = pos.current;
  count++;

  Node *node = allocator.allocate(1);
  allocator.construct(node, value, p->prev, p);

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

  /*auto iiiooo = p->prev;
  auto op = iiiooo->next;*/
  p->prev->next = node;
  p->prev = node;
  return iterator(node);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(list::const_iterator pos, size_t req_count,
                       const T &value) {

  auto *p = pos.current;

  count += req_count;

  //Node *nodes = allocator.allocate(req_count);

  for (size_t i = 0; i < req_count; i++) {
    Node *nodes = allocator.allocate(1);
    //allocator.construct(nodes + i, value, p->prev, p);
    allocator.construct(nodes, value, p->prev, p);

    //auto *poiy = nodes + i;
    //allocator.deallocate(poiy,1);
    /*  p->prev->next = nodes + i;
      p->prev = nodes + i;
      p = nodes + i; */
    p->prev->next = nodes;
    p->prev = nodes;
    p = nodes ;
  }

  // allocator.deallocate(nodes+1,1);

//  return iterator(nodes + req_count - 1);
  return iterator(p);
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
  iterator itr{last};
  return itr;
}
template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
  const_iterator itr{last};
  return itr;
}
template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
  /*if (!empty()) {
    const_iterator const_itr{first->next};
    return const_itr;
  }*/
  /*const_iterator itr{first->next};
  return itr; */
  const_iterator itr{first->next};
  return itr;
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

  // delete  p;
  allocator.deallocate(p, 1);
  count--;
  return iterator(p->next);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(list::const_iterator first, list::const_iterator last) {

  for (auto itr = first; itr != last;)
    itr = erase(itr);

  return iterator(last.current);
}

template <class T, class Alloc> bool list<T, Alloc>::empty() const {
  return size() == 0;
}
template <class T, class Alloc> T &list<T, Alloc>::front() { return *begin(); }

template <class T, class Alloc> const T &list<T, Alloc>::front() const {
  return *cbegin();
}

template <class T, class Alloc> void list<T, Alloc>::pop_back() {
  erase(--cend());
}
template <class T, class Alloc> void list<T, Alloc>::push_front(T &&value) {
  insert(cbegin(), std::move(value));
}
template <class T, class Alloc> T &list<T, Alloc>::back() { return *(--end()); }
template <class T, class Alloc> void list<T, Alloc>::clear() {
  while (!empty())
    pop_back();
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
template <class T, class Alloc> const T &list<T, Alloc>::back() const {
  return *(--cend());
  ;
}
template <class T, class Alloc> void list<T, Alloc>::swap(list &other) {
  Node *l_first_temp, *l_last_temp;
  l_first_temp = first;
  l_last_temp = last;
  first = other.first;
  last = other.last;
  other.first = l_first_temp;
  other.last = l_last_temp;
}
template <class T, class Alloc> void list<T, Alloc>::sort() {
  if (size() < 1)
    return;
  else {
    auto current = first;
    while (current->next != nullptr) {
      auto index = current->next;
      while (index != nullptr) {
        if (current->data > index->data) {
          auto temp = current->data;
          current->data = index->data;
          index->data = temp;
        }
        index = index->next;
      }
      current = current->next;
    }
  }
}

template <class T, class Alloc> list<T, Alloc>::iterator::iterator() {}

template <class T, class Alloc>
list<T, Alloc>::iterator::iterator(const list<T, Alloc>::iterator &) = default;

template <class T, class Alloc>
list<T, Alloc>::iterator::iterator(list::Node *p) : current{p} {}

template <class T, class Alloc>
typename list<T, Alloc>::iterator::reference
list<T, Alloc>::iterator::operator*() const {
  return current->data;
}
template <class T, class Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator--() {
  current = current->prev;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator++() {
  this->current = this->current->next;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator &
list<T, Alloc>::iterator::operator=(const list::iterator &it) {
  if (this == &it)
    return *this;
  current = it.current;

  return *this;
}
template <class T, class Alloc>
bool list<T, Alloc>::iterator::operator==(list::iterator other) const {
  // return current->data == other.current->data;
  return current == other.current;
}
template <class T, class Alloc>
bool list<T, Alloc>::iterator::operator!=(list::iterator other) const {
  return !(*this == other);
}

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator() : current{nullptr} {}

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(const list::const_iterator &) =
default;

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(list::Node *p) : current{p} {}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator--() {

  current = current->prev;
  return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator::reference
list<T, Alloc>::const_iterator::operator*() const {
  return current->data;
}
template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::const_iterator::operator++(int) {
  return list::const_iterator();
}
template <class T, class Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator++() {
  this->current = this->current->next;
  return *this;
}
template <class T, class Alloc>
typename list<T, Alloc>::const_iterator::pointer
list<T, Alloc>::const_iterator::operator->() const {
  return &(current->data);
}
template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(const list::iterator &other) {

  current = other.current;
}
template <class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator==(
    list::const_iterator other) const {
  return current == other.current;
}
template <class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator!=(
    list::const_iterator other) const {
  // return !(*this == other);
  return current != other.current;
}
template <class T, class Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator=(const list::const_iterator &itr) {
  if (this == &itr)
    return *this;
  current = itr.current;

  return *this;
}
/*template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(const list::iterator &) =
    default;*/
} // namespace task
// namespace task