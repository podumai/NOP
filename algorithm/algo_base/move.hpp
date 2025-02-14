#ifndef NOP_ALGORITHM_MOVE_HPP /* Begin algorithm move header file */
#define NOP_ALGORITHM_MOVE_HPP 1UL

#pragma once

#include <iterator>

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace nop */
{

template<
         std::input_iterator InIterator,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator
        >
constexpr OutIterator move(InIterator src_begin,
                           InIterator src_end,
                           OutIterator dst_begin) noexcept(noexcept(*dst_begin++ = std::move(*src_begin++)))
{
  [[likely]]
  while (!(src_begin == src_end))
  {
    *dst_begin++ = std::move(*src_begin++);
  }

  return dst_begin;
}

} /* End namespace nop */

} /* End namespace nop */

#endif /* End algorithm move header file */
