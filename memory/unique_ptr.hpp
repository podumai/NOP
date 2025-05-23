#ifndef NOP_MEMORY_UNIQUE_PTR_HPP /* Begin nop::memory::unique_ptr header file */
#define NOP_MEMORY_UNIQUE_PTR_HPP 1UL

#pragma once

#include "default_deleter.hpp"
#include "unique_ptr_impl.hpp"

namespace nop /* Begin namespace nop */
{

namespace memory /* Begin namespace memory */
{

template<
         typename T,
         class Deleter = nop::memory::default_deleter<T>
        >
class unique_ptr : public __nop_details::memory::unique_ptr_impl<T, Deleter>
{
 private:
  using base = typename __nop_details::memory::unique_ptr_impl<T, Deleter>;

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
  constexpr unique_ptr() noexcept(noexcept(base())) = default;

  constexpr unique_ptr(pointer ptr) noexcept(noexcept(base(ptr)))
    : base(ptr)
  {
    /* Empty */
  }

  constexpr unique_ptr(pointer ptr, const deleter_type& deleter) noexcept(noexcept(base(ptr, deleter)))
    : base(ptr, deleter)
  {
    /* Empty */
  }

  constexpr unique_ptr(pointer ptr, deleter_type&& deleter) noexcept(noexcept(base(ptr, std::move(deleter))))
    : base(ptr, std::move(deleter))
  {
    /* Empty */
  }

  constexpr unique_ptr(unique_ptr&& other) noexcept
    : base(static_cast<base&&>(other))
  {
    /* Empty */
  }

  constexpr func operator=(unique_ptr&& other) noexcept(noexcept(static_cast<unique_ptr&>(static_cast<base&>(*this) = static_cast<base&&>(other)))) -> unique_ptr&
  {
    return static_cast<unique_ptr&>(static_cast<base&>(*this) = static_cast<base&&>(other));
  }
};

template<
         typename T,
         typename... Args
        >
[[nodiscard]] constexpr func make_unique(Args&&... args) -> nop::memory::unique_ptr<T>
requires (!std::is_array_v<T>)
{
  return new T(std::forward<Args>(args)...);
}

template<typename T>
[[nodiscard]] constexpr func make_unique(std::size_t n) -> nop::memory::unique_ptr<T>
requires std::is_unbounded_array_v<T>
{
  return new std::remove_extent_t<T>[n]();
}

template<__nop_details::memory::valid_make_unique_for_overwrite_t T>
[[nodiscard]] constexpr func make_unique_for_overwrite() -> nop::memory::unique_ptr<T>
requires (!std::is_array_v<T>)
{
  return new T;
}

template<__nop_details::memory::valid_make_unique_for_overwrite_t T>
[[nodiscard]] constexpr func make_unique_for_overwrite(std::size_t n) -> nop::memory::unique_ptr<T>
requires std::is_unbounded_array_v<T>
{
  return new std::remove_extent_t<T>[n];
}

} /* End namespace memory */

} /* End namespace nop */

template<
          typename T,
          class Deleter
         >
[[nodiscard]] constexpr func operator==(const nop::memory::unique_ptr<T, Deleter>& lhs,
                                        const nop::memory::unique_ptr<T, Deleter>& rhs) noexcept -> bool
{
  return lhs.get() == rhs.get();
}

template<
         typename T,
         class Deleter
        >
[[nodiscard]] constexpr func operator<=>(const nop::memory::unique_ptr<T, Deleter>& lhs,
                                         const nop::memory::unique_ptr<T, Deleter>& rhs) noexcept -> decltype(lhs.get() <=> rhs.get())
{
  return lhs.get() <=> rhs.get();
}

namespace std /* Begin namespace std */
{

template<
         typename T,
         class Deleter
        >
constexpr func swap(nop::memory::unique_ptr<T, Deleter>& lhs,
                    nop::memory::unique_ptr<T, Deleter>& rhs) noexcept -> void
{
  lhs.swap(rhs);
}

} /* End namespace std */

#endif /* End nop::memory::unique_ptr header file */
