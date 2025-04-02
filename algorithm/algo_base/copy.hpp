#ifndef NOP_ALGORITHM_BASE_COPY_HPP /* Begin nop::algorithm::copy header file */
#define NOP_ALGORITHM_BASE_COPY_HPP 1UL

#pragma once

#include <iterator> /* std::input_iterator<T>, std::output_iterator<T, U> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator
        >
constexpr func copy(InIterator src_begin,
                    InIterator src_end,
                    OutIterator dst_begin) noexcept(noexcept(*dst_begin++ = *src_begin++)) -> OutIterator
{
  [[likely]]
  while (!(src_begin == src_end))
  {
    *dst_begin++ = *src_begin++;
  }

  return dst_begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::copy header file */