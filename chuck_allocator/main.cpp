#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>

template <std::size_t CHUNK_SIZE> class Chunk {

  static const std::size_t HEADER_LENGTH = 4;

  std::uint32_t *point_end_block;
  std::set<std::uint32_t *> free_blocks;
  std::vector<std::uint8_t *> blocks;

public:
  Chunk() {
    blocks.resize(CHUNK_SIZE);

    auto *pointer_header = reinterpret_cast<std::uint32_t *>(blocks.data());

    *pointer_header = CHUNK_SIZE - HEADER_LENGTH;
    point_end_block = pointer_header;
    free_blocks.insert(pointer_header);
  }

  ~Chunk() = default;

  bool contains(const std::uint8_t *address) const noexcept {
    const auto *start_chunk =
        reinterpret_cast<const std::uint8_t *>(blocks.data());
    const auto *end_chunk = start_chunk + CHUNK_SIZE;
    return (start_chunk <= address) && (address <= end_chunk);
  }

  std::uint8_t *reserveBlock(std::size_t allocation_size) {
    const auto alloc_size_req = static_cast<std::uint32_t>(allocation_size);

    if ((!point_end_block) || (alloc_size_req > *point_end_block)) {
      return nullptr;
    }

    auto comp_min_block = [alloc_size_req](const std::uint32_t *lhs,
                                           const std::uint32_t *rhs) {
      if (*rhs < alloc_size_req)
        return true;

      return (*lhs < *rhs) && (*lhs >= alloc_size_req);
    };

    const auto min_item = std::min_element(free_blocks.cbegin(),
                                           free_blocks.cend(), comp_min_block);

    std::uint32_t *header_address = *min_item;
    auto *new_header_address = reinterpret_cast<std::uint32_t *>(
        reinterpret_cast<std::uint8_t *>(header_address) + HEADER_LENGTH +
            alloc_size_req);

    if (free_blocks.find(new_header_address) == free_blocks.cend()) {
      const std::uint32_t old_block_size = *header_address;
      const std::uint32_t difference = old_block_size - HEADER_LENGTH;

      if (difference >= alloc_size_req) {
        const std::uint32_t new_block_size = difference - alloc_size_req;
        *new_header_address = new_block_size;
        free_blocks.insert(new_header_address);
      }
    }

    free_blocks.erase(header_address);

    *header_address = static_cast<std::uint32_t>(allocation_size);

    if (header_address == point_end_block) {

      auto comp_max_block = [](const std::uint32_t *lhs,
                               const std::uint32_t *rhs) {
        return (*lhs) < (*rhs);
      };

      const auto max_it = std::max_element(free_blocks.cbegin(),
                                           free_blocks.cend(), comp_max_block);

      point_end_block = (max_it != free_blocks.cend()) ? (*max_it) : (nullptr);
    }

    return reinterpret_cast<std::uint8_t *>(header_address) + HEADER_LENGTH;
  }

  void releaseBlock(std::uint8_t *block_ptr) {
    std::uint8_t *header_address = block_ptr - HEADER_LENGTH;
    const std::uint32_t size_relized_block = *header_address;

    if ((!point_end_block) || (size_relized_block > *point_end_block)) {
      point_end_block = reinterpret_cast<std::uint32_t *>(header_address);
    }

    free_blocks.insert(reinterpret_cast<std::uint32_t *>(header_address));
  }
};

template <std::size_t CHUNK_SIZE> class AllocationMemory {
  std::list<Chunk<CHUNK_SIZE> *> chunks;

public:
  AllocationMemory() = default;

  ~AllocationMemory() {
    for (auto &chunk : chunks) {
      delete chunk;
    }
  }

  void *allocate_object(std::size_t size) {

    if (size == 0) {
      return nullptr;
    }

    for (auto &chunk : chunks) {
      void *allocated_block = chunk->reserveBlock(size);
      if (allocated_block) {
        return allocated_block;
      }
    }

    auto chunk = new Chunk<CHUNK_SIZE>();

    chunks.push_back(chunk);
    std::uint8_t *allocated_block = chunk->reserveBlock(size);
    return allocated_block;
  }

  void deallocate_object(void *p, std::size_t size) {
    if ((!p) || (size == 0)) {
      return;
    }

    auto *deallocation_ptr = static_cast<std::uint8_t *>(p);
    for (auto &chunk : chunks) {
      if (chunk->contains(deallocation_ptr)) {
        chunk->releaseBlock(deallocation_ptr);
      }
    }
  }
};

