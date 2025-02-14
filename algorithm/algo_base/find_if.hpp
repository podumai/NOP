#ifndef NOP_ALGORITHM_FIND_IF_HPP /* Begin algorithm find_if header file */
#define NOP_ALGORITHM_FIND_IF_HPP 1UL

#pragma once

#include <concepts>
#include <iterator>

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator                                                        InIterator,
         std::predicate<const typename std::iterator_traits<InIterator>::reference> UnaryPredicate
        >
[[nodiscard]] constexpr InIterator find_if(InIterator     begin,
                                           InIterator     end,
                                           UnaryPredicate unary_p) noexcept(noexcept(unary_p(*begin++)))
{
  [[likely]]
  while (!(begin == end) && !unary_p(*begin++))
  {
    /* Empty */
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm find_if header file */
