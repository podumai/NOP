#ifndef NOP_ALGORITHM_BASE_FIND_IF_HPP /* Begin nop::algorithm::find_if header file */
#define NOP_ALGORITHM_BASE_FIND_IF_HPP 1UL

#pragma once

#include <concepts> /* std::predicate<F, Args&&...> */
#include <iterator> /* std::input_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         std::predicate<const typename std::iterator_traits<InIterator>::reference> UnaryPredicate
        >
[[nodiscard]] constexpr func find_if(InIterator begin,
                                     InIterator end,
                                     UnaryPredicate unary_p) noexcept(noexcept(unary_p(*begin))) -> InIterator
{
  [[likely]]
  while (!(begin == end) && !unary_p(*begin))
  {
    ++begin;
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::find_if header file */