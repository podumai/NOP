#ifndef NOP_CONTAINER_DYNAMIC_BITSET_IMPL_HPP /* Begin nop::container::dynamic_bitset header file */
#define NOP_CONTAINER_DYNAMIC_BITSET_IMPL_HPP 1UL

#pragma once

/*
 * TODO: Improve iterators to provide stl_algo support.
 */

#ifndef __cplusplus
  #error CXX compiler required
#endif

#include <iterator> /* iterator_traits, iterator concepts */
#include <memory> /* std::allocator<T> */
#include <bitset> /* std::bitset<N> */
#include <stdexcept> /* std::out_of_range, std::length_error, std::invalid_argument */
#include <concepts> /* std::unsigned_integral */
#include <cstdint> /* std::size_t, std::ptrdiff_t */

#include <cstdlib> /* memcpy */

#include "dynamic_bitset_extension.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace container /* Begin namespace container */
{

template<typename Alloc>
concept is_valid_dynamic_bitset_alloc_t = sizeof(typename std::allocator_traits<Alloc>::value_type) == 8UL
                                       && std::unsigned_integral<typename std::allocator_traits<Alloc>::value_type>;

} /* End namespace container */

} /* End namespace __nop_details */

namespace nop /* Begin namespace nop */
{

//template<typename T, typename Alloc = std::allocator<T>>
//class dynamic_bitset;

namespace details /* Begin namespace details */
{

namespace dynamic_bitset_limits /* Begin namespace dynamic_bitset_limits */
{

#if __WORDSIZE == 64UL

  #ifndef __NOP_DISABLE_WARNINGS__
    #error Library "NOP" : dynamic_bitset class requires explicit allocator template parameter.
    #error Library "NOP" : To disable this message provide : __NOP_DISABLE_WARNINGS__
  #endif

  #if __cplusplus >= 201103L
    extern constexpr std::size_t MAX_SIZE{0x2000000000UL};
    extern constexpr std::size_t MAX_CAPACITY{2147483648UL};
    extern constexpr std::size_t MID_CAPACITY{1073741824UL};
  #else
    #define MAX_SIZE (0x2000000000UL)
    #define MAX_CAPACITY (2147483648UL)
    #define MID_CAPACITY (1073741824UL)
  #endif
#else
  #error Library "NOP" : dynamic_bitset class does not support 32 BIT mode (or less)
#endif

} /* End namespace dynamic_bitset_limits */

} /* End namespace details */

namespace container /* Begin namespace container */
{

template<__nop_details::container::is_valid_dynamic_bitset_alloc_t Alloc = std::allocator<std::size_t>>
class dynamic_bitset
{
 public:
  using value_type = bool;
  using allocator_type = Alloc;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = bool&;
  using const_reference = const bool&;
  using pointer = typename std::allocator_traits<Alloc>::pointer;
  using const_pointer = const typename std::allocator_traits<Alloc>::pointer;
  using bit_state = bool;
  using storage_state = bool;
  using bit_string = std::string;

 private:
  enum bit_mask : size_type
  {
    BIT = 1UL,
    RESET = 0UL,
    SET = 0xffffffffffffffffUL
  };

  public:
  class iterator
  {
   private:
    friend dynamic_bitset;

   public:
    using difference_type = dynamic_bitset::difference_type;
    using value_type = dynamic_bitset::value_type;
    using pointer = dynamic_bitset::pointer;
    using reference = dynamic_bitset::reference;
    using iterator_category = std::contiguous_iterator_tag;

   private:
    class bit_reference
    {
     private:
      friend iterator;

      private:
      pointer m_byte;
      difference_type m_bit;

     public:
      constexpr bit_reference(pointer ptr, difference_type bit_position) noexcept
          : m_byte{ptr},
            m_bit{bit_position}
      {
        /* Empty */
      }

      constexpr bit_reference(const bit_reference&) noexcept = default;
      constexpr bit_reference(bit_reference&&) noexcept = default;
      constexpr ~bit_reference() = default;

      constexpr void swap(bit_reference& other) noexcept
      {
        std::swap(m_byte, other.m_byte);
        std::swap(m_bit, other.m_bit);
      }

      constexpr bit_reference& operator=(bit_state value) noexcept
      {
        if (value)
        {
          m_byte[byte_division(static_cast<dynamic_bitset::size_type>(m_bit), UL)] |=
          dynamic_bitset::bit_mask::BIT << byte_module(static_cast<dynamic_bitset::size_type>(m_bit), UL);
        }
        else
        {
          m_byte[byte_division(static_cast<dynamic_bitset::size_type>(m_bit), UL)] &=
          ~(dynamic_bitset::bit_mask::BIT << byte_module(static_cast<dynamic_bitset::size_type>(m_bit), UL));
        }

        return *this;
      }

