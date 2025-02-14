#ifndef NOP_ALGORITHM_SEARCH_HPP /* Begin algorithm search header file */
#define NOP_ALGORITHM_SEARCH_HPP 1UL

#pragma once

#include <iterator> /* std::forward_iterator<T> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator1,
         std::forward_iterator FwdIterator2
        >
[[nodiscard]] FwdIterator1 search(FwdIterator1 begin1,
                                  FwdIterator1 end1,
                                  FwdIterator2 s_begin2,
                                  FwdIterator2 s_end2) noexcept
{
  [[likely]]
  while (true)
  {
    auto temp_s_begin(s_begin2);
    auto temp_begin(begin1);

    while (!(temp_begin == end1) && !(temp_s_begin == s_end2) && *temp_s_begin++ == *temp_begin++)
    {
      /* Empty */
    }

    if (temp_s_begin == s_end2 || temp_begin == end1)
    {
      return begin1;
    }

    ++begin1;
  }
}

template<
         std::forward_iterator FwdIterator1,
         std::forward_iterator FwdIterator2,
         std::predicate<
                        const typename std::iterator_traits<FwdIterator1>::reference,
                        const typename std::iterator_traits<FwdIterator2>::reference
                       > BinaryPredicate
        >
[[nodiscard]] FwdIterator1 search(FwdIterator1 begin1,
                                  FwdIterator1 end1,
                                  FwdIterator2 s_begin2,
                                  FwdIterator2 s_end2,
                                  BinaryPredicate binary_p) noexcept(noexcept(binary_p(*begin1++, *s_begin2++)))
{
  [[likely]]
  while (true)
  {
    auto temp_s_begin(s_begin2);
    auto temp_begin(begin1);

    while (!(temp_begin == end1) && !(temp_s_begin == s_end2) && binary_p(*temp_s_begin++, *temp_begin++))
    {
      /* Empty */
    }

    if (temp_s_begin == s_end2 || temp_begin == end1)
    {
      return begin1;
    }

    ++begin1;
  }
}


} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm search header file */
