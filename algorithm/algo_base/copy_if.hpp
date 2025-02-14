#ifndef NOP_ALGORITHM_COPY_IF_HPP /* Begin algorithm copy_if header file */
#define NOP_ALGORITHM_COPY_IF_HPP 1UL

#pragma once

#include <concepts> /* std::predicate<F, Args&&...> */
#include <iterator> /* std::input_iterator<T>, std::output_iterator<T, U> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator                                                         InIterator,
         std::output_iterator<typename std::iterator_traits<InIterator>::value_type> OutIterator,
         std::predicate<const typename std::iterator_traits<InIterator>::reference>  UnaryPredicate
        >
constexpr OutIterator copy_if(InIterator     src_begin,
                              InIterator     src_end,
                              OutIterator    dst_begin,
                              UnaryPredicate unary_p) noexcept(noexcept(unary_p(*src_begin)) &&
                                                               noexcept(*dst_begin++ = *src_begin))
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

#endif /* End algorithm copy_if header file */
