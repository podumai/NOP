#ifndef NOP_INPLACE_VECTOR_HPP /* Begin inplace vector header file */
#define NOP_INPLACE_VECTOR_HPP 1UL

#pragma once

#include <type_traits>
#include <iterator> /* std::reverse_iterator */
#include <stdexcept> /* std::out_of_range, std::length_error, std::invalid_argument */
#include <initializer_list> /* std::initializer_list<T> */

#include "inplace_vector_base.hpp"

namespace nop /* Begin namespace nop */
{

namespace strategy /* Begin namespace strategy */
{

struct inplace_vector_nothrow
{
  /* Empty */
};

struct inplace_vector_throw
{
  /* Empty */
};

} /* End namespace strategy */

namespace details /* Begin namespace details */
{

template<typename T>
concept valid_inplace_vector_policy = std::is_base_of_v<nop::strategy::inplace_vector_nothrow, T> ||
                                      std::is_base_of_v<nop::strategy::inplace_vector_throw, T>;

} /* End namespace details */

template<
         typename T,
         std::size_t N,
         nop::details::valid_inplace_vector_policy Policy = nop::strategy::inplace_vector_nothrow
        >
requires std::is_nothrow_destructible_v<T>
class inplace_vector : public nop::details::inplace_vector_base<T, N>
{
 private:
  using base = nop::details::inplace_vector_base<T, N>;

 public:
  class iterator;
  class const_iterator;

 public:
  using value_type             = base::value_type;
  using size_type              = base::size_type;
  using difference_type        = base::difference_type;
  using reference              = base::reference;
  using const_reference        = base::const_reference;
  using pointer                = base::pointer;
  using const_pointer          = base::const_pointer;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 public:
  class iterator
  {
   private:
    friend inplace_vector;

   public:
    using value_type        = inplace_vector::value_type;
    using size_type         = inplace_vector::size_type;
    using difference_type   = inplace_vector::difference_type;
    using reference         = inplace_vector::reference;
    using const_reference   = inplace_vector::const_reference;
    using pointer           = inplace_vector::pointer;
    using const_pointer     = inplace_vector::const_pointer;
    using iterator_category = std::contiguous_iterator_tag;

   private:
    pointer m_element;

   public:
    iterator() noexcept
      : m_element{nullptr}
    {
      /* Empty */
    }

    iterator(pointer ptr) noexcept
      : m_element{ptr}
    {
      /* Empty */
    }

    iterator(const iterator&) noexcept = default;
    iterator(iterator&&) noexcept = default;
    ~iterator() = default;

    [[nodiscard]] reference operator*() noexcept
    {
      return *m_element;
    }

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return *m_element;
    }

    [[nodiscard]] pointer operator->() noexcept
    {
      return m_element;
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return m_element;
    }

    [[nodiscard]] reference operator[](difference_type index) noexcept
    {
      return m_element[index];
    }

    [[nodiscard]] const_reference operator[](difference_type index) const noexcept
    {
      return m_element[index];
    }

    iterator& operator++() noexcept
    {
      ++m_element;
      return *this;
    }

    iterator operator++(std::int32_t) noexcept
    {
      return {m_element++};
    }

    iterator& operator--() noexcept
    {
      --m_element;
      return *this;
    }

    iterator operator--(std::int32_t) noexcept
    {
      return {m_element--};
    }

    iterator& operator+=(difference_type offset) noexcept
    {
      m_element += offset;
      return *this;
    }

    iterator& operator-=(difference_type offset) noexcept
    {
      m_element -= offset;
      return *this;
    }

    [[nodiscard]] iterator operator+(difference_type offset) noexcept
    {
      return {m_element + offset};
    }

    [[nodiscard]] friend iterator operator+(difference_type offset,
                                            const iterator& iter) noexcept
    {
      return {iter.m_element + offset};
    }

    [[nodiscard]] iterator operator-(difference_type offset) noexcept
    {
      return {m_element - offset};
    }

    [[nodiscard]] difference_type operator-(const iterator& other) noexcept
    {
      return m_element - other.m_element;
    }

    [[nodiscard]] bool operator==(const iterator& other) const noexcept
    {
      return m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const iterator& other) const noexcept
    {
      return m_element != other.m_element;
    }

