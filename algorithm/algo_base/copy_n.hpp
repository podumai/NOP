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
         std::input_iterator InIterator,
         std::integral Size,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator
        >
constexpr OutIterator copy_n(InIterator src, Size n, OutIterator dst) noexcept(noexcept(*dst++ = *src++))
{
  [[likely]]
  while (n--)
  {
    *dst++ = *src++;
  }

  return dst;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm copy_n header file */
