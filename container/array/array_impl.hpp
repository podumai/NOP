#ifndef NOP_DETAILS_ARRAY_IMPL_HPP /* Begin nop_details array_impl header file */
#define NOP_DETAILS_ARRAY_IMPL_HPP 1UL

#pragma once

#ifndef NOP_CONTAINER_ARRAY_HPP
  #error "This header file cannot be included explicitly. Use array.hpp instead."
#endif

#include <type_traits>
#include <concepts>
#include <iterator>
#include <stdexcept>
#include <algorithm>

#include "NOP/algorithm/algo_base/copy.hpp"
#include "NOP/algorithm/algo_base/move.hpp"
#include "NOP/algorithm/algo_base/equal.hpp"
#include "array_base.hpp"

namespace nop_details /* Begin namespace nop_details */
{

template<typename T>
concept valid_array_t = std::default_initializable<T> &&
                        std::destructible<T>;

namespace container /* Begin namespace container */
{

template<
         nop_details::valid_array_t T,
         std::size_t                N
        >
class array_impl : public nop_details::container::array_base<T, N>
{
 private:
  using base = typename nop_details::container::array_base<T, N>;

 public:
  using value_type             = typename base::value_type;
  using size_type              = typename base::size_type;
  using difference_type        = typename base::difference_type;
  using reference              = typename base::reference;
  using const_reference        = typename base::const_reference;
  using pointer                = typename base::pointer;
  using const_pointer          = typename base::const_pointer;
  class iterator;
  class const_iterator;
  using reverse_iterator       = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

 public:
  class iterator
  {
   private:
    friend array_impl;

   public:
    using value_type        = typename array_impl::value_type;
    using difference_type   = typename array_impl::difference_type;
    using reference         = typename array_impl::reference;
    using const_reference   = typename array_impl::const_reference;
    using pointer           = typename array_impl::pointer;
    using const_pointer     = typename array_impl::const_pointer;
    using iterator_category = typename std::contiguous_iterator_tag;

   private:
    pointer m_element;

   public:
    constexpr iterator() noexcept
      : m_element{nullptr}
    {
      /* Empty */
    }

    constexpr iterator(pointer ptr) noexcept
      : m_element{ptr}
    {
      /* Empty */
    }

    constexpr iterator(const iterator&) noexcept = default;
    constexpr iterator(iterator&&)      noexcept = default;

    [[nodiscard]] constexpr reference operator*() noexcept
    {
      return *this->m_element;
    }

    [[nodiscard]] constexpr const_reference operator*() const noexcept
    {
      return *this->m_element;
    }

    [[nodiscard]] constexpr pointer operator->() noexcept
    {
      return this->m_element;
    }

    [[nodiscard]] constexpr const_pointer operator->() const noexcept
    {
      return this->m_element;
    }

    constexpr iterator& operator++() noexcept
    {
      ++this->m_element;
      return *this;
    }

    constexpr iterator operator++(std::int32_t) noexcept
    {
      return {this->m_element++};
    }

    constexpr iterator& operator--() noexcept
    {
      --this->m_element;
      return *this;
    }

    constexpr iterator operator--(std::int32_t) noexcept
    {
      return {this->m_element--};
    }

    constexpr iterator& operator+=(difference_type offset) noexcept
    {
      this->m_element += offset;
      return *this;
    }

    constexpr iterator& operator-=(difference_type offset) noexcept
    {
      this->m_element -= offset;
      return *this;
    }

    [[nodiscard]] constexpr iterator operator+(difference_type offset) const noexcept
    {
      return {this->m_element + offset};
    }

    [[nodiscard]] constexpr friend iterator operator+(difference_type offset,
                                                      const iterator& iter) noexcept
    {
      return {offset + iter.m_element};
    }

    [[nodiscard]] constexpr iterator operator-(difference_type offset) const noexcept
    {
      return {this->m_element - offset};
    }

    [[nodiscard]] constexpr difference_type operator-(const const_iterator& other) const noexcept
    {
      return this->m_element - other.m_element;
    }

    [[nodiscard]] constexpr reference operator[](difference_type index) noexcept
    {
      return this->m_element[index];
    }

    [[nodiscard]] constexpr const_reference operator[](difference_type index) const noexcept
    {
      return this->m_element[index];
    }

    [[nodiscard]] constexpr bool operator==(const iterator& other) const noexcept
    {
      return this->m_element == other.m_element;
    }

    [[nodiscard]] constexpr bool operator!=(const iterator& other) const noexcept
    {
      return this->m_element != other.m_element;
    }

    [[nodiscard]] constexpr bool operator<(const iterator& other) const noexcept
    {
      return this->m_element < other.m_element;
    }

    [[nodiscard]] constexpr bool operator<=(const iterator& other) const noexcept
    {
      return this->m_element <= other.m_element;
    }