    [[nodiscard]] bool operator<(const iterator& other) const noexcept
    {
      return m_element < other.m_element;
    }

    [[nodiscard]] bool operator<=(const iterator& other) const noexcept
    {
      return m_element <= other.m_element;
    }

    [[nodiscard]] bool operator>(const iterator& other) const noexcept
    {
      return m_element > other.m_element;
    }

    [[nodiscard]] bool operator>=(const iterator& other) const noexcept
    {
      return m_element >= other.m_element;
    }

    iterator& operator=(const iterator&) noexcept = default;
    iterator& operator=(iterator&&) noexcept = default;

  };

  class const_iterator
  {
   private:
    friend inplace_vector;

   public:
    using value_type        = const inplace_vector::value_type;
    using size_type         = inplace_vector::size_type;
    using difference_type   = inplace_vector::difference_type;
    using reference         = inplace_vector::reference;
    using const_reference   = inplace_vector::const_reference;
    using pointer           = inplace_vector::pointer;
    using const_pointer     = inplace_vector::const_pointer;
    using iterator_category = std::contiguous_iterator_tag;

   private:
    const_pointer m_element;

   public:
    const_iterator() noexcept
      : m_element{nullptr}
    {
      /* Empty */
    }

    const_iterator(const_pointer ptr) noexcept
      : m_element{ptr}
    {
      /* Empty */
    }

    const_iterator(const const_iterator& other) noexcept = default;
    const_iterator(const_iterator&& other) noexcept = default;
    ~const_iterator() = default;

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return *m_element;
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return m_element;
    }

    [[nodiscard]] const_reference operator[](difference_type index) const noexcept
    {
      return m_element[index];
    }

    const_iterator& operator++() noexcept
    {
      ++m_element;
      return *this;
    }

    const_iterator operator++(std::int32_t) noexcept
    {
      return {m_element++};
    }

    const_iterator& operator--() noexcept
    {
      --m_element;
      return *this;
    }

    const_iterator operator--(std::int32_t) noexcept
    {
      return {m_element--};
    }

    const_iterator& operator+=(difference_type offset) noexcept
    {
      m_element += offset;
      return *this;
    }

    [[nodiscard]] const_iterator operator+(difference_type offset) noexcept
    {
      return {m_element + offset};
    }

    [[nodiscard]] friend const_iterator operator+(difference_type offset,
                                                  const const_iterator& iter) noexcept
    {
      return {offset + iter.m_element};
    }

    const_iterator& operator-=(difference_type offset) noexcept
    {
      m_element -= offset;
      return *this;
    }

    [[nodiscard]] const_iterator operator-(difference_type offset) noexcept
    {
      return {m_element - offset};
    }

    [[nodiscard]] friend const_iterator operator-(difference_type offset,
                                                  const const_iterator& other) noexcept
    {
      return {offset - other.m_element};
    }

    [[nodiscard]] difference_type operator-(const const_iterator& other) const noexcept
    {
      return m_element - other.m_element;
    }

    [[nodiscard]] bool operator==(const const_iterator& other) const noexcept
    {
      return m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const const_iterator& other) const noexcept
    {
      return m_element != other.m_element;
    }

    [[nodiscard]] bool operator>(const const_iterator& other) const noexcept
    {
      return m_element > other.m_element;
    }

    [[nodiscard]] bool operator>=(const const_iterator& other) const noexcept
    {
      return m_element >= other.m_element;
    }

    [[nodiscard]] bool operator<(const const_iterator& other) const noexcept
    {
      return m_element < other.m_element;
    }

    [[nodiscard]] bool operator<=(const const_iterator& other) const noexcept
    {
      return m_element <= other.m_element;
    }