      constexpr bit_reference& operator=(const bit_reference& other) noexcept
      {
        if (this != std::addressof(other))
        {
          if (other.get_bit())
          {
            m_byte[byte_division(static_cast<dynamic_bitset::size_type>(m_bit), UL)] |=
            dynamic_bitset::bit_mask::BIT << byte_module(static_cast<dynamic_bitset::size_type>(m_bit), UL);
          }
          else
          {
            m_byte[byte_division(static_cast<dynamic_bitset::size_type>(m_bit), UL)] &=
            ~(dynamic_bitset::bit_mask::BIT << byte_module(static_cast<dynamic_bitset::size_type>(m_bit), UL));
          }
        }

        return *this;
      }

      [[nodiscard]] constexpr bit_state get_bit() const noexcept
      {
        return m_byte[byte_division(static_cast<dynamic_bitset::size_type>(m_bit), UL)] &
        dynamic_bitset::bit_mask::BIT << byte_module(static_cast<dynamic_bitset::size_type>(m_bit), UL);
      }

      constexpr bit_reference& operator=(bit_reference&&) noexcept = default;

      [[nodiscard]] constexpr bool operator==(const bit_reference& other) const noexcept
      {
        if (!m_byte || !other.m_byte)
        {
          return false;
        }
        else
        {
          return get_bit() == other.get_bit();
        }
      }

      [[nodiscard]] constexpr bool operator!=(const bit_reference& other) const noexcept
      {
        return !(*this == other);
      }

      constexpr operator bool() const noexcept
      {
        return m_byte != nullptr ? get_bit() : false;
      }

      explicit constexpr operator char() const noexcept
      {
        return m_byte != nullptr ? get_bit() + '0' : '0';
      }

    };

   private:
    bit_reference m_iter;

   public:
    constexpr iterator(pointer ptr, difference_type bitPosition) noexcept
      : m_iter{ptr, bitPosition}
    {
      /* Empty */
    }

    constexpr iterator(const iterator&) noexcept = default;
    constexpr iterator(iterator&&) noexcept = default;
    constexpr ~iterator() = default;

    constexpr void swap(iterator& other) noexcept
    {
      m_iter.swap(other.m_iter);
    }

    constexpr iterator& operator++() noexcept
    {
      ++m_iter.m_bit;
      return *this;
    }

    constexpr iterator operator++(std::int32_t) noexcept
    {
      return {m_iter.m_byte, m_iter.m_bit++};
    }

    constexpr iterator& operator+=(difference_type index) noexcept
    {
      m_iter.m_bit += index;
      return *this;
    }

    [[nodiscard]] constexpr iterator operator+(difference_type index) const noexcept
    {
      return {m_iter.m_byte, m_iter.m_bit + index};
    }

    [[nodiscard]] friend constexpr iterator operator+(difference_type index, const iterator& iter) noexcept
    {
      return {iter.m_iter.m_byte, iter.m_iter.m_bit + index};
    }

    constexpr iterator& operator--() noexcept
    {
      --m_iter;
      return *this;
    }

    constexpr iterator operator--(std::int32_t) noexcept
    {
      return {m_iter.m_byte, m_iter.m_bit--};
    }

    constexpr iterator& operator-=(difference_type index) noexcept
    {
      m_iter.m_bit -= index;
      return *this;
    }

    [[nodiscard]] constexpr iterator operator-(difference_type index) const noexcept
    {
      return {m_iter.m_byte, m_iter.m_bit - index};
    }

    [[nodiscard]] constexpr difference_type operator-(const iterator& other) const noexcept
    {
      return m_iter.m_bit - other.m_iter.m_bit;
    }

