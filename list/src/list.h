#pragma once
#include <iterator>

namespace task {

template <class T, class Alloc = std::allocator<T>> class list {
  struct Node {
    T data;
    Node *prev;
    Node *next;

    Node(const T &d = T{}, Node *p = nullptr, Node *n = nullptr)
        : data(d), prev(p), next(n) {}

    /* Node(const T &d = T{}, Node *p = nullptr, Node *n = nullptr)
     */
    /*: data(d)*/ /* */ /*, prev(p), next(n)*/ /* {
       prev = p;
       next = n;
       data = d;
     }*/
    Node(T &&d, Node *p = nullptr, Node *n = nullptr)
        : data(std::move(d)), prev(p), next(n) {}

    /*Node(const T &&d, Node *p = nullptr, Node *n = nullptr)
        : data(std::move(d)), prev(p), next(n) {}*/
  };

  using allocator_type_internal =
  typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
  allocator_type_internal allocator;

  int count;
  Node *first;
  Node *last;

public:
  class iterator {
  public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator();
    iterator(const iterator &);
    iterator &operator=(const iterator &);

    iterator &operator++();
    iterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    iterator &operator--();
    iterator operator--(int);

    bool operator==(iterator other) const;
    bool operator!=(iterator other) const;

    // Your code goes here?..

  protected:
    Node *current; // pointer to node in List
    T &retrieve() const;

    explicit iterator(Node *p); // protected constructor

    friend class list<T>;

  private:
    // Your code hoes here...
  };

  class const_iterator {
  public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator();
    const_iterator(const const_iterator &);
    // explicit const_iterator(const iterator &);
    const_iterator(const iterator &);
    const_iterator &operator=(const const_iterator &);

    const_iterator &operator++();
    const_iterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    const_iterator &operator--();
    const_iterator operator--(int);

    bool operator==(const_iterator other) const;
    bool operator!=(const_iterator other) const;

  protected:
    Node *current; // pointer to node in List
    T &retrieve() const;

    explicit const_iterator(Node *p); // protected constructor

    friend class list<T>;
  };

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  list();
  explicit list(const Alloc &alloc);
  list(size_t count, const T &value, const Alloc &alloc = Alloc());
  explicit list(size_t count, const Alloc &alloc = Alloc());

  ~list();

  list(const list &other);
  list(list &&other);
  list &operator=(const list &other);
  list &operator=(list &&other);

  Alloc get_allocator() const;

  T &front();
  const T &front() const;

  T &back();
  const T &back() const;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  reverse_iterator rbegin();
  reverse_iterator rend();

  const_reverse_iterator crbegin() const;
  const_reverse_iterator crend() const;

  bool empty() const;
  size_t size() const;
  size_t max_size() const;
  void clear();

  iterator insert(const_iterator pos, const T &value);
  iterator insert(const_iterator pos, T &&value);
  iterator insert(const_iterator pos, size_t count, const T &value);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void push_back(const T &value);
  void push_back(T &&value);
  void pop_back();

  void push_front(const T &value);
  void push_front(T &&value);
  void pop_front();

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args);

  template <class... Args> void emplace_back(Args &&... args);

  template <class... Args> void emplace_front(Args &&... args);

  void resize(size_t count);
  void swap(list &other);

  void merge(list &other);
  void splice(const_iterator pos, list &other);
  void remove(const T &value);
  void reverse();
  void unique();
  void sort();

  // Your code goes here?..

private:
  void init();
  // Your code goes here...
};

// Your template function definitions may go here...
//#include "list.cpp"
} // namespace task

#include "list.cpp"