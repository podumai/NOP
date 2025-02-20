#ifndef NOP_CONTAINER_ARRAY_HPP /* Begin container array header file */
#define NOP_CONTAINER_ARRAY_HPP 1UL

#pragma once

#include "array_impl.hpp"

namespace nop /* Begin namespace nop */
{

namespace container /* Begin namespace container */
{

template<
         typename    T,
         std::size_t N
        >
class array : public nop_details::container::array_impl<T, N>
{
 private:
  using base = typename nop_details::container::array_impl<T, N>;

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
  using reverse_iterator       = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

 public:
  constexpr array() noexcept(noexcept(base()))
    : base()
  {
    /* Empty */
  }

  constexpr array(const array& other) noexcept(noexcept(base(static_cast<const base&>(other))))
    : base(static_cast<const base&>(other))
  {
    /* Empty */
  }

  constexpr array(array&& other) noexcept(noexcept(base(static_cast<base&&>(other))))
    : base(static_cast<base&&>(other))
  {
    /* Empty */
  }

  constexpr array(size_type n, const value_type& value)
    : base(n, value)
  {
    /* Empty */
  }

  template<typename InIterator>
  constexpr array(InIterator begin, InIterator end)
    : base(begin, end)
  {
    /* Empty */
  }

  constexpr array(std::initializer_list<value_type> ilist)
    : base(ilist.begin(), ilist.end())
  {
    /* Empty */
  }

  template<size_type Size>
  constexpr array(const value_type (&arr)[Size])
    : base(arr, arr + Size)
  {
    /* Empty */
  }

  [[nodiscard]] iterator begin() noexcept
  {
    return base::begin();
  }

  [[nodiscard]] const_iterator cbegin() const noexcept
  {
    return base::cbegin();
  }

  [[nodiscard]] reverse_iterator rbegin() noexcept
  {
    return base::rbegin();
  }

  [[nodiscard]] const_reverse_iterator crbegin() const noexcept
  {
    return base::crbegin();
  }

  [[nodiscard]] iterator end() noexcept
  {
    return base::end();
  }

  [[nodiscard]] const_iterator cend() const noexcept
  {
    return base::cend();
  }

  [[nodiscard]] reverse_iterator rend() noexcept
  {
    return base::rend();
  }

  [[nodiscard]] const_reverse_iterator crend() const noexcept
  {
    return base::crend();
  }

  [[nodiscard]] constexpr reference operator[](size_type index) noexcept
  {
    return base::operator[](index);
  }

  [[nodiscard]] constexpr const_reference operator[](size_type index) const noexcept
  {
    return base::operator[](index);
  }

  [[nodiscard]] constexpr reference at(size_type index) noexcept
  {
    return base::at(index);
  }

  [[nodiscard]] constexpr const_reference at(size_type index) const noexcept
  {
    return base::at(index);
  }

  [[nodiscard]] constexpr reference front() noexcept
  {
    return base::front();
  }

  [[nodiscard]] constexpr const_reference front() const noexcept
  {
    return base::front();
  }

  [[nodiscard]] constexpr reference back() noexcept
  {
    return base::back();
  }

  [[nodiscard]] constexpr const_reference back() const noexcept
  {
    return base::back();
  }

  [[nodiscard]] constexpr pointer data() noexcept
  {
    return base::data();
  }

  [[nodiscard]] constexpr const_pointer data() const noexcept
  {
    return base::data();
  }

  [[nodiscard]] constexpr size_type size() const noexcept
  {
    return base::size();
  }

  [[nodiscard]] constexpr bool empty() const noexcept
  {
    return base::empty();
  }

  [[nodiscard]] constexpr size_type max_size() const noexcept
  {
    return base::max_size();
  }

  constexpr void swap(array& other) noexcept(noexcept(base::swap(static_cast<base&>(other))))
  {
    base::swap(static_cast<base&>(other));
  }

  constexpr void fill(const value_type& value) noexcept(noexcept(base::fill(value)))
  {
    base::fill(value);
  }

  constexpr array& operator=(const array& other) noexcept(noexcept(static_cast<array&>(static_cast<base&>(*this) = static_cast<const base&>(other))))
  {
    return static_cast<array&>(static_cast<base&>(*this) = static_cast<const base&>(other));
  }

  constexpr array& operator=(array&& other) noexcept(noexcept(static_cast<array&>(static_cast<base&>(*this) = static_cast<base&&>(other))))
  {
    return static_cast<array&>(static_cast<base&>(*this) = static_cast<base&&>(other));
  }

  constexpr array& operator=(std::initializer_list<value_type> ilist)
  {
    return static_cast<array&>(static_cast<base&>(*this) = ilist);
  }

};

} /* End namespace container */

} /* End namespace nop */

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] constexpr bool operator==(const nop::container::array<T, N>& lhs,
                                        const nop::container::array<T, N>& rhs) noexcept
{
  return std::equal(lhs.cbegin(),
                    lhs.cend(),
                    rhs.cbegin());
}

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] constexpr auto operator<=>(const nop::container::array<T, N>& lhs,
                                         const nop::container::array<T, N>& rhs) noexcept
{
  return std::lexicographical_compare_three_way(lhs.cbegin(),
                                                lhs.cend(),
                                                rhs.cbegin(),
                                                rhs.cend());
}

namespace std /* Begin namespace std */
{

template<
         typename    T,
         std::size_t N
        >
constexpr void swap(nop::container::array<T, N>& lhs,
                    nop::container::array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}

} /* End namespace std */

#endif /* End container array header file */
