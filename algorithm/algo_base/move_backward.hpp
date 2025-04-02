#ifndef NOP_ALGORITHM_BASE_MOVE_BACKWARD_HPP /* Begin nop::algorithm::move_backward header file */
#define NOP_ALGORITHM_BASE_MOVE_BACKWARD_HPP 1UL

#pragma once

#include <iterator> /* std::bidirectional_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::bidirectional_iterator BidirIterator1,
         std::bidirectional_iterator BidirIterator2
        >
constexpr func move_backward(BidirIterator1 src_begin,
                             BidirIterator1 src_end,
                             BidirIterator2 dst_end) noexcept(noexcept(*--dst_end = std::move(*--src_end))) -> BidirIterator2
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

#endif /* End nop::algorithm::move_backward header file */