    const_iterator& operator=(const const_iterator&) noexcept = default;
    const_iterator& operator=(const_iterator&&) noexcept = default;

  };

 private:
  std::conditional_t<std::is_fundamental_v<value_type>, value_type, std::uint8_t> m_storage[std::is_fundamental_v<value_type> ? N : sizeof(value_type) * N];
  size_type m_size;

 private:
  constexpr void range_check(size_type index) const
  requires std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>
  {
    if (m_size <= index)
    {
      throw std::out_of_range{"inplace_vector::range_check(size_type) -> index is out of range"};
    }
  }

 public:
  inplace_vector() noexcept
    : m_size{}
  {
    /* Empty */
  }

  inplace_vector(const inplace_vector& other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
    : m_size{}
  {
    while (m_size < other.m_size)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size) value_type(static_cast<pointer>(static_cast<void*>(other))[m_size]);
      }
      else
      {
        (void) ::new (m_storage + m_size) value_type(other.m_storage[m_size]);
      }

      ++m_size;
    }
  }

  inplace_vector(inplace_vector&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
    : m_size{}
  {
    while (m_size < other.m_size)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size) value_type(std::move(static_cast<pointer>(static_cast<void*>(other.m_storage))[m_size]));
      }
      else
      {
        (void) ::new (m_storage + m_size) value_type(other.m_storage[m_size]);
      }

      ++m_size;
    }
  }

  explicit inplace_vector(size_type n) noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy> &&
                                                std::is_nothrow_default_constructible_v<value_type>)
  requires std::is_default_constructible_v<value_type>
    : m_size{}
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (n > N)
      {
        throw std::length_error{"inplace_vector::inplace_vector(size_type) -> invalid number of elements"};
      }
    }

    while (m_size < n)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type();
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type();
      }
    }
  }

  inplace_vector(size_type n, const value_type& value) noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy> &&
                                                                std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
    : m_size{}
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (n > N)
      {
        throw std::length_error{"inplace_vector::inplace_vector(size_type, const value_type&) -> invalid number of elements"};
      }
    }

    while (m_size < n)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(value);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(value);
      }
    }
  }

  inplace_vector(std::initializer_list<value_type> ilist) noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy> &&
                                                                   std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
    : m_size{}
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (ilist.size() > N)
      {
        throw std::length_error{"inplace_vector::inplace_vector(std::initializer_list) -> invalid number of elements"};
      }
    }

    for (auto begin{ilist.begin()}; begin != ilist.end(); ++begin)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(*begin);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(*begin);
      }
    }
  }

  template<typename InIterator>
  requires std::is_copy_constructible_v<value_type>
  inplace_vector(InIterator begin, InIterator end) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
    : m_size{}
  {
    while (begin != end)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(*begin++);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(*begin++);
      }
    }
  }

  ~inplace_vector()
  {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      while (m_size)
      {
        (static_cast<pointer>(static_cast<void*>(m_storage)) + --m_size)->~value_type();
      }
    }
  }

  [[nodiscard]] iterator begin() noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return {static_cast<pointer>(static_cast<void*>(m_storage))};
    }
    else
    {
      return {m_storage};
    }
  }

  [[nodiscard]] iterator end() noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return {static_cast<pointer>(static_cast<void*>(m_storage)) + m_size};
    }
    else
    {
      return {m_storage + m_size};
    }
  }

  [[nodiscard]] const_iterator cbegin() const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return {static_cast<const_pointer>(static_cast<void*>(m_storage))};
    }
    else
    {
      return {m_storage};
    }
  }

  [[nodiscard]] const_iterator cend() const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return {static_cast<const_pointer>(static_cast<const void*>(m_storage)) + m_size};
    }
    else
    {
      return {m_storage + m_size};
    }
  }

  [[nodiscard]] std::reverse_iterator<iterator> rbegin() noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return std::make_reverse_iterator<iterator>(static_cast<pointer>(static_cast<void*>(m_storage)) + m_size);
    }
    else
    {
      return std::make_reverse_iterator<iterator>(m_storage + m_size);
    }
  }

  [[nodiscard]] std::reverse_iterator<iterator> rend() noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return std::make_reverse_iterator<iterator>(static_cast<pointer>(static_cast<void*>(m_storage)));
    }
    else
    {
      return std::make_reverse_iterator<iterator>(m_storage);
    }
  }

  [[nodiscard]] std::reverse_iterator<const_iterator> crbegin() const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return std::make_reverse_iterator<const_iterator>(static_cast<pointer>(static_cast<void*>(m_storage)) + m_size);
    }
    else
    {
      return std::make_reverse_iterator<const_iterator>(m_storage + m_size);
    }
  }

  [[nodiscard]] std::reverse_iterator<const_iterator> crend() const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return std::make_reverse_iterator<const_iterator>(static_cast<pointer>(static_cast<void*>(m_storage)));
    }
    else
    {
      return std::make_reverse_iterator<const_iterator>(m_storage);
    }
  }

  [[nodiscard]] pointer data() noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return static_cast<pointer>(static_cast<void*>(m_storage));
    }
    else
    {
      return m_storage;
    }
  }

  [[nodiscard]] const_pointer data() const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return static_cast<const_pointer>(static_cast<const void*>(m_storage));
    }
    else
    {
      return m_storage;
    }
  }

  [[nodiscard]] size_type size() const noexcept
  {
    return m_size;
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return !m_size;
  }

  void clear() noexcept(std::is_nothrow_destructible_v<value_type>)
  {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      while (m_size)
      {
        (static_cast<pointer>(static_cast<void*>(m_storage)) + --m_size)->~value_type();
      }
    }
  }

  void push_back(const value_type& value) noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy> &&
                                                   std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(value);
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(value);
        }
      }
    }
    else
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(value);
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(value);
        }
      }
      else
      {
        throw std::out_of_range{"inplace_vector::push_back() -> storage overflow"};
      }
    }
  }

  void push_back(value_type&& value) noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy> &&
                                              std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::move(value));
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(value);
        }
      }
    }
    else
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::move(value));
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(value);
        }
      }
      else
      {
        throw std::length_error{"inplace_vector::push_back() -> storage overflow"};
      }
    }
  }

  pointer try_push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
  {
    if (m_size < N)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        return ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(value);
      }
      else
      {
        return ::new (m_storage + m_size++) value_type(value);
      }
    }
    else
    {
      return nullptr;
    }
  }

  pointer try_push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
  {
    if (m_size < N)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        return ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::move(value));
      }
      else
      {
        return ::new (m_storage + m_size++) value_type(value);
      }
    }
    else
    {
      return nullptr;
    }
  }

  reference unchecked_push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return *(::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(value));
    }
    else
    {
      return *(::new (m_storage + m_size++) value_type(value));
    }
  }

  reference unchecked_push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return *(::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::move(value)));
    }
    else
    {
      return *(::new (m_storage + m_size++) value_type(value));
    }
  }

  template<typename... Args>
  void emplace_back(Args&&... args)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::forward<Args>(args)...);
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(std::forward<Args>(args)...);
        }
      }
    }
    else
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::forward<Args>(args)...);
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(std::forward<Args>(args)...);
        }
      }
      else
      {
        throw std::length_error{"inplace_vector::emplace_back() -> storage overflow"};
      }
    }
  }

  template<typename... Args>
  pointer try_emplace_back(Args&&... args)
  {
    if (m_size < N)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        return ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::forward<Args>(args)...);
      }
      else
      {
        return ::new (m_storage + m_size++) value_type(std::forward<Args>(args)...);
      }
    }
    else
    {
      return nullptr;
    }
  }

  template<typename... Args>
  reference unchecked_emplace_back(Args&&... args)
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return *(::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::forward<Args>(args)...));
    }
    else
    {
      return *(::new (m_storage + m_size++) value_type(std::forward<Args>(args)...));
    }
  }

  void pop_back() noexcept(std::is_nothrow_destructible_v<value_type> &&
                           std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (!m_size)
      {
        throw std::out_of_range{"inplace_vector::pop_back() -> storage underflow"};
      }
    }

    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      (static_cast<pointer>(static_cast<void*>(m_storage)) + --m_size)->~value_type();
    }
    else
    {
      --m_size;
    }
  }

  [[nodiscard]] reference operator[](size_type index) noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return static_cast<pointer>(static_cast<void*>(m_storage))[index];
    }
    else
    {
      return m_storage[index];
    }
  }

  [[nodiscard]] const_reference operator[](size_type index) const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return static_cast<pointer>(static_cast<void*>(m_storage))[index];
    }
    else
    {
      return m_storage[index];
    }
  }

  [[nodiscard]] reference front() noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      this->range_check(0UL);
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return *static_cast<pointer>(static_cast<void*>(m_storage));
    }
    else
    {
      return *m_storage;
    }
  }

  [[nodiscard]] const_reference front() const noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      this->range_check(0UL);
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return *static_cast<pointer>(static_cast<void*>(m_storage));
    }
    else
    {
      return *m_storage;
    }
  }

  [[nodiscard]] reference back() noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      this->range_check(m_size - 1UL);
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return static_cast<pointer>(static_cast<void*>(m_storage))[m_size - 1UL];
    }
    else
    {
      return m_storage[m_size - 1UL];
    }
  }

  [[nodiscard]] const_reference back() const noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      this->range_check(m_size - 1UL);
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return static_cast<pointer>(static_cast<void*>(m_storage))[m_size - 1UL];
    }
    else
    {
      return m_storage[m_size - 1UL];
    }
  }

  [[nodiscard]] reference at(size_type index) noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      this->range_check(index);
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return static_cast<pointer>(static_cast<void*>(m_storage))[index];
    }
    else
    {
      return m_storage[index];
    }
  }

  [[nodiscard]] const_reference at(size_type index) const noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      this->range_check(index);
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return static_cast<pointer>(static_cast<void*>(m_storage))[index];
    }
    else
    {
      return m_storage[index];
    }
  }

  void assign(std::initializer_list<value_type> ilist) noexcept(std::is_fundamental_v<value_type>)
  requires (std::is_copy_constructible_v<value_type> &&
            std::is_copy_assignable_v<value_type>)
  {
    auto begin{ilist.begin()};

    if (m_size)
    {
      for (size_type i{}; i < m_size && begin != ilist.end(); ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          static_cast<pointer>(static_cast<void*>(m_storage))[i] = *begin++;
        }
        else
        {
          m_storage[i] = *begin++;
        }
      }
    }

    while (m_size < N && begin != ilist.end())
    {
      if constexpr (!std::is_fundamental_v<value_type> &&
                    std::is_move_constructible_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::move(*begin++));
      }
      else if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(*begin++);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(*begin++);
      }
    }
  }

  template<typename InIterator>
  requires (std::is_copy_constructible_v<value_type> &&
            std::is_copy_assignable_v<value_type>)
  void assign(InIterator begin, InIterator end)
  {
    if (m_size)
    {
      for (size_type i{}; i < m_size && begin != end; ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          static_cast<pointer>(static_cast<void*>(m_storage))[i] = *begin++;
        }
        else
        {
          m_storage[i] = *begin++;
        }
      }
    }

    while (m_size < N && begin != end)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        static_cast<pointer>(static_cast<void*>(m_storage))[m_size++] = *begin++;
      }
      else
      {
        m_storage[m_size++] = *begin++;
      }
    }
  }

  void assign(size_type n, const value_type& value)
  requires (std::is_copy_constructible_v<value_type> &&
            std::is_copy_assignable_v<value_type>)
  {
    if (m_size)
    {
      for (size_type i{}; i < m_size && i < n; ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          static_cast<pointer>(static_cast<void*>(m_storage))[i] = value;
        }
        else
        {
          m_storage[i] = value;
        }
      }
    }


    while (m_size < N && m_size < n)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(value);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(value);
      }
    }
  }

  iterator insert(const_iterator position, const value_type& value)
  requires ((std::is_copy_constructible_v<value_type>  ||
             std::is_move_constructible_v<value_type>) &&
            (std::is_copy_assignable_v<value_type>     ||
             std::is_move_assignable_v<value_type>))
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (m_size == N)
      {
        throw std::length_error{"inplace_vector::insert(const_iterator, const value_type&) -> storage overflow"};
      }
    }

    if (m_size)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        pointer begin{static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++ - 1UL};

        if constexpr (std::is_move_constructible_v<value_type>)
        {
          (void) ::new (begin + 1UL) value_type(std::move(*begin));
        }
        else
        {
          (void) ::new (begin + 1UL) value_type(*begin);
        }

        pointer end{const_cast<pointer>(position.m_element)};

        while (begin >= end)
        {
          if constexpr (std::is_move_assignable_v<value_type>)
          {
            *begin = std::move(begin[-1UL]);
          }
          else
          {
            *begin = begin[-1UL];
          }

          --begin;
        }

        *begin = value;

        return {begin};
      }
      else
      {
        pointer begin{m_storage + m_size++ - 1UL};

        (void) ::new (begin + 1UL) value_type(*begin);

        pointer end{const_cast<pointer>(position.m_element)};

        while (begin >= end)
        {
          (void) ::new (begin) value_type(begin[-1UL]);
          --begin;
        }

        return {::new (begin) value_type(value)};
      }
    }
    else
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        return {::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(value)};
      }
      else
      {
        return {::new (m_storage + m_size++) value_type(value)};
      }
    }
  }

  iterator insert(const_iterator position, value_type&& value)
  requires (std::is_move_constructible_v<value_type> &&
            std::is_move_assignable_v<value_type>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (m_size == N)
      {
        throw std::length_error{"inplace_vector::insert(const_iterator, value_type&&) -> storage overflow"};
      }
    }

    if (m_size)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        pointer begin{static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++ - 1UL};

        (void) ::new (begin + 1UL) value_type(std::move(*begin));

        pointer end{const_cast<pointer>(position.m_element)};

        while (begin >= end)
        {
          *begin = std::move(begin[-1UL]);
          --begin;
        }

        *begin = std::move(value);

        return {begin};
      }
      else
      {
        pointer begin{m_storage + m_size++ - 1UL};

        (void) ::new (begin + 1UL) value_type(*begin);

        pointer end{const_cast<pointer>(position.m_element)};

        while (begin >= end)
        {
          (void) ::new (begin) value_type(begin[-1UL]);
          --begin;
        }

        return {::new (begin) value_type(value)};
      }
    }
    else
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        return {::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size++) value_type(std::move(value))};
      }
      else
      {
        return {::new (m_storage + m_size++) value_type(value)};
      }
    }
  }

  void swap(inplace_vector& other)
  requires (std::is_copy_constructible_v<value_type> ||
            std::is_move_constructible_v<value_type>)
  {
    if (this != std::addressof(other))
    {
      size_type old_self_size{m_size};
      size_type old_other_size{other.m_size};

      for (size_type i{}; i < m_size && i < other.m_size; ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          std::swap(static_cast<pointer>(static_cast<void*>(m_storage))[i], static_cast<pointer>(static_cast<void*>(other.m_storage))[i]);
        }
        else
        {
          std::swap(m_storage[i], other.m_storage[i]);
        }
      }

      if (m_size < other.m_size)
      {
        while (m_size < other.m_size)
        {
          if constexpr (std::is_move_constructible_v<value_type> &&
                        !std::is_fundamental_v<value_type>)
          {
            (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size) value_type(std::move(static_cast<pointer>(static_cast<void*>(other.m_storage))[m_size]));
          }
          else if constexpr (!std::is_fundamental_v<value_type>)
          {
            (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size) value_type(static_cast<pointer>(static_cast<void*>(other.m_storage))[m_size]);
          }
          else
          {
            (void) ::new (m_storage + m_size) value_type(other.m_storage[m_size]);
          }

          if constexpr (!std::is_trivially_destructible_v<value_type>)
          {
            (static_cast<pointer>(static_cast<void*>(other.m_storage)) + m_size++)->~value_type();
          }
          else
          {
            ++m_size;
          }
        }
      }
      else
      {
        while (other.m_size < m_size)
        {
          if constexpr (std::is_move_constructible_v<value_type> &&
                        !std::is_fundamental_v<value_type>)
          {
            (void) ::new (static_cast<pointer>(static_cast<void*>(other.m_storage)) + other.m_size) value_type(std::move(reinterpret_cast<pointer>(static_cast<void*>(m_storage))[other.m_size]));
          }
          else if constexpr (!std::is_fundamental_v<value_type>)
          {
            (void) ::new (static_cast<pointer>(static_cast<void*>(other.m_storage)) + other.m_size) value_type(static_cast<pointer>(static_cast<void*>(m_storage))[other.m_size]);
          }
          else
          {
            (void) ::new (other.m_storage + other.m_size) value_type(m_storage[other.m_size]);
          }

          if constexpr (!std::is_trivially_destructible_v<value_type>)
          {
            (static_cast<pointer>(static_cast<void*>(m_storage)) + other.m_size++)->~value_type();
          }
          else
          {
            ++other.m_size;
          }
        }
      }

      m_size = old_other_size;
      other.m_size = old_self_size;
    }
  }

  iterator erase(const_iterator position)
  requires (std::is_move_assignable_v<value_type> ||
            std::is_copy_assignable_v<value_type>)
  {
    if (m_size)
    {
      for (size_type i{static_cast<size_type>(position.m_element - static_cast<pointer>(static_cast<void*>(m_storage)))}; i < m_size - 1UL; ++i)
      {
        if constexpr (std::is_move_assignable_v<value_type> &&
                      !std::is_fundamental_v<value_type>)
        {
          static_cast<pointer>(static_cast<void*>(m_storage))[i] = std::move(static_cast<pointer>(static_cast<void*>(m_storage))[i + 1UL]);
        }
        else if constexpr (!std::is_fundamental_v<value_type>)
        {
          static_cast<pointer>(static_cast<void*>(m_storage))[i] = static_cast<pointer>(static_cast<void*>(m_storage))[i + 1UL];
        }
        else
        {
          m_storage[i] = m_storage[i + 1UL];
        }
      }

      if constexpr (!std::is_trivially_destructible_v<value_type>)
      {
        (static_cast<pointer>(static_cast<void*>(m_storage)) + --m_size)->~value_type();
      }
      else
      {
        --m_size;
      }
    }

    return {const_cast<pointer>(position.m_element)};
  }

  inplace_vector& operator=(std::initializer_list<value_type> ilist)
  requires (std::is_copy_constructible_v<value_type> &&
            std::is_copy_assignable_v<value_type>)
  {
    this->assign(ilist);
    return *this;
  }

  inplace_vector& operator=(const inplace_vector& other)
  requires ((std::is_copy_assignable_v<value_type> ||
            std::is_copy_constructible_v<value_type>) &&
            (std::is_copy_assignable_v<value_type> ||
             std::is_move_assignable_v<value_type>))
  {
    if (this != std::addressof(other))
    {
      this->assign(other.begin(), other.end());
    }

    return *this;
  }

  inplace_vector& operator=(inplace_vector&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires (std::is_move_assignable_v<value_type> &&
            std::is_move_constructible_v<value_type>)
  {
    if (this != std::addressof(other))
    {
      for (size_type i{}; i < m_size && i < other.m_size; ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          static_cast<pointer>(static_cast<void*>(m_storage))[i] = std::move(static_cast<pointer>(static_cast<void*>(other.m_storage))[i]);
        }
        else
        {
          (void) ::new (m_storage + i) value_type(other.m_storage[i]);
        }
      }

      while (m_size < other.m_size)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (static_cast<pointer>(static_cast<void*>(m_storage)) + m_size) value_type(std::move(static_cast<pointer>(static_cast<void*>(other.m_storage))[m_size]));
        }
        else
        {
          (void) ::new (m_storage + m_size) value_type(other.m_storage[m_size]);
        }

        ++m_size;
      }
    }

    return *this;
  }

};

} /* End namespace nop */

template<typename T, std::size_t N, class Policy>
[[nodiscard]] bool operator==(const nop::inplace_vector<T, N, Policy>& lhs,
                              const nop::inplace_vector<T, N, Policy>& rhs) noexcept
{
  if (lhs.size() == rhs.size())
  {
    for (std::size_t i{}; i < lhs.size(); ++i)
    {
      if (lhs[i] != rhs[i])
      {
        return false;
      }
    }

    return true;
  }

  return false;
}

template<typename T, std::size_t N, class Policy>
[[nodiscard]] bool operator!=(const nop::inplace_vector<T, N, Policy>& lhs,
                              const nop::inplace_vector<T, N, Policy>& rhs) noexcept
{
  if (lhs.size() == rhs.size())
  {
    for (std::size_t i{}; i < lhs.size(); ++i)
    {
      if (lhs[i] != rhs[i])
      {
        return true;
      }
    }
  }

  return false;
}

namespace std /* Begin namespace std */
{

template<typename T, std::size_t N, class Policy>
constexpr void swap(nop::inplace_vector<T, N, Policy>& lhs, nop::inplace_vector<T, N, Policy>& rhs)
{
  lhs.swap(rhs);
}

} /* End namespace std */

#endif /* End inplace vector header file */
