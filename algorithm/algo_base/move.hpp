#ifndef NOP_ALGORITHM_BASE_MOVE_HPP /* Begin nop::algorithm::move header file */
#define NOP_ALGORITHM_BASE_MOVE_HPP 1UL

#pragma once

#include <iterator> /* std::input_iterator<T>, std::output_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace nop */
{

template<
         std::input_iterator InIterator,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator
        >
constexpr func move(InIterator src_begin,
                    InIterator src_end,
                    OutIterator dst_begin) noexcept(noexcept(*dst_begin++ = std::move(*src_begin++))) -> OutIterator
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

#endif /* End nop::algorithm::move header file */