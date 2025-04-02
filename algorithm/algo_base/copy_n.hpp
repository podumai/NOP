#ifndef NOP_ALGORITHM_BASE_COPY_N_HPP /* Begin nop::algorithm::copy_n header file */
#define NOP_ALGORITHM_BASE_COPY_N_HPP 1UL

#pragma once

#include <concepts> /* std::integral<T> */
#include <iterator> /* std::input_iterator<T>, std::output_iterator<T>  */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         std::integral Size,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator
        >
constexpr func copy_n(InIterator src_begin,
                      Size n,
                      OutIterator dst_begin) noexcept(noexcept(*dst_begin++ = *src_begin++)) -> OutIterator
{
  [[likely]]
  while (n--)
  {
    *dst_begin++ = *src_begin++;
  }

  return dst_begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::copy_n header file */