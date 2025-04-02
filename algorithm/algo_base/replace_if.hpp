#ifndef NOP_ALGORITHM_BASE_REPLACE_IF_HPP /* Begin nop::algorithm::replace_if header file */
#define NOP_ALGORITHM_BASE_REPLACE_IF_HPP 1UL

#pragma once

#include <concepts>    /* std::predicate<F, Args&&...> */
#include <iterator>    /* std::forward_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator,
         typename T = typename std::iterator_traits<FwdIterator>::value_type,
         std::predicate<const typename std::iterator_traits<FwdIterator>::reference> UnaryPredicate
        >
constexpr func replace_if(FwdIterator begin,
                          FwdIterator end,
                          const T& value,
                          UnaryPredicate unary_p) noexcept(noexcept(unary_p(*begin)) &&
                                                           noexcept(*begin = value)) -> FwdIterator
{
  [[likely]]
  while (!(begin == end))
  {
    if (unary_p(*begin))
    {
      *begin = value;
    }

    ++begin;
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::replace_if header file */