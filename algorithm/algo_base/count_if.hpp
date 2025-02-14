#ifndef NOP_ALGORITHM_COUNT_IF_HPP /* Begin algorithm count_if header file */
#define NOP_ALGORITHM_COUNT_IF_HPP 1UL

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
[[nodiscard]] constexpr typename std::iterator_traits<InIterator>::difference_type count_if(InIterator     begin,
                                                                                            InIterator     end,
                                                                                            UnaryPredicate unary_p) noexcept(noexcept(unary_p(*begin++)))
{
  typename std::iterator_traits<InIterator>::difference_type c{};

  [[likely]]
  while (!(begin == end))
  {
    if (unary_p(*begin++))
    {
      ++c;
    }
  }

  return c;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm count_if header file */
