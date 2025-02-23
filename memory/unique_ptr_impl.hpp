#ifndef NOP_DETAILS_MEMORY_UNIQUE_PTR_IMPL_HPP /* Begin __nop_details::memory::unique_ptr_impl header file */
#define NOP_DETAILS_MEMORY_UNIQUE_PTR_IMPL_HPP 1UL

#pragma once

#ifndef NOP_MEMORY_UNIQUE_PTR_HPP
  #error "This header file cannot be included explicitly. Use unique_ptr.hpp instead."
#endif

#include <concepts> /* std::destructible<T>, std::movable<T> */
#include <compare>  /* operator <=> overload */

#include "unique_ptr_base.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace memory /* Begin namespace memory */
{

template<typename T>
concept valid_unique_ptr_t = std::destructible<std::remove_extent_t<T>> &&
                             std::movable<std::remove_extent_t<T>>      &&
                             (sizeof(T) > 0UL);

template<typename T>
concept valid_default_deleter_t = !std::is_array_v<T>   &&
                                   std::destructible<T> &&
                                   (sizeof(T) > 0UL);

template<typename F, typename T>
concept valid_unique_ptr_d = std::invocable<F, T*>;

template<typename T, typename... Args>
concept valid_make_unique_t = !std::is_array_v<T>;

template<typename T>
concept valid_make_unique_for_overwrite_t = !std::is_array_v<T> &&
                                             std::default_initializable<T>;

template<
         __nop_details::memory::valid_unique_ptr_t T,
         __nop_details::memory::valid_unique_ptr_d<T> Deleter
        >
class unique_ptr_impl : public __nop_details::memory::unique_ptr_base<T>
{
 private:
  using base = typename __nop_details::memory::unique_ptr_base<T>;

 public:
  using value_type      = typename base::value_type;
  using difference_type = typename base::difference_type;
  using reference       = typename base::reference;
  using const_reference = typename base::const_reference;
  using pointer         = typename base::pointer;
  using const_pointer   = typename base::const_pointer;
  using deleter_type    = Deleter;

 private:
  pointer m_element;
  [[no_unique_address]] deleter_type m_deleter;

 public:
  constexpr unique_ptr_impl() noexcept
    : m_element{nullptr},
      m_deleter()
  {
    /* Empty */
  }

  constexpr unique_ptr_impl(unique_ptr_impl&& other) noexcept
    : m_element{other.m_element},
      m_deleter()
  {
    other.m_element = nullptr;
  }

  constexpr unique_ptr_impl(pointer ptr) noexcept
    : m_element{ptr},
      m_deleter()
  {
    /* Empty */
  }

  constexpr unique_ptr_impl(pointer ptr, const deleter_type& deleter) noexcept(std::is_nothrow_copy_constructible_v<deleter_type>)
    : m_element{ptr},
      m_deleter(deleter)
  {
    /* Empty */
  }

  constexpr unique_ptr_impl(pointer ptr, deleter_type&& deleter) noexcept(std::is_nothrow_move_constructible_v<deleter_type>)
    : m_element{ptr},
      m_deleter(std::move(deleter))
  {
    /* Empty */
  }

  constexpr ~unique_ptr_impl()
  {
    if (this->m_element)
    {
      m_deleter(this->m_element);
      this->m_element = nullptr;
    }
  }

  constexpr func reset(pointer ptr = nullptr) noexcept(noexcept(this->m_deleter(this->m_element))) -> void
  {
    if (this->m_element)
    {
      this->m_deleter(this->m_element);
    }

    this->m_element = ptr;
  }

  [[nodiscard]] constexpr func release() noexcept -> pointer
  {
    pointer temp_ptr{this->m_element};
    this->m_element = nullptr;
    return temp_ptr;
  }

  constexpr func swap(unique_ptr_impl& other) noexcept -> void
  {
    std::swap(this->m_element, other.m_element);
  }

  [[nodiscard]] constexpr func get() noexcept -> pointer
  {
    return this->m_element;
  }

  [[nodiscard]] constexpr func get() const noexcept -> const_pointer
  {
    return this->m_element;
  }

  [[nodiscard]] constexpr func get_deleter() const noexcept -> deleter_type
  {
    return this->m_deleter;
  }

  [[nodiscard]] constexpr func operator*() noexcept -> reference
  {
    return *this->m_element;
  }

  [[nodiscard]] constexpr func operator*() const noexcept -> const_reference
  {
    return *this->m_element;
  }

  [[nodiscard]] constexpr func operator->() noexcept -> pointer
  {
    return this->m_element;
  }

  [[nodiscard]] constexpr func operator->() const noexcept -> const_pointer
  {
    return this->m_element;
  }

  [[nodiscard]] constexpr func operator!() const noexcept -> bool
  {
    return !this->m_element;
  }

  constexpr operator bool() const noexcept
  {
    return this->m_element;
  }

  [[nodiscard]] constexpr func operator-(const unique_ptr_impl& other) const noexcept -> difference_type
  {
    return this->m_element - other.m_element;
  }

  constexpr func operator=(unique_ptr_impl&& other) noexcept -> unique_ptr_impl&
  {
    if (this != &other)
    {
      this->swap(other.release());
    }

    return *this;
  }

};

} /* End namespace memory */

} /* End namespace __nop_details */

#endif /* End __nop_details::memory::unique_ptr_impl header file */
