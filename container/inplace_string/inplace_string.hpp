#ifndef NOP_CONTAINER_INPLACE_STRING_HPP /* Begin nop::container::inplace_string header file */
#define NOP_CONTAINER_INPLACE_STRING_HPP 1UL

#pragma once

#include <initializer_list>
#include "inplace_string_impl.hpp"

namespace nop /* Begin namespace nop */
{

namespace container /* Begin namespace container */
{

template<std::size_t N, typename CharT, typename Traits = std::char_traits<CharT>>
class inplace_string : public __nop_details::container::inplace_string_impl<N, CharT, Traits>
{
 private:
  using base = typename __nop_details::container::inplace_string_impl<N, CharT, Traits>;

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

 public:
  constexpr inplace_string() noexcept
      : base()
  {
    // Empty
  }

  template<typename InIterator>
  constexpr inplace_string(InIterator first, InIterator last)
      : base(first, last)
  {
    // Empty
  }

  constexpr inplace_string(std::initializer_list<value_type> ilist)
      : base(ilist.begin(), ilist.end())
  {
    // Empty
  }
};

} /* End namespace nop */

} /* End namespace container */

#endif /* End nop::container::inplace_string header file */