    [[nodiscard]] constexpr bool operator>(const iterator& other) const noexcept
    {
      return this->m_element > other.m_element;
    }

    [[nodiscard]] constexpr bool operator>=(const iterator& other) const noexcept
    {
      return this->m_element >= other.m_element;
    }

    constexpr iterator& operator=(const iterator&) noexcept = default;
    constexpr iterator& operator=(iterator&&)      noexcept = default;

  };

  class const_iterator
  {
   private:
    friend array_impl;

   public:
    using value_type        = typename array_impl::value_type;
    using difference_type   = typename array_impl::difference_type;
    using reference         = typename array_impl::reference;
    using const_reference   = typename array_impl::const_reference;
    using pointer           = typename array_impl::pointer;
    using const_pointer     = typename array_impl::const_pointer;
    using iterator_category = typename std::contiguous_iterator_tag;

   public:
    pointer m_element;

   public:
    constexpr const_iterator() noexcept
      : m_element{nullptr}
    {
      /* Empty */
    }

    constexpr const_iterator(const_pointer ptr) noexcept
      : m_element{const_cast<pointer>(ptr)}
    {
      /* Empty */
    }

    constexpr const_iterator(const const_iterator&) noexcept = default;
    constexpr const_iterator(const_iterator&&)      noexcept = default;

    [[nodiscard]] constexpr const_reference operator*() const noexcept
    {
      return *this->m_element;
    }

    [[nodiscard]] constexpr const_pointer operator->() const noexcept
    {
      return this->m_element;
    }

    constexpr const_iterator& operator++() noexcept
    {
      ++this->m_element;
      return *this;
    }

    constexpr const_iterator& operator--() noexcept
    {
      --this->m_element;
      return *this;
    }

    constexpr const_iterator operator++(std::int32_t) noexcept
    {
      return {this->m_element++};
    }

    constexpr const_iterator operator--(std::int32_t) noexcept
    {
      return {this->m_element--};
    }

    constexpr const_iterator& operator+=(difference_type offset) noexcept
    {
      this->m_element += offset;
      return *this;
    }

    constexpr const_iterator& operator-=(difference_type offset) noexcept
    {
      this->m_element -= offset;
      return *this;
    }

    [[nodiscard]] constexpr const_iterator operator+(difference_type offset) const noexcept
    {
      return {this->m_element + offset};
    }

    [[nodiscard]] constexpr friend const_iterator operator+(difference_type offset,
                                                            const const_iterator& iter) noexcept
    {
      return {offset + iter.m_element};
    }

    [[nodiscard]] constexpr const_iterator operator-(difference_type offset) const noexcept
    {
      return {this->m_element - offset};
    }

    [[nodiscard]] constexpr difference_type operator-(const const_iterator& other) const noexcept
    {
      return this->m_element - other.m_element;
    }

    [[nodiscard]] constexpr const_reference operator[](difference_type index) const noexcept
    {
      return this->m_element[index];
    }

    [[nodiscard]] constexpr bool operator==(const const_iterator& other) const noexcept
    {
      return this->m_element == other.m_element;
    }

    [[nodiscard]] constexpr bool operator!=(const const_iterator& other) const noexcept
    {
      return this->m_element != other.m_element;
    }

    [[nodiscard]] constexpr bool operator<(const const_iterator& other) const noexcept
    {
      return this-m_element < other.m_element;
    }

    [[nodiscard]] constexpr bool operator<=(const const_iterator& other) const noexcept
    {
      return this->m_element <= other.m_element;
    }

    [[nodiscard]] constexpr bool operator>(const const_iterator& other) const noexcept
    {
      return this->m_element > other.m_element;
    }

    [[nodiscard]] constexpr bool operator>=(const const_iterator& other) const noexcept
    {
      return this->m_element >= other.m_element;
    }

    constexpr const_iterator& operator=(const const_iterator&) noexcept = default;
    constexpr const_iterator& operator=(const_iterator&&)      noexcept = default;

  };

 private:
  alignas(value_type) value_type m_storage[N]{};

 private:
  constexpr void range_check(size_type index) const
  {
    if (index >= N)
    {
      throw std::out_of_range{"array::range_check(size_type) -> index is out of range"};
    }
  }

 public:
  constexpr array_impl() noexcept(std::is_nothrow_default_constructible_v<value_type>)
  {
    /* Empty */
  }

  constexpr array_impl(const array_impl& other) noexcept(std::is_nothrow_default_constructible_v<value_type> &&
                                                         std::is_nothrow_copy_assignable_v<value_type>)
  requires std::copyable<value_type>
  {
    nop::algorithm::copy(other.m_storage,
                         other.m_storage + N,
                         this->m_storage);
  }

