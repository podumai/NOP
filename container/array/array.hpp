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
class array : public __nop_details::container::array_impl<T, N>
{
 private:
  using base = typename __nop_details::container::array_impl<T, N>;

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

  template<size_type Size>
  constexpr array(value_type (&&arr)[Size])
    : base(std::move(arr))
  {
    /* Empty */
  }

  constexpr func operator=(const array& other) noexcept(noexcept(static_cast<array&>(static_cast<base&>(*this) = static_cast<const base&>(other)))) -> array&
  {
    return static_cast<array&>(static_cast<base&>(*this) = static_cast<const base&>(other));
  }

  constexpr func operator=(array&& other) noexcept(noexcept(static_cast<array&>(static_cast<base&>(*this) = static_cast<base&&>(other)))) -> array&
  {
    return static_cast<array&>(static_cast<base&>(*this) = static_cast<base&&>(other));
  }

};

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] constexpr func to_array(T (&arr)[N]) noexcept(std::is_nothrow_copy_assignable_v<T>) -> nop::container::array<std::remove_cv_t<T>, N>
{
  return {arr};
}

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] constexpr func to_array(T (&&arr)[N]) noexcept(std::is_nothrow_move_assignable_v<T>) -> nop::container::array<std::remove_cv_t<T>, N>
{
  return {std::move(arr)};
}

} /* End namespace container */

} /* End namespace nop */

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] constexpr func operator==(const nop::container::array<T, N>& lhs,
                                        const nop::container::array<T, N>& rhs) noexcept -> bool
{
  return std::equal(lhs.cbegin(),
                    lhs.cend(),
                    rhs.cbegin());
}

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] constexpr func operator<=>(const nop::container::array<T, N>& lhs,
                                         const nop::container::array<T, N>& rhs) noexcept -> decltype(*lhs.cbegin() <=> *rhs.cbegin())
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
constexpr func swap(nop::container::array<T, N>& lhs,
                    nop::container::array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void
{
  lhs.swap(rhs);
}

} /* End namespace std */

#endif /* End container array header file */
