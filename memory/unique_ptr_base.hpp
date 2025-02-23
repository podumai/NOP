#ifndef NOP_DETAILS_MEMORY_UNIQUE_PTR_BASE_HPP /* Begin __nop_details::memory::unique_ptr_base header file */
#define NOP_DETAILS_MEMORY_UNIQUE_PTR_BASE_HPP 1UL

#pragma once

#ifndef NOP_DETAILS_MEMORY_UNIQUE_PTR_IMPL_HPP
  #error "This header file cannot be included explicitly. Use unique_ptr.hpp instead"
#endif

#include <cstdint> /* std::ptrdiff_t */

#include "NOP/base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace memory /* Begin namespace memory */
{

template<typename T>
class unique_ptr_base
{
 public:
  using value_type      = T;
  using difference_type = std::ptrdiff_t;
  using reference       = T&;
  using const_reference = const T&;
  using pointer         = T*;
  using const_pointer   = const T*;
};

} /* End namespace memory */

} /* End namespace __nop_details */

#endif /* End __nop_details::memory::unique_ptr_base header file */
