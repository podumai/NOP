#ifndef NOP_DETAILS_CONTAINER_INPLACE_STRING_IMPL_HPP /* Begin __nop_details::container::inplace_string_impl header file */
#define NOP_DETAILS_CONTAINER_INPLACE_STRING_IMPL_HPP 1UL

#pragma once


#ifndef NOP_CONTAINER_INPLACE_STRING_HPP
  #error "This header file can not be included explicitly. Use inplace_string.hpp instead."
#endif

#include "inplace_string_base.hpp"
#include <string> /* std::char_traits<T> */
#include <stdexcept> /* std::out_of_range */
#include <new> /* std::bad_alloc */
#include <type_traits>
#include "base/func_keyword.hpp"
#include "base/nil_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace container /* Begin namespace container */
{

template<std::size_t N, typename CharT, typename Traits>
class inplace_string_impl : public __nop_details::container::inplace_string_base<CharT, Traits>
{
 private:
  using base = typename __nop_details::container::inplace_string_base<CharT, Traits>;

 public:
  using traits_type = typename base::traits_type;
  using value_type = typename base::value_type;
  using size_type = typename base::size_type;
  using difference_type = typename base::difference_type;
  using reference = typename base::reference;
  using const_reference = typename base::const_reference;
  using pointer = typename base::pointer;
  using const_pointer = typename base::const_pointer;
  using iterator = typename base::iterator;
  using const_iterator = typename base::const_iterator;
  using reverse_iterator = typename base::reverse_iterator;
  using const_reverse_iterator = typename base::const_reverse_iterator;

 private:
  alignas(value_type) value_type m_storage[N];
  size_type m_size;

 public:
  constexpr inplace_string_impl() noexcept
      : m_size{}
  {
    std::fill_n(std::begin(m_storage), N, traits_type::eof());
  }

  template<std::input_iterator InIter>
  constexpr inplace_string_impl(InIter first, InIter last)
  {
    size_type dist{static_cast<size_type>(std::distance(first, last))};
    if (dist >= N)
    {
      throw std::bad_alloc{};
    }
    m_size = dist;
    std::copy(first, last, m_storage);
  }

  [[nodiscard]] constexpr func begin() noexcept -> iterator
  {
    return m_storage;
  }

  [[nodiscard]] constexpr func cbegin() const noexcept -> const_iterator
  {
    return begin();
  }

  [[nodiscard]] constexpr func rbegin() noexcept -> reverse_iterator
  {
    return {end()};
  }

  [[nodiscard]] constexpr func crbegin() const noexcept -> const_reverse_iterator
  {
    return {cend()};
  }

  [[nodiscard]] constexpr func end() noexcept -> iterator
  {
    return m_storage + m_size;
  }

  [[nodiscard]] constexpr func cend() const noexcept -> const_iterator
  {
    return end();
  }

  [[nodiscard]] constexpr func rend() noexcept -> reverse_iterator
  {
    return {begin()};
  }

  [[nodiscard]] constexpr func crend() const noexcept -> const_reverse_iterator
  {
    return {cbegin()};
  }

  [[nodiscard]] constexpr func operator[](size_type index) -> reference
  {
    return m_storage[index];
  }

  [[nodiscard]] constexpr func operator[](size_type index) const -> const_reference
  {
    return this->operator[](index);
  }

  [[nodiscard]] constexpr func front() noexcept -> reference
  {
    return *m_storage;
  }

  [[nodiscard]] constexpr func front() const noexcept -> const_reference
  {
    return *m_storage;
  }

  [[nodiscard]] constexpr func back() noexcept -> reference
  {
    return m_storage[m_size - 1UL];
  }

  [[nodiscard]] constexpr func back() const noexcept -> const_reference
  {
    return m_storage[m_size - 1UL];
  }

 private:
  func check_range(size_type index) const -> void
  {
    if (index >= N)
    {
      throw std::out_of_range{"nop::container::inplace_string::check_range(index) -> index is out of range"};
    }
  }

 public:
  [[nodiscard]] constexpr func at(size_type index) -> reference
  {
    check_range(index);
    return m_storage[index];
  }

  [[nodiscard]] constexpr func at(size_type index) const -> const_reference
  {
    return at(index);
  }

  [[nodiscard]] constexpr func size() const noexcept -> size_type
  {
    return m_size;
  }

  [[nodiscard]] constexpr func length() const noexcept -> size_type
  {
    return size();
  }

  [[nodiscard]] constexpr static func max_size() noexcept -> size_type
  {
    return N;
  }

  [[nodiscard]] constexpr static func capacity() noexcept -> size_type
  {
    return max_size();
  }

 private:
  constexpr func construct(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>) -> pointer
  {
    return ::new (m_storage + m_size++) value_type(value);
  }

  constexpr func construct(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>) -> pointer
  {
    return ::new (m_storage + m_size++) value_type(std::move(value));
  }

 public:
  constexpr func push_back(const value_type& value) -> void
  {
    if (m_size >= N)
    {
      throw std::bad_alloc{};
    }
    (void)construct(value);
  }

  constexpr func push_back(value_type&& value) -> void
  {
    if (m_size >= N)
    {
      throw std::bad_alloc{};
    }
    (void)construct(std::move(value));
  }

  constexpr func try_push_back(const value_type& value) noexcept(noexcept(construct(value))) -> pointer
  {
    if (m_size >= N)
    {
      return nil;
    }
    return construct(value);
  }

  constexpr func try_push_back(value_type&& value) noexcept(noexcept(construct(std::move(value)))) -> pointer
  {
    if (m_size >= N)
    {
      return nil;
    }
    return construct(std::move(value));
  }

  constexpr func unchecked_push_back(const value_type& value) noexcept(noexcept(construct(value))) -> reference
  {
    return *construct(value);
  }

  constexpr func unchecked_push_back(value_type&& value) noexcept(noexcept(construct(std::move(value)))) -> reference
  {
    return *construct(std::move(value));
  }

  constexpr func operator+=(const_pointer str) -> inplace_string_impl&
  {
    size_type str_length{traits_type::length(str)};
    if (str_length + m_size >= N)
    {
      throw std::bad_alloc{};
    }
    while (str_length-- > 0UL)
    {
      unchecked_push_back(*str++);
    }
    return *this;
  }

  constexpr func operator+=(const inplace_string_impl& str) noexcept(noexcept(construct(*str))) -> inplace_string_impl
  {
    if (str.size() + m_size >= N)
    {
      throw std::bad_alloc{};
    }
    for (const auto& symbol : str)
    {
      unchecked_push_back(symbol);
    }
    return *this;
  }
};

} /* End namespace container */

} /* End namespace __nop_details */

#endif /* End __nop_details::container::inplace_string_impl header file */