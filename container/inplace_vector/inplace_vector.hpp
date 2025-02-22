#ifndef NOP_CONTAINER_INPLACE_VECTOR_HPP /* Begin nop::container::inplace_vector header file */
#define NOP_CONTAINER_INPLACE_VECTOR_HPP 1UL

#pragma once

#include <algorithm> /* std::copy, std::move, std::lexicographical_compare_three_way */

#include "inplace_vector_impl.hpp"

namespace nop /* Begin namespace nop */
{

namespace container /* Begin namespace container */
{

template<
         typename T,
         std::size_t N
        >
class inplace_vector : public __nop_details::container::inplace_vector_impl<T, N>
{
 private:
  using base = typename __nop_details::container::inplace_vector_impl<T, N>;

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
  inplace_vector() noexcept
    : base()
  {
    /* Empty */
  }

  explicit inplace_vector(size_type n)
    : base(n)
  {
    /* Empty */
  }

  inplace_vector(size_type n, const value_type& value)
    : base(n, value)
  {
    /* Empty */
  }

  template<typename InIterator>
  inplace_vector(InIterator begin, InIterator end)
    : base(begin, end)
  {
    /* Empty */
  }

  inplace_vector(std::initializer_list<value_type> ilist)
    : base(ilist.begin(), ilist.end())
  {
    /* Empty */
  }

  inplace_vector(const inplace_vector& other) noexcept(noexcept(base(static_cast<const base&>(other))))
    : base(static_cast<const base&>(other))
  {
    /* Empty */
  }

  inplace_vector(inplace_vector&& other) noexcept(noexcept(base(static_cast<base&&>(other))))
    : base(static_cast<base&&>(other))
  {
    /* Empty */
  }

  func operator=(std::initializer_list<value_type> ilist) -> inplace_vector&
  {
    base::assign(ilist.begin(), ilist.end());
    return *this;
  }

  func operator=(const inplace_vector& other) noexcept(noexcept(static_cast<inplace_vector&>(static_cast<base&>(*this) = static_cast<const base&>(other)))) -> inplace_vector&
  {
    return static_cast<inplace_vector&>(static_cast<base&>(*this) = static_cast<const base&>(other));
  }

  func operator=(inplace_vector&& other) noexcept(noexcept(static_cast<inplace_vector&>(static_cast<base&>(*this) = static_cast<base&&>(other)))) -> inplace_vector&
  {
    return static_cast<inplace_vector&>(static_cast<base&>(*this) = static_cast<base&&>(other));
  }

};

} /* End namespace container */

} /* End namespace nop */

namespace std /* Begin namespace std */
{

template<
         typename    T,
         std::size_t N
        >
constexpr func swap(nop::container::inplace_vector<T, N>& lhs,
                    nop::container::inplace_vector<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void
{
  lhs.swap(rhs);
}

} /* End namespace std */

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] constexpr func operator==(const nop::container::inplace_vector<T, N>& lhs,
                                        const nop::container::inplace_vector<T, N>& rhs) noexcept -> bool
{
  return std::equal(lhs.cbegin(),
                    lhs.cend(),
                    rhs.cbegin(),
                    rhs.cend());
}

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] func operator<=>(const nop::container::inplace_vector<T, N>& lhs,
                               const nop::container::inplace_vector<T, N>& rhs) noexcept -> decltype(*lhs.cbegin() <=> *rhs.cbegin())
{
  return std::lexicographical_compare_three_way(lhs.cbegin(),
                                                lhs.cend(),
                                                rhs.cbegin(),
                                                rhs.cend());
}


#endif /* End nop::container::inplace_vector header file */
