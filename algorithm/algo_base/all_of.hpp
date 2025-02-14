#ifndef NOP_ALGORITHM_ALL_OF_HPP /* Begin algorithm all_of header file */
#define NOP_ALGORITHM_ALL_OF_HPP 1UL

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
[[nodiscard]] constexpr bool all_of(InIterator     begin,
                                    InIterator     end,
                                    UnaryPredicate unary_p) noexcept(noexcept(unary_p(*begin++)))
{
  [[likely]]
  while (!(begin == end) && unary_p(*begin++))
  {
    /* Empty */
  }

  return begin == end;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm all_of header file */
