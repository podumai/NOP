#ifndef NOP_MEMORY_AUTO_PTR_HPP /* Begin nop::memory::auto_ptr header file */
#define NOP_MEMORY_AUTO_PTR_HPP 1UL

#pragma once

#include <compare> /* operator <=> overload */

#include "default_deleter.hpp"
#include "auto_ptr_impl.hpp"

namespace nop /* Begin namespace nop */
{

namespace memory /* Begin namespace memory */
{

template<
         typename T,
         class Deleter = nop::memory::default_deleter<T>
        >
class auto_ptr : public __nop_details::memory::auto_ptr_impl<T, Deleter>
{
 private:
  using base = typename __nop_details::memory::auto_ptr_impl<T, Deleter>;

 public:
  using element_type    = typename base::element_type;
  using value_type      = typename base::value_type;
  using size_type       = typename base::size_type;
  using difference_type = typename base::difference_type;
  using reference       = typename base::reference;
  using const_reference = typename base::const_reference;
  using pointer         = typename base::pointer;
  using const_pointer   = typename base::const_pointer;
  using deleter_type    = typename base::deleter_type;

 public:
  constexpr auto_ptr() noexcept(noexcept(base())) = default;

  constexpr auto_ptr(pointer ptr) noexcept(noexcept(base(ptr)))
    : base(ptr)
  {
    /* Empty */
  }

  constexpr auto_ptr(pointer ptr, const deleter_type& deleter) noexcept(noexcept(base(ptr, deleter)))
    : base(ptr, deleter)
  {
    /* Empty */
  }

  constexpr auto_ptr(pointer ptr, deleter_type&& deleter) noexcept(noexcept(base(ptr, std::move(deleter))))
    : base(ptr, std::move(deleter))
  {
    /* Empty */
  }

  constexpr auto_ptr(const auto_ptr& other) noexcept(noexcept(base(other)))
    : base(other)
  {
    /* Empty */
  }

  constexpr auto_ptr(auto_ptr&& other) noexcept(noexcept(base(other)))
    : base(static_cast<base&&>(other))
  {
    /* Empty */
  }

  constexpr func operator=(const auto_ptr& other) noexcept(noexcept(static_cast<auto_ptr&>(static_cast<base&>(*this) = static_cast<const base&>(other)))) -> auto_ptr&
  {
    return static_cast<auto_ptr&>(static_cast<base&>(*this) = static_cast<const base&>(other));
  }

  constexpr func operator=(auto_ptr&& other) noexcept(noexcept(static_cast<auto_ptr&>(static_cast<base&>(*this) = static_cast<base&&>(other)))) -> auto_ptr&
  {
    return static_cast<auto_ptr&>(static_cast<base&>(*this) = static_cast<base&&>(other));
  }
};

template<
         __nop_details::memory::valid_auto_ptr_t T,
         typename... Args
        >
[[nodiscard]] func make_auto_ptr(Args&&... args) -> nop::memory::auto_ptr<T>
{
  return new T(std::forward<Args>(args)...);
}

template<__nop_details::memory::valid_auto_ptr_t T>
[[nodiscard]] func make_auto_ptr_for_overwrite() -> nop::memory::auto_ptr<T>
{
  return new T;
}

} /* End namespace memory */

} /* End namespace nop */

template<
         typename T,
         class Deleter
        >
[[nodiscard]] constexpr func operator==(const nop::memory::auto_ptr<T, Deleter>& lhs,
                                        const nop::memory::auto_ptr<T, Deleter>& rhs) noexcept -> bool
{
  return lhs.get() == rhs.get();
}

template<
         typename T,
         class Deleter
        >
[[nodiscard]] constexpr func operator<=>(const nop::memory::auto_ptr<T, Deleter>& lhs,
                                         const nop::memory::auto_ptr<T, Deleter>& rhs) noexcept -> decltype(lhs.get() <=> rhs.get())
{
  return lhs.get() <=> rhs.get();
}

#endif /* End nop::memory::auto_ptr header file */
