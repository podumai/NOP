#ifndef NOP_ALGORITHM_BASE_COPY_IF_HPP /* Begin nop::algorithm::copy_if header file */
#define NOP_ALGORITHM_BASECOPY_IF_HPP 1UL

#pragma once

#include <concepts> /* std::predicate<F, Args&&...> */
#include <iterator> /* std::input_iterator<T>, std::output_iterator<T, U> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator,
         std::predicate<const typename std::iterator_traits<InIterator>::reference> UnaryPredicate
        >
constexpr func copy_if(InIterator src_begin,
                       InIterator src_end,
                       OutIterator dst_begin,
                       UnaryPredicate unary_p) noexcept(noexcept(unary_p(*src_begin)) &&
                                                        noexcept(*dst_begin++ = *src_begin)) -> OutIterator
{
  [[likely]]
  while (!(src_begin == src_end))
  {
    if (unary_p(*src_begin))
    {
      *dst_begin++ = *src_begin;
    }

    ++src_begin;
  }

  return dst_begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::copy_if header file */