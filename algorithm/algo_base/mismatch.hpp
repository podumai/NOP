#ifndef NOP_ALGORITHM_MISMATCH_HPP /* Begin algorithm mismatch header file */
#define NOP_ALGORITHM_MISMATCH_HPP 1UL

#pragma once

#include <concepts> /* std::predicate<F, Args&&...> */
#include <iterator> /* std::input_iterator<T> */
#include <utility>  /* std::pair<T, U> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2
        >
[[nodiscard]] constexpr std::pair<InIterator1, InIterator2> mismatch(InIterator1 begin1,
                                                                     InIterator1 end1,
                                                                     InIterator2 begin2) noexcept
{
  [[likely]]
  while (!(begin1 == end1) && *begin1++ == *begin2++)
  {
    /* Empty */
  }

  return {begin1, begin2};
}

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2
        >
[[nodiscard]] constexpr std::pair<InIterator1, InIterator2> mismatch(InIterator1 begin1,
                                                                     InIterator1 end1,
                                                                     InIterator2 begin2,
                                                                     InIterator2 end2) noexcept
{
  [[likely]]
  while (!(begin1 == end1) && !(begin2 == end2) && *begin1++ == *begin2++)
  {
    /* Empty */
  }

  return {begin1, begin2};
}

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2,
         std::predicate<
                        const typename std::iterator_traits<InIterator1>::reference,
                        const typename std::iterator_traits<InIterator2>::reference
                       > BinaryPredicate
        >
[[nodiscard]] constexpr std::pair<InIterator1, InIterator2> mismatch(InIterator1     begin1,
                                                                     InIterator1     end1,
                                                                     InIterator2     begin2,
                                                                     BinaryPredicate binary_p) noexcept(noexcept(binary_p(*begin1++, *begin2++)))
{
  [[likely]]
  while (!(begin1 == end1) && binary_p(*begin1++, *begin2++))
  {
    /* Empty */
  }

  return {begin1, begin2};
}

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2,
         std::predicate<
                        const typename std::iterator_traits<InIterator1>::reference,
                        const typename std::iterator_traits<InIterator2>::reference
                       > BinaryPredicate
        >
[[nodiscard]] constexpr std::pair<InIterator1, InIterator2> mismatch(InIterator1     begin1,
                                                                     InIterator1     end1,
                                                                     InIterator2     begin2,
                                                                     InIterator2     end2,
                                                                     BinaryPredicate binary_p) noexcept(noexcept(binary_p(*begin1++, *begin2++)))
{
  [[likely]]
  while (!(begin1 == end1) && !(begin2 == end2) && binary_p(*begin1++, *begin2++))
  {
    /* Empty */
  }

  return {begin1, begin2};
}

} /* End namespace algorithm */

} /* End namespace nop */


#endif /* End algorithm mismatch header file */