  constexpr array_impl(array_impl&& other) noexcept(std::is_nothrow_default_constructible_v<value_type> &&
                                                    std::is_nothrow_move_assignable_v<value_type>)
  requires std::movable<value_type>
  {
    nop::algorithm::move(other.m_storage,
                         other.m_storage + N,
                         this->m_storage);
  }

  template<std::input_iterator InIterator>
  constexpr array_impl(InIterator begin, InIterator end)
  requires std::copyable<value_type>
  {
    if (static_cast<size_type>(std::distance(begin, end)) > N)
    {
      throw std::invalid_argument{"array::array(InIterator, InIterator) -> invalid range size"};
    }

    nop::algorithm::copy(begin,
                         end,
                         this->m_storage);
  }

  constexpr array_impl(size_type n, const value_type& value)
  requires std::copyable<value_type>
  {
    if (n > N)
    {
      throw std::invalid_argument{"array::array(size_type, const value_type&) -> invalid number of elements"};
    }

    std::fill(this->m_storage,
              this->m_storage + n,
              value);
  }

  [[nodiscard]] constexpr iterator begin() noexcept
  {
    return {this->m_storage};
  }

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept
  {
    return {this->m_storage};
  }

  [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
  {
    return std::make_reverse_iterator<iterator>(this->m_storage + N);
  }

  [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
  {
    return std::make_reverse_iterator<const_iterator>(this->m_storage + N);
  }

  [[nodiscard]] constexpr iterator end() noexcept
  {
    return {this->m_storage + N};
  }

  [[nodiscard]] constexpr const_iterator cend() const noexcept
  {
    return {this->m_storage + N};
  }

  [[nodiscard]] constexpr reverse_iterator rend() noexcept
  {
    return std::make_reverse_iterator<iterator>(this->m_storage);
  }

  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
  {
    return std::make_reverse_iterator<const_iterator>(this->m_storage);
  }

  [[nodiscard]] constexpr reference operator[](size_type index) noexcept
  {
    return this->m_storage[index];
  }

  [[nodiscard]] constexpr const_reference operator[](size_type index) const noexcept
  {
    return this->m_storage[index];
  }

  [[nodiscard]] constexpr reference at(size_type index)
  {
    this->range_check(index);

    return this->m_storage[index];
  }

  [[nodiscard]] constexpr const_reference at(size_type index) const
  {
    this->range_check(index);

    return this->m_storage[index];
  }

  [[nodiscard]] constexpr reference front() noexcept
  {
    return this->m_storage[0UL];
  }

  [[nodiscard]] constexpr const_reference front() const noexcept
  {
    return this->m_storage[0UL];
  }

  [[nodiscard]] constexpr reference back() noexcept
  {
    return this->m_storage[N - 1UL];
  }

  [[nodiscard]] constexpr const_reference back() const noexcept
  {
    return this->m_storage[N - 1UL];
  }

  [[nodiscard]] pointer data() noexcept
  {
    return this->m_storage;
  }

  [[nodiscard]] const_pointer data() const noexcept
  {
    return this->m_storage;
  }

  constexpr void fill(const value_type& value) noexcept(std::is_nothrow_copy_assignable_v<value_type>)
  requires std::copyable<value_type>
  {
    std::fill(this->m_storage,
              this->m_storage + N,
              value);
  }

  constexpr void swap(array_impl& other) noexcept((std::is_nothrow_move_constructible_v<value_type> ||
                                                   std::is_nothrow_copy_constructible_v<value_type>) &&
                                                  (std::is_nothrow_move_assignable_v<value_type> ||
                                                   std::is_nothrow_copy_assignable_v<value_type>))
  requires (std::movable<value_type> ||
            std::copyable<value_type>)
  {
    std::swap_ranges(other.m_storage,
                     other.m_storage + N,
                     this->m_storage);
  }

  constexpr array_impl& operator=(const array_impl& other) noexcept(std::is_nothrow_copy_assignable_v<value_type>)
  requires std::copyable<value_type>
  {
    if (this != std::addressof(other))
    {
      nop::algorithm::copy(other.m_storage,
                           other.m_storage + N,
                           this->m_storage);
    }

    return *this;
  }

  constexpr array_impl& operator=(array_impl&& other) noexcept(std::is_nothrow_move_assignable_v<value_type>)
  requires std::movable<value_type>
  {
    if (this != std::addressof(other))
    {
      nop::algorithm::move(other.m_storage,
                           other.m_storage + N,
                           this->m_storage);
    }

    return *this;
  }

  constexpr array_impl& operator=(std::initializer_list<value_type> ilist)
  {
    if (ilist.size() > N)
    {
      throw std::invalid_argument{"array::operator=(std::initializer_list<value_type>) -> invalid size"};
    }

    nop::algorithm::copy(ilist.begin(),
                         ilist.end(),
                         this->m_storage);

    return *this;
  }

};

} /* End namespace container */

} /* End namespace nop_details */

#endif /* End nop_details array_impl header file */
