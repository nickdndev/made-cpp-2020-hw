#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace details {

template <std::size_t CHUNK_SIZE> class Chunk {
  static const std::size_t HEADER_LENGTH = 4u;

public:
  Chunk() {
    blocks.resize(CHUNK_SIZE);
    std::uint32_t *pointer_header =
        reinterpret_cast<std::uint32_t *>(blocks.data());
    *pointer_header = CHUNK_SIZE - HEADER_LENGTH;
    point_end_block = pointer_header;
    free_blocks.insert(pointer_header);
  }

  bool contains(const std::uint8_t *address) const noexcept {
    const std::uint8_t *start_chunk_address =
        reinterpret_cast<const std::uint8_t *>(blocks.data());
    const std::uint8_t *end_chunk_address = start_chunk_address + CHUNK_SIZE;
    return (start_chunk_address <= address) && (address <= end_chunk_address);
  }

  std::uint8_t *reserve_block(std::size_t allocation_size) {
    const std::size_t not_aligned_address =
        reinterpret_cast<std::size_t>(point_end_block) + allocation_size;
    /*  const std::size_t alignment_padding =
          getAlignmentPadding(not_aligned_address, HEADER_LENGTH);*/
    const std::uint32_t alloc_size_with_alignment =
        static_cast<std::uint32_t>(allocation_size /* + alignment_padding*/);
    if ((!point_end_block) || (alloc_size_with_alignment > *point_end_block)) {
      return nullptr;
    }

    const auto min_it = std::min_element(
        free_blocks.cbegin(), free_blocks.cend(),
        [alloc_size_with_alignment](const std::uint32_t *lhs,
                                    const std::uint32_t *rhs) {
          if (*rhs < alloc_size_with_alignment) {
            return true;
          }

          return (*lhs < *rhs) && (*lhs >= alloc_size_with_alignment);
        });

    std::uint32_t *header_address = *min_it;
    std::uint32_t *new_header_address = reinterpret_cast<std::uint32_t *>(
        reinterpret_cast<std::uint8_t *>(header_address) + HEADER_LENGTH +
            alloc_size_with_alignment);

    if (free_blocks.find(new_header_address) == free_blocks.cend()) {
      const std::uint32_t old_block_size = *header_address;
      const std::uint32_t difference = old_block_size - HEADER_LENGTH;

      if (difference >= alloc_size_with_alignment) {
        const std::uint32_t new_block_size =
            difference - alloc_size_with_alignment;
        *new_header_address = new_block_size;
        free_blocks.insert(new_header_address);
      }
    }

    free_blocks.erase(header_address);

    *header_address = static_cast<std::uint32_t>(allocation_size);

    if (header_address == point_end_block) {

      const auto max_it = std::max_element(
          free_blocks.cbegin(), free_blocks.cend(),
          [](const std::uint32_t *lhs, const std::uint32_t *rhs) {
            return (*lhs) < (*rhs);
          });

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
    auto forward_it =
        free_blocks.find(reinterpret_cast<std::uint32_t *>(header_address));
  }

public:
  std::uint32_t *point_end_block;
  std::set<std::uint32_t *> free_blocks;
  std::vector<std::uint8_t *> blocks;
};

} // namespace details

