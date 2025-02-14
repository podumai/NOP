#ifndef NOP_ALGORITHM_FIND_IF_NOT_HPP /* Begin algrotihm find_if_not header file */
#define NOP_ALGORITHM_FIND_IF_NOT_HPP 1UL

#pragma once

#include <concepts> /* std::predicate<F, Args&&...> */
#include <iterator> /* std::input_iterator<T> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator                                                        InIterator,
         std::predicate<const typename std::iterator_traits<InIterator>::reference> UnaryPredicate
        >
[[nodiscard]] constexpr InIterator find_if_not(InIterator     begin,
                                               InIterator     end,
                                               UnaryPredicate unary_p) noexcept(noexcept(unary_p(*begin++)))
{
  [[likely]]
  while (!(begin == end) && unary_p(*begin++))
  {
    /* Empty */
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm find_if_not header file */
