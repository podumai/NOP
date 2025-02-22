#ifndef NOP_DETAILS_ARRAY_IMPL_HPP /* Begin __nop_details array_impl header file */
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

namespace __nop_details /* Begin namespace __nop_details */
{

template<typename T>
concept valid_array_t = std::default_initializable<T> &&
                        std::destructible<T>;

namespace container /* Begin namespace container */
{

template<
         __nop_details::valid_array_t T,
         std::size_t                  N
        >
class array_impl : public __nop_details::container::array_base<T, N>
{
 private:
  using base = typename __nop_details::container::array_base<T, N>;

 public:
  using value_type             = typename base::value_type;
  using size_type              = typename base::size_type;
  using difference_type        = typename base::difference_type;
  using reference              = typename base::reference;
  using const_reference        = typename base::const_reference;
  using pointer                = typename base::pointer;
  using const_pointer          = typename base::const_pointer;
  using iterator               = typename base::iterator;
  using const_iterator         = typename base::const_iterator;
  using reverse_iterator       = typename base::reverse_iterator;
  using const_reverse_iterator = typename base::const_reverse_iterator;

 private:
  alignas(value_type) value_type m_storage[N]{};

 private:
  constexpr func range_check(size_type index) const -> void
  {
    if (index >= N)
    {
      throw std::out_of_range{"array::range_check(size_type) -> index is out of range"};
    }
  }

 public:
  constexpr array_impl() noexcept(std::is_nothrow_default_constructible_v<value_type>) = default;

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
      throw __nop_details::container::array_overflow{};
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
      throw __nop_details::container::array_overflow{};
    }

    std::fill(this->m_storage,
              this->m_storage + n,
              value);
  }

  template<size_type Size>
  constexpr array_impl(value_type (&&arr)[Size])
  {
    if (Size > N)
    {
      throw __nop_details::container::array_overflow{};
    }

    nop::algorithm::move(arr,
                         arr + Size,
                         this->m_storage);
  }

  [[nodiscard]] constexpr func begin() noexcept -> iterator
  {
    return this->m_storage;
  }

  [[nodiscard]] constexpr func cbegin() const noexcept -> const_iterator
  {
    return this->m_storage;
  }

  [[nodiscard]] constexpr func rbegin() noexcept -> reverse_iterator
  {
    return std::make_reverse_iterator<iterator>(this->m_storage + N);
  }

  [[nodiscard]] constexpr func crbegin() const noexcept -> const_reverse_iterator
  {
    return std::make_reverse_iterator<const_iterator>(this->m_storage + N);
  }

  [[nodiscard]] constexpr func end() noexcept -> iterator
  {
    return this->m_storage + N;
  }

  [[nodiscard]] constexpr func cend() const noexcept -> const_iterator
  {
    return this->m_storage + N;
  }

  [[nodiscard]] constexpr func rend() noexcept -> reverse_iterator
  {
    return std::make_reverse_iterator<iterator>(this->m_storage);
  }

  [[nodiscard]] constexpr func crend() const noexcept -> const_reverse_iterator
  {
    return std::make_reverse_iterator<const_iterator>(this->m_storage);
  }

  [[nodiscard]] constexpr func operator[](size_type index) noexcept -> reference
  {
    return this->m_storage[index];
  }

  [[nodiscard]] constexpr func operator[](size_type index) const noexcept -> const_reference
  {
    return this->m_storage[index];
  }

  [[nodiscard]] constexpr func at(size_type index) -> reference
  {
    this->range_check(index);

    return this->m_storage[index];
  }

  [[nodiscard]] constexpr func at(size_type index) const -> const_reference
  {
    this->range_check(index);

    return this->m_storage[index];
  }

  [[nodiscard]] constexpr func front() noexcept -> reference
  {
    return *this->m_storage;
  }

  [[nodiscard]] constexpr func front() const noexcept -> const_reference
  {
    return *this->m_storage;
  }

  [[nodiscard]] constexpr func back() noexcept -> reference
  {
    return this->m_storage[N - 1UL];
  }

  [[nodiscard]] constexpr func back() const noexcept -> const_reference
  {
    return this->m_storage[N - 1UL];
  }

  [[nodiscard]] constexpr func data() noexcept -> pointer
  {
    return this->m_storage;
  }

  [[nodiscard]] constexpr func data() const noexcept -> const_pointer
  {
    return this->m_storage;
  }

  constexpr func fill(const value_type& value) noexcept(std::is_nothrow_copy_assignable_v<value_type>) -> void
  requires std::copyable<value_type>
  {
    std::fill(this->m_storage,
              this->m_storage + N,
              value);
  }

  constexpr func swap(array_impl& other) noexcept((std::is_nothrow_move_constructible_v<value_type> ||
                                                   std::is_nothrow_copy_constructible_v<value_type>) &&
                                                  (std::is_nothrow_move_assignable_v<value_type> ||
                                                   std::is_nothrow_copy_assignable_v<value_type>)) -> void
  requires (std::movable<value_type> ||
            std::copyable<value_type>)
  {
    std::swap_ranges(other.m_storage,
                     other.m_storage + N,
                     this->m_storage);
  }

  constexpr func operator=(const array_impl& other) noexcept(std::is_nothrow_copy_assignable_v<value_type>) -> array_impl&
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

  constexpr func operator=(array_impl&& other) noexcept(std::is_nothrow_move_assignable_v<value_type>) -> array_impl&
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

};

} /* End namespace container */

} /* End namespace __nop_details */

#endif /* End __nop_details array_impl header file */