    [[nodiscard]] friend constexpr  bool operator<(const iterator& lhs, const iterator& rhs) noexcept
    {
      return lhs.m_iter.m_bit < rhs.m_iter.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator<=(const iterator& lhs, const iterator& rhs) noexcept
    {
      return lhs.m_iter.m_bit <= rhs.m_iter.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator>(const iterator& lhs, const iterator& rhs) noexcept
    {
      return lhs.m_iter.m_bit > rhs.m_iter.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator>=(const iterator& lhs, const iterator& rhs) noexcept
    {
      return lhs.m_iter.m_bit >= rhs.m_iter.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator==(const iterator& lhs, const iterator& rhs) noexcept
    {
      return lhs.m_iter.m_bit == rhs.m_iter.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator!=(const iterator& lhs, const iterator& rhs) noexcept
    {
      return lhs.m_iter.m_bit != rhs.m_iter.m_bit;
    }

    [[nodiscard]] constexpr bit_reference& operator*() noexcept
    {
      return m_iter;
    }

    [[nodiscard]] constexpr bit_reference operator[](difference_type index) noexcept
    {
      return {m_iter.m_byte, m_iter.m_bit + index};
    }

    constexpr iterator& operator=(const iterator&) noexcept = default;
    constexpr iterator& operator=(iterator&&) noexcept = default;

  };

  class const_iterator
  {
   public:
    using difference_type = typename dynamic_bitset::difference_type;
    using value_type = typename dynamic_bitset::value_type;
    using pointer = typename dynamic_bitset::pointer;
    using reference = typename dynamic_bitset::value_type&;
    using iterator_category = typename std::contiguous_iterator_tag;

   private:
    pointer m_byte;
    difference_type m_bit;

   public:
    constexpr const_iterator(pointer ptr, difference_type bit_position) noexcept
        : m_byte{ptr},
          m_bit{bit_position}
    {
      /* Empty */
    }

    constexpr const_iterator(const const_iterator&) noexcept = default;
    constexpr const_iterator(const_iterator&&) noexcept = default;
    constexpr ~const_iterator() = default;

    constexpr const_iterator& operator++() noexcept
    {
      ++m_bit;
      return *this;
    }

    [[nodiscard]] constexpr const_iterator operator++(std::int32_t) noexcept
    {
      return {m_byte, m_bit++};
    }

    constexpr const_iterator& operator+=(difference_type index) noexcept
    {
      m_bit += index;
      return *this;
    }

    [[nodiscard]] constexpr const_iterator operator+(difference_type index) noexcept
    {
      return {m_byte, m_bit + index};
    }

    [[nodiscard]] friend constexpr const_iterator operator+(difference_type index,
                                                            const const_iterator& iter) noexcept
    {
      return {iter.m_byte, iter.m_bit + index};
    }

    constexpr const_iterator& operator--() noexcept
    {
      --m_bit;
      return *this;
    }

    [[nodiscard]] constexpr const_iterator operator--(std::int32_t) noexcept
    {
      return {m_byte, m_bit--};
    }

    constexpr const_iterator& operator-=(difference_type index) noexcept
    {
      m_bit -= index;
      return *this;
    }

    [[nodiscard]] constexpr const_iterator operator-(difference_type index) noexcept
    {
      return {m_byte, m_bit - index};
    }

    [[nodiscard]] friend constexpr const_iterator operator-(difference_type index,
                                                            const const_iterator& other) noexcept
    {
      return {other.m_byte, other.m_bit - index};
    }

    [[nodiscard]] constexpr difference_type operator-(const const_iterator& other) noexcept
    {
      return m_bit - other.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator>(const const_iterator& lhs,
                                                  const const_iterator& rhs) noexcept
    {
      return lhs.m_bit > rhs.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator>=(const const_iterator& lhs,
                                                    const const_iterator& rhs) noexcept
    {
      return lhs.m_bit >= rhs.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator<(const const_iterator& lhs,
                                                  const const_iterator& rhs) noexcept
    {
      return lhs.m_bit < rhs.m_bit;
    }

    [[nodiscard]] friend constexpr bool operator<=(const const_iterator& lhs,
                                                    const const_iterator& rhs) noexcept
    {
      return lhs.m_bit <= rhs.m_bit;
    }

    [[nodiscard]] constexpr bool operator==(const const_iterator& other) const noexcept
    {
      return m_bit == other.m_bit;
    }

    [[nodiscard]] constexpr bool operator!=(const const_iterator& other) const noexcept
    {
      return m_bit != other.m_bit;
    }

    [[nodiscard]] constexpr bit_state operator*() const noexcept
    {
      return m_byte[byte_division(m_bit, UL)] & dynamic_bitset::bit_mask::BIT << byte_module(m_bit, UL);
    }

    [[nodiscard]] constexpr bit_state operator[](difference_type index) noexcept
    {
      return m_byte[byte_division(index, UL)] & dynamic_bitset::bit_mask::BIT << byte_module(index, UL);
    }

    constexpr const_iterator& operator=(const const_iterator&) noexcept = default;
    constexpr const_iterator& operator=(const_iterator&&) noexcept = default;

  };

 private:
  [[no_unique_address]] allocator_type xmalloc;
  pointer   m_storage;
  size_type m_bits;
  size_type m_bytes;

 private:
  [[nodiscard]] constexpr size_type calculate_capacity(size_type bits_number) const noexcept
  {
    return byte_division(bits_number, UL) + (byte_module(bits_number, UL) ? 1UL : 0UL);
  }

  constexpr void set_bit(size_type index, bit_state value) noexcept
  {
    if (value)
    {
      m_storage[byte_division(index, UL)] |= bit_mask::BIT << byte_module(index, UL);
    }
    else
    {
      m_storage[byte_division(index, UL)] &= ~(bit_mask::BIT << byte_module(index, UL));
    }
  }

  [[nodiscard]] constexpr bool resize_factor() const noexcept
  {
    return byte_division(m_bits, UL) == m_bytes;
  }

  constexpr void copy_data(pointer source, pointer destination, size_type n) noexcept
  {
    pointer end{source + n};

    [[likely]]
    while (source < end)
    {
      *destination++ = *source++;
    }
  }

  constexpr void fill_data(pointer source, size_type n, size_type value) noexcept
  {
    pointer end{source + n};

    [[likely]]
    while (source < end)
    {
      *source++ = value;
    }
  }

  constexpr void grow_init()
  {
    const size_type new_size{(m_bytes << 1UL) + 2UL};

    if (new_size >= nop::details::dynamic_bitset_limits::MAX_CAPACITY)
    {
      throw std::length_error{"dynamic_bitset::push_back(bit_state) -> exceeded maximum size"};
    }

    pointer temp_ptr{xmalloc.allocate(new_size)};

    if (m_storage)
    {
      std::copy(m_storage, m_storage + m_bytes, temp_ptr);
      xmalloc.deallocate(m_storage, m_bytes);
    }

    std::fill(temp_ptr + m_bytes, temp_ptr + new_size, bit_mask::RESET);
    m_bytes = new_size;
    m_storage = temp_ptr;
  }

 public:
  [[nodiscard]] constexpr iterator begin() noexcept
  {
    return {m_storage, ZERO_VALUE};
  }

  [[nodiscard]] constexpr iterator end() noexcept
  {
    return {m_storage, static_cast<difference_type>(m_bits)};
  }

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept
  {
    return {m_storage, ZERO_VALUE};
  }

  [[nodiscard]] constexpr const_iterator cend() const noexcept
  {
    return {m_storage, m_bits};
  }

  constexpr dynamic_bitset() noexcept(noexcept(allocator_type{}))
    : xmalloc{},
      m_storage{nullptr},
      m_bits{},
      m_bytes{}
  {
    /* Empty */
  }

  explicit constexpr dynamic_bitset([[maybe_unused]] const allocator_type& allocator) noexcept
    : xmalloc{allocator},
      m_storage{nullptr},
      m_bits{},
      m_bytes{}
  {
    /* Empty */
  }

  constexpr dynamic_bitset(size_type bits_number, size_type value = ZERO_VALUE, [[maybe_unused]] const allocator_type& allocator = allocator_type{})
    : xmalloc{allocator},
      m_bits{bits_number},
      m_bytes{calculate_capacity(bits_number)}
  {
    if (m_bits > nop::details::dynamic_bitset_limits::MAX_SIZE)
    {
      throw std::length_error{"dynamic_bitset::dynamic_bitset(size_type, size_type) -> invalid number of bits"};
    }
    else if (!bits_number)
    {
      return;
    }

    m_storage = xmalloc.allocate(m_bytes);

    for (size_type i{}; i < m_bytes; ++i)
    {
      m_storage[i] = bit_mask::RESET;
    }

    if (value)
    {
      *m_storage = value;
    }
  }

  constexpr dynamic_bitset(const dynamic_bitset& other)
    : xmalloc{},
      m_storage{nullptr},
      m_bits{other.m_bits},
      m_bytes{other.m_bytes}
  {
    if (m_bytes)
    {
      m_storage = xmalloc.allocate(m_bytes);
      copy_data(other.m_storage, m_storage, m_bytes);
    }
  }

  constexpr dynamic_bitset(const dynamic_bitset& other, [[maybe_unused]] const allocator_type& allocator)
    : xmalloc{allocator},
      m_storage{nullptr},
      m_bits{other.m_bits},
      m_bytes{other.m_bytes}
  {
    if (m_bytes)
    {
      m_storage = xmalloc.allocate(m_bytes);
      copy_data(other.m_storage, m_storage, m_bytes);
    }
  }

  constexpr dynamic_bitset(dynamic_bitset&& other) noexcept(noexcept(allocator_type{}))
    : xmalloc{},
      m_storage{other.m_storage},
      m_bits{other.m_bits},
      m_bytes{other.m_bytes}
  {
    other.m_storage = nullptr;
    other.m_bits = other.m_bytes = ZERO_VALUE;
  }

  constexpr dynamic_bitset(dynamic_bitset&& other, [[maybe_unused]] const allocator_type& allocator) noexcept
    : xmalloc{allocator},
      m_storage{other.m_storage},
      m_bits{other.m_bits},
      m_bytes{other.m_bytes}
  {
    other.m_storage = nullptr;
    other.m_bits = other.m_bytes = ZERO_VALUE;
  }

  constexpr ~dynamic_bitset()
  {
    if (m_storage)
    {
      xmalloc.deallocate(m_storage, m_bytes);
      m_storage = nullptr;
    }
  }

  [[nodiscard]] constexpr size_type capacity() const noexcept
  {
    return m_bytes << 6UL;
  }

  [[nodiscard]] constexpr size_type max_size() const noexcept
  {
    return nop::details::dynamic_bitset_limits::MAX_SIZE;
  }

  [[nodiscard]] constexpr pointer data() noexcept
  {
    return m_storage;
  }

  [[nodiscard]] constexpr const_pointer data() const noexcept
  {
    return m_storage;
  }

  [[nodiscard]] constexpr allocator_type get_allocator() const noexcept
  {
    return xmalloc;
  }

  [[nodiscard]] constexpr size_type count() const noexcept
  {
    if (!m_storage)
    {
      return ZERO_VALUE;
    }

    pointer end{m_storage + calculate_capacity(m_bits) - 1UL};
    size_type bit_count{};

    for (pointer begin{m_storage}; begin < end; ++begin)
    {
      bit_count += std::bitset<__WORDSIZE>(*begin).count(); /* Works for now */
    }

    const size_type remaining_bits{byte_module(m_bits, UL) ? byte_module(m_bits, UL) : __WORDSIZE};

    for (size_type current_bit{}; current_bit < remaining_bits; ++current_bit)
    {
      if (*end & bit_mask::BIT << current_bit)
      {
        ++bit_count;
      }
    }

    return bit_count;
  }

  constexpr void shrink_to_fit()
  {
    if (!m_bits)
    {
      clear();
      return;
    }

    const size_type current_bytes{calculate_capacity(m_bits)};

    if (current_bytes < m_bytes)
    {
      pointer temp_ptr{xmalloc.allocate(current_bytes)};
      copy_data(m_storage, temp_ptr, m_bytes);
      xmalloc.deallocate(m_storage, m_bytes);
      m_storage = temp_ptr;
      m_bytes = current_bytes;
    }
  }

  [[nodiscard]] constexpr storage_state any() const noexcept
  {
    if (!m_bits)
    {
      return !NONE_SET;
    }

    pointer end{m_storage + calculate_capacity(m_bits) - 1UL};

    for (pointer begin{m_storage}; begin < end; ++begin)
    {
      if (*begin)
      {
        return ANY_SET;
      }
    }
    const size_type remaining_bits{byte_module(m_bits, UL) ? byte_module(m_bits, UL) : __WORDSIZE};

    for (size_type current_bit{}; current_bit < remaining_bits; ++current_bit)
    {
      if (*end & bit_mask::BIT >> current_bit)
      {
        return ANY_SET;
      }
    }

    return !NONE_SET;
  }

  [[nodiscard]] constexpr storage_state none() const noexcept
  {
    if (!m_storage || !m_bits)
    {
      return NONE_SET;
    }
    pointer end{m_storage + calculate_capacity(m_bits) - 1UL};

    for (pointer begin{m_storage}; begin < end; ++begin)
    {
      if (*begin)
      {
        return !ANY_SET;
      }
    }

    const size_type remaining_bits{byte_module(m_bits, UL) ? byte_module(m_bits, UL) : __WORDSIZE};

    for (size_type current_bit{}; current_bit < remaining_bits; ++current_bit)
    {
      if (*end & dynamic_bitset::bit_mask::BIT >> current_bit)
      {
        return !ANY_SET;
      }
    }

    return NONE_SET;
  }

  [[nodiscard]] constexpr storage_state empty() const noexcept
  {
    return !m_bits;
  }

  // Modifiers
  constexpr void clear() noexcept
  {
    if (m_storage)
    {
      xmalloc.deallocate(m_storage, m_bytes);
      m_storage = nullptr;
      m_bits = m_bytes = ZERO_VALUE;
    }
  }

  constexpr void resize(size_type bits_number, bit_state value = BIT_UNSET)
  {
    if (!bits_number)
    {
      clear();
      return;
    }
    else if (bits_number == m_bits)
    {
      return;
    }

    const size_type new_size = calculate_capacity(bits_number);

    if (m_bytes < new_size)
    {
      pointer temp_ptr{xmalloc.allocate(new_size)};

      if (m_storage)
      {
        copy_data(m_storage, temp_ptr, m_bytes);
        xmalloc.deallocate(m_storage, m_bytes);
      }

      m_storage = temp_ptr;

      fill_data(m_storage + m_bytes,
                new_size - m_bytes,
                value ? bit_mask::SET : bit_mask::RESET);
      m_bytes = new_size;
      m_bits = bits_number;

      return;
    }

    while (m_bits < bits_number)
    {
      set_bit(m_bits++, value);
    }
  }

  constexpr void reserve(size_type bytes_number)
  {
    const size_type new_size{m_bytes + bytes_number};

    if (new_size > nop::details::dynamic_bitset_limits::MAX_CAPACITY)
    {
      throw std::length_error{"dynamic_bitset::reserve() -> invalid number of bytes"};
    }
    else if (!bytes_number ||
              m_bytes == nop::details::dynamic_bitset_limits::MAX_CAPACITY)
    {
      return;
    }

    pointer temp_ptr{xmalloc.allocate(new_size)};

    if (m_storage)
    {
      copy_data(m_storage, temp_ptr, m_bytes);
      xmalloc.deallocate(m_storage, m_bytes);
    }

    m_storage = temp_ptr;
    m_bytes = new_size;
  }

  constexpr void push_back(bit_state value)
  {
    if (resize_factor())
    {
      grow_init();
    }

    set_bit(m_bits++, value);
  }

  [[nodiscard]] constexpr size_type size() const noexcept
  {
    return m_bits;
  }

  constexpr void pop_back() noexcept
  {
    --m_bits;
  }

  constexpr dynamic_bitset& set(size_type index, bit_state value = BIT_UNSET)
  {
    if (index >= m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::set(size_type, bit_state) -> index is out of range"};
    }

    set_bit(index, value);
    return *this;
  }

  constexpr dynamic_bitset& set()
  {
    if (!m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::set() -> invalid number of bits"};
    }

    fill_data(m_storage, calculate_capacity(m_bits), bit_mask::SET);
    return *this;
  }

  constexpr dynamic_bitset& reset(size_type index)
  {
    if (index >= m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::reset(size_type) -> index is out of range"};
    }

    m_storage[byte_division(index, UL)] &= ~(bit_mask::BIT << byte_module(index, UL));
    return *this;
  }

  constexpr dynamic_bitset& reset()
  {
    if (!m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::reset() -> invalid number of bits"};
    }

    fill_data(m_storage, calculate_capacity(m_bits), bit_mask::RESET);
    return *this;
  }

  constexpr dynamic_bitset& flip(size_type index)
  {
    if (index >= m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::flip(size_type) -> index is out of range"};
    }

    m_storage[byte_division(index, UL)] ^= bit_mask::BIT << byte_module(index, UL);
    return *this;
  }

  constexpr dynamic_bitset& flip()
  {
    if (!m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::flip() -> invalid number of bits"};
    }

    pointer end{m_storage + calculate_capacity(m_bits)};

    [[likely]]
    for (pointer begin{m_storage}; begin < end; ++begin)
    {
      *begin ^= bit_mask::SET;
    }

    return *this;
  }

  constexpr void swap(dynamic_bitset& other) noexcept
  {
    if (this != std::addressof(other))
    {
      std::swap(m_storage, other.m_storage);
      std::swap(m_bits, other.m_bits);
      std::swap(m_bytes, other.m_bytes);
    }
  }

  // Element access
  [[nodiscard]] constexpr decltype(auto) operator[](size_type index) noexcept
  {
    return typename iterator::bit_reference{m_storage, static_cast<difference_type>(index)};
  }

  [[nodiscard]] constexpr bit_state operator[](size_type index) const noexcept
  {
    return m_storage[byte_division(index, UL)] & bit_mask::BIT << byte_module(index, UL);
  }

  [[nodiscard]] constexpr typename iterator::bit_reference front() noexcept
  {
    return {m_storage, ZERO_VALUE};
  }

  [[nodiscard]] constexpr bit_state front() const noexcept
  {
    return *m_storage & bit_mask::BIT;
  }

  [[nodiscard]] constexpr typename iterator::bit_reference back() noexcept
  {
    return {m_storage, static_cast<difference_type>(m_bits - 1UL)};
  }

  [[nodiscard]] constexpr bit_state back() const noexcept
  {
    return m_storage[byte_division(m_bits - 1UL, UL)] & bit_mask::BIT << byte_module(m_bits - 1UL, UL);
  }

  [[nodiscard]] constexpr typename iterator::bit_reference at(size_type index)
  {
    if (index >= m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::at(size_type) -> index is out of range"};
    }

    return {m_storage, static_cast<difference_type>(index)};
  }

  [[nodiscard]] constexpr bit_state at(size_type index) const
  {
    if (index >= m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::at(size_type) -> index is out of range"};
    }

    return m_storage[byte_division(index, UL)] & bit_mask::BIT << byte_module(index, UL);
  }

  // Operations
  dynamic_bitset& operator=(const dynamic_bitset& other)
  {
    if (this != std::addressof(other))
    {
      if (m_bytes != other.m_bytes)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = xmalloc.allocate(other.m_bytes);
      }

      copy_data(m_storage, other.m_storage, other.m_bytes);
      m_bits = other.m_bits;
      m_bytes = other.m_bytes;
    }

    return *this;
  }

  constexpr dynamic_bitset& operator=(dynamic_bitset&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      if (m_storage != nullptr)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = nullptr;
      }

      std::swap(m_storage, other.m_storage);
      m_bits = other.m_bits;
      m_bytes = other.m_bytes;
      other.m_bits = other.m_bytes = ZERO_VALUE;
    }

    return *this;
  }

  constexpr dynamic_bitset& operator&=(const dynamic_bitset& rhs)
  {
    if (m_bits != rhs.m_bits || !m_bits || !rhs.m_bits)
    {
      throw std::invalid_argument{"dynamic_bitset::operator(&=) -> invalid storage size"};
    }

    pointer begin_lhs{m_storage};
    pointer begin_rhs{rhs.m_storage};
    pointer end{m_storage + m_bytes};

    [[likely]]
    while (begin_lhs < end)
    {
      *begin_lhs++ &= *begin_rhs++;
    }

    return *this;
  }

  constexpr dynamic_bitset& operator|=(const dynamic_bitset& rhs)
  {
    if (m_bits != rhs.m_bits || !m_bits || !rhs.m_bits)
    {
      throw std::invalid_argument{"dynamic_bitset::operator(|=) -> invalid storage size"};
    }

    pointer begin_lhs{m_storage};
    pointer begin_rhs{rhs.m_storage};
    pointer end{m_storage + m_bytes};

    [[likely]]
    while (begin_lhs < end)
    {
      *begin_lhs++ |= *begin_rhs++;
    }

    return *this;
  }

  constexpr dynamic_bitset& operator^=(const dynamic_bitset& rhs)
  {
    if (m_bits != rhs.m_bits || !m_bits || !rhs.m_bits)
    {
      throw std::invalid_argument{"dynamic_bitset::operator(^=) -> invalid storage size"};
    }

    pointer begin_lhs{m_storage};
    pointer begin_rhs{rhs.m_storage};
    pointer end{m_storage + m_bytes};

    [[likely]]
    while (begin_lhs < end)
    {
      *begin_lhs++ ^= *begin_rhs++;
    }

    return *this;
  }

  [[nodiscard]] constexpr dynamic_bitset operator~() const
  {
    if (!m_storage)
    {
      throw std::out_of_range{"dynamic_bitset::operator(~) -> invalid storage pointer (nullptr)"};
    }

    auto temp_obj{*this};
    pointer end{temp_obj.m_storage + temp_obj.m_bytes};

    [[likely]]
    for (pointer begin{temp_obj.m_storage}; begin < end; ++begin)
    {
      *begin ^= bit_mask::SET;
    }

    return temp_obj;
  }

  constexpr dynamic_bitset& operator>>=(size_type bit_offset)
  {
    if (!m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::operator(>>=) -> invalid storage pointer (nullptr)"};
    }
    else if (bit_offset >= m_bits)
    {
      fill_data(m_storage, calculate_capacity(m_bits), bit_mask::RESET);
    }
    else if (bit_offset)
    {
      size_type total_shifts{bit_offset - 1UL};

      for (size_type BIT{m_bits - 1UL}; BIT > total_shifts; --BIT)
      {
        size_type bit_shift{BIT - bit_offset};
        size_type byte{m_storage[byte_division(bit_shift, UL)] &
                        bit_mask::BIT << byte_module(bit_shift, UL)};
        set_bit(BIT, byte);
      }

      fill_data(m_storage, byte_division(bit_offset, UL), bit_mask::RESET);

      for (size_type BIT{bit_offset - byte_module(bit_offset, UL)}; BIT != bit_offset; ++BIT)
      {
        set_bit(BIT, BIT_UNSET);
      }
    }

    return *this;
  }

  constexpr dynamic_bitset& operator<<=(size_type bit_offset)
  {
    if (!m_bits)
    {
      throw std::out_of_range{"dynamic_bitset::operator(<<=) -> invalid storage pointer (nullptr)"};
    }
    else if (bit_offset >= m_bits)
    {
      fill_data(m_storage, calculate_capacity(m_bits), bit_mask::RESET);
    }
    else if (bit_offset)
    {
      size_type total_shifts{m_bits - bit_offset};
      for (size_type BIT{}; BIT < total_shifts; ++BIT)
      {
        size_type bit_shift{BIT + bit_offset};
        size_type state{m_storage[byte_division(bit_shift, UL)] &
                        bit_mask::BIT << byte_module(bit_shift, UL)};
        set_bit(BIT, state);
      }
      size_type byte_shift{byte_division(bit_offset, UL)};
      fill_data(m_storage + (calculate_capacity(m_bits) - byte_shift),
                byte_shift,
                bit_mask::RESET);
      size_type bit_shift{m_bits - (byte_shift << 3UL)};
      for (size_type BIT{m_bits - bit_offset}; BIT != bit_shift; ++BIT)
      {
        set_bit(BIT, BIT_UNSET);
      }
    }

    return *this;
  }

  [[nodiscard]] constexpr bit_string to_string() const
  {
    bit_string storage_representation;
    storage_representation.reserve(m_bits);

    for (size_type current_bit{}; current_bit < m_bits; ++current_bit)
    {
      storage_representation.push_back(
      static_cast<bool>(m_storage[byte_division(current_bit, UL)] &
      bit_mask::BIT << byte_module(current_bit, UL)) + '0');
    }

    return storage_representation;
  }

};

} /* End namespace container */

} /* End namespace nop */

template<typename Alloc1, typename Alloc2>
[[nodiscard]] constexpr bool operator==(const nop::container::dynamic_bitset<Alloc1>& lhs,
                                        const nop::container::dynamic_bitset<Alloc2>& rhs) noexcept
{
  if (lhs.size() == rhs.size())
  {
    auto begin_lhs = lhs.data();
    auto end = begin_lhs + lhs.capacity();
    auto begin_rhs = rhs.data();

    [[likely]]
    while (begin_lhs < end)
    {
      if (*begin_lhs++ != *begin_rhs++)
      {
        return false;
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

template<typename Alloc1, typename Alloc2>
[[nodiscard]] constexpr bool operator!=(const nop::container::dynamic_bitset<Alloc1>& lhs,
                                        const nop::container::dynamic_bitset<Alloc2>& rhs) noexcept
{
  return !(lhs == rhs);
}

template<typename Alloc>
[[nodiscard]] constexpr nop::container::dynamic_bitset<Alloc> operator&(const nop::container::dynamic_bitset<Alloc>& lhs,
                                                                        const nop::container::dynamic_bitset<Alloc>& rhs)
{
  auto temp_dynamic_bitset{lhs};
  return temp_dynamic_bitset &= rhs;
}

template<class Alloc>
[[nodiscard]] constexpr nop::container::dynamic_bitset<Alloc> operator|(const nop::container::dynamic_bitset<Alloc>& lhs,
                                                                        const nop::container::dynamic_bitset<Alloc>& rhs)
{
  auto temp_dynamic_bitset{lhs};
  return temp_dynamic_bitset |= rhs;
}

template<class Alloc>
[[nodiscard]] constexpr nop::container::dynamic_bitset<Alloc> operator^(const nop::container::dynamic_bitset<Alloc>& lhs,
                                                                        const nop::container::dynamic_bitset<Alloc>& rhs)
{
  auto temp_dynamic_bitset{lhs};
  return temp_dynamic_bitset ^= rhs;
}

template<class Alloc>
[[nodiscard]] constexpr nop::container::dynamic_bitset<Alloc> operator<<(const nop::container::dynamic_bitset<Alloc>& dynamic_bitset_obj,
                                                                         const std::size_t bit_offset)
{
  auto temp_dynamic_bitset{dynamic_bitset_obj};
  return temp_dynamic_bitset <<= bit_offset;
}

template<class Alloc>
[[nodiscard]] constexpr nop::container::dynamic_bitset<Alloc> operator>>(const nop::container::dynamic_bitset<Alloc>& dynamic_bitset_obj,
                                                                         const std::size_t bit_offset)
{
  auto temp_dynamic_bitset{dynamic_bitset_obj};
  return temp_dynamic_bitset >>= bit_offset;
}

namespace std /* Begin namespace std */
{

template<class Alloc>
struct formatter<nop::container::dynamic_bitset<Alloc>> : formatter<string>
{
  [[nodiscard]] auto format(const nop::container::dynamic_bitset<Alloc> dynamic_bitset_obj, format_context& ctx) const
  {
    return formatter<string>::format(dynamic_bitset_obj.to_string(), ctx);
  }
};

template<class Alloc>
constexpr void swap(nop::container::dynamic_bitset<Alloc>& lhs, nop::container::dynamic_bitset<Alloc>& rhs) noexcept
{
  lhs.swap(rhs);
}

} /* End namespace std */

#include "undef_dynamic_bitset_extension.hpp"

#endif /* End __nop_details::container::dynamic_bitset_impl header file */