template <typename T, std::size_t CHUNK_SIZE = 1024> class Allocator {
  AllocationMemory<CHUNK_SIZE> *memory;
  size_t *number_instances;

public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  template <typename U, std::size_t> friend class Allocator;

  template <typename U> struct rebind { using other = Allocator<U>; };

public:
  Allocator() {
    memory = new AllocationMemory<CHUNK_SIZE>();
    number_instances = new size_t(1);

    std::cout << std::endl
              << "---Crete allocator and allocate memory---"
              << (*number_instances) << std::endl;
  };

  Allocator(const Allocator &other) noexcept {
    memory = other.memory;
    number_instances = other.number_instances;
    (*number_instances)++;
  }

  ~Allocator() {
    if ((*number_instances) > 1) {

      (*number_instances)--;

      std::cout << std::endl
                << "---Call destructor for copped allocator---" << std::endl
                << " Number remains instances : " << (*number_instances)
                << std::endl;
    } else {
      delete memory;

      std::cout << std::endl
                << "---Destroy allocator and release memory---" << std::endl;
    }
  }

  template <typename U> Allocator(const Allocator<U> &other) noexcept {
    memory = other.memory;
    number_instances = other.number_instances;
    (*number_instances)++;
  }

  T *allocate(std::size_t n) {
    return static_cast<T *>(memory->allocate_object(n * sizeof(T)));
  }

  void deallocate(T *p, std::size_t n) {
    memory->deallocate_object(p, n * sizeof(T));
  }

  template <typename U, typename... Args>
  void construct(U *ptr, Args &&... args) {
    new (reinterpret_cast<void *>(ptr)) U{std::forward<Args>(args)...};
  }

  template <typename U> void destroy(U *ptr) { ptr->~U(); }

  Allocator &operator=(const Allocator &a) {
    if (this == &a)
      return *this;

    if ((*number_instances) > 1) {
      delete memory;
    } else {
      (*number_instances)--;
    }

    memory = a.memory;
    number_instances = a.number_instances;
    (*number_instances)++;

    return *this;
  }
};

int main(int argc, char **argv) {

  {
    /* Test copy allocator*/
    Allocator<int> custom_int_allocator;

    Allocator<float> custom_float_allocator{custom_int_allocator};

    std::vector<int, Allocator<int>> vector{custom_int_allocator};

    /* Test allocator for vector stl container*/
    for (int i = 0; i < 50; ++i) {
      vector.push_back(i);
      std::cout << vector.at(i) << " ";
    }

    std::cout << std::endl;

    vector.resize(16);
    for (int val : vector) {
      std::cout << val << " ";
    }

    /* Test allocator for list<float> stl container*/

    std::list<float, Allocator<float>> list{
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}, custom_float_allocator};
    for (float val : list) {
      std::cout << val << " ";
    }

    std::cout << std::endl;

    /* Test allocator for map<pair> stl container*/
    Allocator<std::pair<double, double>> allocator_for_pair;

    std::map<double, double, std::less<>,
        Allocator<std::pair<const double, double>>>
        map{{{1.0, 100.0}, {2.0, 200.0}}, allocator_for_pair};

    for (const auto &it : map) {
      std::cout << "{" << it.first << " : " << it.second << "} ";
    }

    /* Test allocator for set<double> stl container*/

    Allocator<double> allocator_for_double;

    std::set<double, std::less<>, Allocator<double>> set{
        {1000.0, 2000.0, 3000.0}, allocator_for_double};

    for (double val : set) {
      std::cout << val << " ";
    }

    {

      /* Test allocator for string stl container*/

      Allocator<std::string> allocator_for_string;
      std::basic_string<char, std::char_traits<char>, Allocator<char>> string1{
          "from 1 string\n", allocator_for_string};

      std::basic_string<char, std::char_traits<char>, Allocator<char>> string2{
          "from 2 string\n", allocator_for_string};

      std::basic_string<char, std::char_traits<char>, Allocator<char>> string3{
          "from 2 string\n", allocator_for_string};

      std::basic_string<char, std::char_traits<char>, Allocator<char>>
          result_string = string1 + string2 + string3;

      std::cout << result_string;
    }
  }

  return 0;
}