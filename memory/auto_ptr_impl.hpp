#ifndef NOP_DETAILS_MEMORY_AUTO_PTR_IMPL_HPP /* Begin __nop_details::memory::auto_ptr_impl header file */
#define NOP_DETAILS_MEMORY_AUTO_PTR_IMPL_HPP 1UL

#pragma once

#ifndef NOP_MEMORY_AUTO_PTR_HPP
  #error "This header file cannot be included explicitly. Use auto_ptr.hpp instead."
#endif

#include <concepts>

#include "auto_ptr_base.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace memory /* Begin namespace memory */
{

template<typename T>
concept valid_auto_ptr_t = std::destructible<std::remove_extent_t<T>> &&
                           std::movable<std::remove_extent_t<T>>      &&
                           std::copyable<std::remove_extent_t<T>>     &&
                           (sizeof(std::remove_extent_t<T>) > 0UL);

template<typename F, typename T>
concept valid_auto_ptr_d = std::invocable<F, std::remove_extent_t<T>*>;

template<
         __nop_details::memory::valid_auto_ptr_t T,
         __nop_details::memory::valid_auto_ptr_d<T> Deleter
        >
class auto_ptr_impl : public __nop_details::memory::auto_ptr_base<T>
{
 private:
  using base = typename __nop_details::memory::auto_ptr_base<T>;

 public:
  using element_type    = typename base::element_type;
  using value_type      = typename base::value_type;
  using size_type       = typename base::size_type;
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
  constexpr auto_ptr_impl() noexcept(std::is_nothrow_default_constructible_v<deleter_type>)
    : m_element{nullptr},
      m_deleter()
  {
    /* Empty */
  }

  constexpr auto_ptr_impl(pointer ptr) noexcept(std::is_nothrow_default_constructible_v<deleter_type>)
    : m_element{ptr},
      m_deleter()
  {
    /* Empty */
  }

  constexpr auto_ptr_impl(pointer ptr, const deleter_type& deleter) noexcept(std::is_nothrow_copy_constructible_v<deleter_type>)
    : m_element{ptr},
      m_deleter(deleter)
  {
    /* Empty */
  }

  constexpr auto_ptr_impl(pointer ptr, deleter_type&& deleter) noexcept(std::is_nothrow_move_constructible_v<deleter_type>)
    : m_element{ptr},
      m_deleter(std::move(deleter))
  {
    /* Empty */
  }

  constexpr auto_ptr_impl(const auto_ptr_impl& other) noexcept(std::is_nothrow_copy_constructible_v<deleter_type>)
    : m_element{other.m_element},
      m_deleter(other.m_deleter)
  {
    /* Empty */
  }

  constexpr auto_ptr_impl(auto_ptr_impl&& other) noexcept(std::is_nothrow_move_constructible_v<deleter_type>)
    : m_element{other.m_element},
      m_deleter(std::move(other.m_deleter))
  {
    other.m_element = nullptr;
  }

  constexpr ~auto_ptr_impl()
  {
    if (this->m_element)
    {
      this->m_deleter(this->m_element);
      this->m_element = nullptr;
    }
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

  [[nodiscard]] constexpr func release() noexcept -> pointer
  {
    pointer temp_ptr{this->m_element};
    this->m_element = nullptr;
    return temp_ptr;
  }

  constexpr func reset(pointer ptr = nullptr) noexcept -> void
  {
    this->m_element = ptr;
  }

  constexpr func swap(auto_ptr_impl& other) noexcept -> void
  {
    std::swap(this->m_element, other.m_element);
    std::swap(this->m_deleter, other.m_deleter);
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

  [[nodiscard]] constexpr func operator[](size_type index) noexcept -> reference
  requires std::is_unbounded_array_v<T>
  {
    return this->m_element[index];
  }

  [[nodiscard]] constexpr func operator[](size_type index) const noexcept -> reference
  requires std::is_unbounded_array_v<T>
  {
    return this->m_element[index];
  }

  [[nodiscard]] constexpr func operator-(const auto_ptr_impl& other) const noexcept -> difference_type
  {
    return this->m_element - other.m_element;
  }

  constexpr operator bool() const noexcept
  {
    return this->m_element;
  }

  [[nodiscard]] constexpr func operator!() const noexcept -> bool
  {
    return !this->m_element;
  }

  constexpr func operator=(const auto_ptr_impl& other) noexcept -> auto_ptr_impl&
  {
    if (this != &other)
    {
      this->m_element = other.m_element;
      this->m_deleter = other.m_deleter;
    }

    return *this;
  }

  constexpr func operator=(auto_ptr_impl&& other) noexcept -> auto_ptr_impl&
  {
    if (this != &other)
    {
      this->m_element = other.release();
      this->m_deleter = std::move(other.m_deleter);
    }

    return *this;
  }
};

} /* End namespace memory */

} /* End namespace __nop_details */

#endif /* End __nop_details::memory::autp_ptr_impl header file */
