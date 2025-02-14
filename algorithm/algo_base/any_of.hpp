#ifndef NOP_ALGORITHM_ANY_OF_HPP /* Begin algorithm any_of header file */
#define NOP_ALGORITHM_ANY_OF_HPP 1UL

#pragma once

#include <concepts> /* std::predicate<F, Args&&...> */
#include <iterator> /* std::input_iterator<T> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator                                                         InIterator,
         std::predicate<const typename std::iterator_traits<InIterator>::value_type> UnaryPredicate
        >
[[nodiscard]] constexpr bool any_of(InIterator     begin,
                                    InIterator     end,
                                    UnaryPredicate unary_p) noexcept(noexcept(unary_p(*begin++)))
{
  [[likely]]
  while (!(begin == end))
  {
    if (unary_p(*begin++))
    {
      return true;
    }
  }

  return false;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm any_of header file */