template <typename T> class Allocator {
  std::deque<details::Chunk<16'384u>> chunks{1u};

public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  template <typename U> friend class Allocator;

  template <typename U> struct rebind { using other = Allocator<U>; };

  void *allocate_object(std::size_t size) {

    if (size == 0u) {
      return nullptr;
    }

    for (auto &chunk : chunks) {
      void *allocated_block = chunk.reserve_block(size);
      if (allocated_block) {
        return allocated_block;
      }
    }

    chunks.push_back(details::Chunk<16'384u>{});
    auto &chunk = chunks.back();
    std::uint8_t *allocated_block = chunk.reserve_block(size);
    return allocated_block;
  }

  void deallocate_object(void *p, std::size_t size) {
    if ((!p) || (size == 0u)) {
      return;
    }

    auto *deallocation_ptr = static_cast<std::uint8_t *>(p);
    for (auto &chunk : chunks) {
      if (chunk.contains(deallocation_ptr)) {
        chunk.releaseBlock(deallocation_ptr);
      }
    }
  }

public:
  Allocator() = default;

  Allocator(const Allocator &other) noexcept {}

  template <typename U> Allocator(const Allocator<U> &other) noexcept {}

  T *allocate(std::size_t n) {
    return static_cast<T *>(allocate_object(n * sizeof(T)));
  }

  void deallocate(T *p, std::size_t n) { deallocate_object(p, n * sizeof(T)); }

  template <typename U, typename... Args>
  void construct(U *ptr, Args &&... args) {
    new (reinterpret_cast<void *>(ptr)) U{std::forward<Args>(args)...};
  }

  template <typename U> void destroy(U *ptr) { ptr->~U(); }
};

template <typename T, typename U>
bool operator==(const Allocator<T> &lhs, const Allocator<U> &rhs) {
  return lhs.alloc_core == rhs.alloc_core;
}

template <typename T, typename U>
bool operator!=(const Allocator<T> &lhs, const Allocator<U> &rhs) {
  return !(lhs == rhs);
}

template <typename T, std::size_t CHUNK_SIZE = 16'384u>
using CustomAllocator = Allocator<T>;

template <typename T> using CustomAllocatorWithStackChunks = Allocator<T>;

template <typename T> using CustomAllocatorWithHeapChunks = Allocator<T>;

template <typename T> using custom_vector = std::vector<T, CustomAllocator<T>>;

template <typename T> using custom_list = std::list<T, CustomAllocator<T>>;

template <typename T>
using custom_set = std::set<T, std::less<T>, CustomAllocator<T>>;

template <typename T>
using custom_unordered_set =
std::unordered_set<T, std::hash<T>, std::equal_to<T>, CustomAllocator<T>>;

template <typename K, typename V>
using custom_map =
std::map<K, V, std::less<K>, CustomAllocator<std::pair<const K, V>>>;

template <typename K, typename V>
using custom_unordered_map =
std::unordered_map<K, std::hash<K>, std::equal_to<K>,
CustomAllocator<std::pair<const K, V>>>;

using custom_string =
std::basic_string<char, std::char_traits<char>, CustomAllocator<char>>;

int main(int argc, char **argv) {

  CustomAllocator<int> custom_int_allocator;
  custom_vector<int> vector{custom_int_allocator};
  for (int i = 0u; i < 100; ++i) {
    vector.push_back(i);
    std::cout << vector.at(i) << " ";
  }

  vector.resize(16u);
  for (int val : vector) {
    std::cout << val << " ";
  }

  CustomAllocator<float> custom_float_allocator{custom_int_allocator};
  custom_list<float> list{{10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f},
                          custom_float_allocator};
  for (float val : list) {
    std::cout << val << " ";
  }

  CustomAllocator<std::pair<double, double>> custom_pair_allocator;
  custom_map<double, double> map{{{1.0, 100.0}, {2.0, 200.0}},
                                 custom_pair_allocator};
  for (const auto &it : map) {
    std::cout << "{" << it.first << " : " << it.second << "} ";
  }

  CustomAllocator<double> custom_double_allocator;
  custom_set<double> set{{1000.0, 2000.0, 3000.0}, custom_double_allocator};
  for (double val : set) {
    std::cout << val << " ";
  }

  CustomAllocator<char> custom_char_allocator;
  custom_string string1{"from 1 string\n", custom_char_allocator};
  custom_string string2{"from 2 string\n", custom_char_allocator};
  custom_string string3{"from 2 string\n", custom_char_allocator};
  custom_string result_string = string1 + string2 + string3;
  std::cout << result_string;

  return 0;
}