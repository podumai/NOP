#ifndef NOP_ALGORITHM_BASE_TRANSFORM_HPP /* Begin nop::algorithm::transform header file */
#define NOP_ALGORITHM_BASE_TRANSFORM_HPP 1UL

#pragma once

#include <concepts> /* std::invocable<F, Args&&...> */
#include <iterator> /* std::input_iterator<T>, std::output_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator,
         std::invocable<const typename std::iterator_traits<InIterator>::reference> UnaryOp
        >
constexpr func transform(InIterator src_begin,
                         InIterator src_end,
                         OutIterator dst_begin,
                         UnaryOp unary_op) noexcept(noexcept(*dst_begin++ = unary_op(*src_begin++))) -> OutIterator
{
  [[likely]]
  while (!(src_begin == src_end))
  {
    *dst_begin++ = unary_op(*src_begin++);
  }

  return dst_begin;
}

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2,
         std::output_iterator<typename std::iterator_traits<InIterator1>::value_type> OutIterator,
         std::invocable<
                        const typename std::iterator_traits<InIterator1>::reference,
                        const typename std::iterator_traits<InIterator2>::reference
                       > BinaryOp
        >
constexpr func transform(InIterator1 src1_begin,
                         InIterator1 src1_end,
                         InIterator2 src2_begin,
                         InIterator2 src2_end,
                         OutIterator dst_begin,
                         BinaryOp binary_op) noexcept(noexcept(*dst_begin++ = binary_op(*src1_begin++, *src2_begin++))) -> OutIterator
{
  [[likely]]
  while (!(src1_begin == src1_end) && !(src2_begin == src2_end))
  {
    *dst_begin++ = binary_op(*src1_begin++, *src2_begin++);
  }

  return dst_begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::transform header file */