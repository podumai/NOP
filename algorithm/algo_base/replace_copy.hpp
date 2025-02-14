#ifndef NOP_ALGORITHM_REPLACE_COPY_HPP /* Begin algorithm replace_copy header file */
#define NOP_ALGORITHM_REPLACE_COPY_HPP 1UL

#pragma once

#include <iterator> /* std::input_iterator<T>, std::output_iterator<T> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator                                                         InIterator,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator,
         typename T = typename std::iterator_traits<InIterator>::value_type
        >
constexpr OutIterator replace_copy(InIterator  src_begin,
                                   InIterator  src_end,
                                   OutIterator dst_begin,
                                   const T&    old_value,
                                   const T&    new_value) noexcept(noexcept(*dst_begin = new_value) &&
                                                                   noexcept(*dst_begin = *src_begin))
{
  [[likely]]
  while (!(src_begin == src_end))
  {
    if (*src_begin == old_value)
    {
      *dst_begin = new_value;
    }
    else
    {
      *dst_begin = *src_begin;
    }

    ++src_begin;
    ++dst_begin;
  }

  return dst_begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm replace_copy header file */
