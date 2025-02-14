#ifndef NOP_ALGORITHM_COPY_N_HPP /* Begin algorithm copy_n header file */
#define NOP_ALGORITHM_COPY_N_HPP 1UL

#pragma once

#include <concepts>
#include <iterator>

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator                                                         InIterator,
         std::integral                                                               Size,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator
        >
constexpr OutIterator copy_n(InIterator  src_begin,
                             Size        n,
                             OutIterator dst_begin) noexcept(noexcept(*dst_begin++ = *src_begin++))
{
  [[likely]]
  while (n--)
  {
    *dst_begin++ = *src_begin++;
  }

  return dst;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm copy_n header file */
