#ifndef NOP_ALGORITHM_FILL_N_HPP /* Begin algorithm fill_n header file */
#define NOP_ALGORITHM_FILL_N_HPP 1UL

#pragma once

#include <iterator>

namespace nop /* Begin namespace nop */
{

namespace details /* Begin namespace details */
{

template<typename T>
concept valid_fill_n_iter_type = std::output_iterator<T, typename std::iterator_traits<T>::value_type>;

} /* End namespace details */

namespace algorithm /* Begin namespace algorithm */
{

template<
         nop::details::valid_fill_n_iter_type OutIterator,
         std::integral                        Size,
         typename T = typename std::iterator_traits<OutIterator>::value_type
        >
constexpr OutIterator fill_n(OutIterator begin,
                             Size n,
                             const T& value) noexcept(noexcept(*begin++ = value))
{
  [[likely]]
  while (n--)
  {
    *begin++ = value;
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm fill_n header file */
