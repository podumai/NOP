#ifndef NOP_ALGORITHM_MOVE_BACKWARD_HPP /* Begin algorithm move_backward header file */
#define NOP_ALGORITHM_MOVE_BACKWARD_HPP 1UL

#pragma once

#include <iterator> /* std::bidirectional_iterator<T> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::bidirectional_iterator BidirIterator1,
         std::bidirectional_iterator BidirIterator2
        >
constexpr BidirIterator2 move_backward(BidirIterator1 src_begin,
                                       BidirIterator1 src_end,
                                       BidirIterator2 dst_end) noexcept(noexcept(*--dst_end = std::move(*--src_end)))
{
  [[likely]]
  while (!(src_end == src_begin))
  {
    *--dst_end = std::move(*--src_end);
  }

  return dst_end;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm move_backward header file */
