#ifndef NOP_SMART_PTR_HPP /* Begin smart ptr header file */
#define NOP_SMART_PTR_HPP 1

#pragma once

#include <type_traits>

#include <fmt/format.h>

#include "exception.hpp"

#include <cstdint>
#include <cassert>


namespace nop /* Begin namespace nop */
{

namespace strategy /* Begin namespace strategy */
{

  /**
   * @brief Strategy class/struct for conversion policy
   *
   * @class all_conversion
   * all_conversion Applies the implicit conversions to bool, void*
   *
   * @enum Consists compile-time constants for valid/invalid conversions
   */
  class all_conversion
  {
   public:
    enum : bool
    {
      VOID_POINTER = true,
      BOOL = true
    };
  };

  /**
   * @brief Strategy class for conversion policy
   *
   * @class no_conversion
   * no_conversion Denies the implicit conversions to bool, void*
   *
   * @enum Consists compile-time constants for valid/invalid conversions
   */
  class no_conversion
  {
   public:
    enum : bool
    {
      VOID_POINTER = false,
      BOOL = false
    };
  };
 
  /**
   * @brief Strategy class for store policy
   *
   * @class single_storage
   * single_storage Create, Delete and provide compile-time constant
   * to check the operator subscript support
   * This support gives information that this pointer is containing the
   * single object
   *
   * @tparam T -> The target type
   *
   * @function Create
   * @tparam [in] Args -> The parameter pack for object constructor
   * @throw std::bad_alloc()
   * @return T*
   *
   * @function Delete
   * @param [in] ptr -> Memory to deallocate (previously allocated via new)
   * @return None
   *
   * @enum Consist the information about operator subscript overloading
   */
  template<typename T>
  class single_storage
  {
   public:
    using value_type = T;
    using pointer    = value_type*;

   public:
    template<typename... Args>
    [[nodiscard]] static pointer Create(Args&&... args)
    {
      return new value_type(std::forward<Args>(args)...);
    }

    static void Delete(pointer ptr) noexcept
    {
      delete ptr;
    }

    enum : bool { RND_ACCESS = false };
  };

  /**
   * @brief Strategy class for store policy
   *
   * @class multiple_storage
   * multiple_storage Create, Delete and provide compile-time constant
   * to check the operator subscript support
   * This support gives information that this pointer is containing the
   * contiguios storage allocated on the heap
   *
   * @tparam T -> The target type
   *
   * @function Create
   * @param [in] initSize -> The length of the array
   * @tparam [in] Args -> The parameter pack containing arguments
   * for constructor
   * @throw std::bad_alloc()
   * @return T*
   *
   * @function Delete
   * @param [in] ptr -> Memory to deallocate (previously allocated via new[])
   * @return None
   *
   * @enum Consist the information about operator subscript overloading
   */
  template<typename T>
  class multiple_storage
  {
   public:
    using value_type = T;
    using pointer    = value_type*;
    using size_type  = std::size_t;

   public:
    /* Maybe a bit expensive, Naive approach */
    template<typename... Args>
    [[nodiscard]] static pointer Create(size_type init_size, Args&&... args)
    {
      pointer ptr{new value_type[init_size]};
      if constexpr (std::is_move_assignable_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        for (std::size_t i{}; i < init_size; ++i)
        {
          ptr[i] = static_cast<value_type&&>(value_type(std::forward<Args>(args)...));
        }
      }
      else if constexpr (std::is_copy_assignable_v<value_type>)
      {
        value_type temp_val(std::forward<Args>(args)...);
        for (std::size_t i{}; i < init_size; ++i)
        {
          ptr[i] = temp_val;
        }
      }
      return ptr;
    }

    static void Delete(pointer ptr) noexcept
    {
      delete[] ptr;
    }

    enum : bool { RND_ACCESS = true };
  };

  /**
  * @brief Template strategy class for safety policy
  *
  * @class exception_safety
  * exception_safety checks the validness of the provided pointer
  *
  * @tparam T -> The target type
  *
  * @function Check
  * @param [in] ptr -> Pointer that need to be checked
  * @throw nop::err::InvalidArgument
  * @return None
  */
  template<typename T>
  class exception_safety
  {
   public:
    using value_type = T;
    using pointer    = value_type*;

   public:
    static void Check(pointer ptr)
    {
      if (!ptr)
      {
        throw err::InvalidArgument{
              fmt::format("exception_safety::Check(pointer ptr) -> {}",
                          static_cast<const void*>(ptr))};
      }
    }
  };

  /**
   * @brief Template strategy class for safety policy
   *
   * @class assert_safety
   * assert_safety uses the C assert macro to check the validness
   * of the provided pointer
   *
   * @tparam T -> The target type
   *
   * @function Check
   * @param [in] ptr -> Pointer that need to be checked
   * @return None
   */
  template<typename T>
  class assert_safety
  {
   public:
    using value_type = T;
    using pointer    = value_type*;

   public:
    static void Check(pointer ptr) noexcept
    {
      assert(ptr != nullptr);
    }
  };

  /**
   * @brief Template strategy class for safety policy
   *
   * @class no_safety
   * no_safety does not check the passed parameter and acts like stub
   *
   * @tparam T -> The target type
   *
   * @function Check
   * @param [in] ptr -> Pointer that need to be checked
   * @return None
   */
  template<typename T>
  class no_safety
  {
   public:
    using value_type = T;
    using pointer    = value_type*;

   public:
    static void Check([[maybe_unused]] pointer ptr) noexcept
    {
      /* Empty */
    }
  };

} /* End namespace strategy */

  /**
   * @brief Template smart pointer
   *
   * @class smart_ptr
   * SmartPtr Recieves four strategies and maintain it's storage depending
   * on provided strategies
   *
   * @tparam T -> The target type
   * @tparam Conversion -> Strategy struct/class for conversion policy
   * Default: all_conversion
   * @see all_conversion
   * @tparam Safety -> Strategy struct/class for safety policy
   * Default: no_safety
   * @see no_safety
   * @tparam Storage -> Strategy struct/class for storage policy
   * Default: single_storage
   * @see single_storage
   */
  template<
           typename T,
// TODO: Implement new class-strategies -> Ownership
// {template<typename> class Ownership = unique_ownership}
           class Conversion = strategy::all_conversion,
           template<typename> class Safety = strategy::no_safety,
           template<typename> class Storage = strategy::single_storage
          >
  class smart_ptr
  {
   public:
    using value_type      = T;
    using size_type       = std::size_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

   private:
    pointer m_pointer;

   public:
    constexpr smart_ptr() noexcept
      : m_pointer{nullptr}
    {
      /* Empty */
    }

    explicit constexpr smart_ptr(pointer ptr) noexcept
      : m_pointer{ptr}
    {
      /* Empty */
    }

    smart_ptr(const smart_ptr&) = delete;

    constexpr smart_ptr(smart_ptr&& other) noexcept
      : m_pointer{other.release()}
    {
      /* Empty */
    }

    ~smart_ptr()
    {
      Storage<value_type>::Delete(m_pointer);
    }

    [[nodiscard]] constexpr pointer get() noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<value_type>::Check(m_pointer);
      return m_pointer;
    }

    [[nodiscard]] constexpr const_pointer get() const noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<value_type>::Check(m_pointer);
      return m_pointer;
    }

    constexpr void reset(pointer ptr = nullptr) noexcept
    {
      Storage<value_type>::Delete(m_pointer);
      m_pointer = ptr;
    }

    [[nodiscard]] constexpr pointer release() noexcept
    {
      pointer tmp_ptr{m_pointer};
      m_pointer = nullptr;
      return tmp_ptr;
    }

    [[nodiscard]] constexpr reference operator*() noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<value_type>::Check(m_pointer);
      return *m_pointer;
    }

    [[nodiscard]] constexpr const_reference operator*() const noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<value_type>::Check(m_pointer);
      return *m_pointer;
    }

    [[nodiscard]] pointer operator->() noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<value_type>::Check(m_pointer);
      return m_pointer;
    }

    [[nodiscard]] const_pointer operator->() const noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<value_type>::Check(m_pointer);
      return m_pointer;
    }

    [[nodiscard]] reference operator[](size_type index) noexcept(noexcept(Safety<T>::Check(nullptr)))
    requires (static_cast<bool>(Storage<value_type>::RND_ACCESS))
    {
      Safety<value_type>::Check(m_pointer);
      return m_pointer[index];
    }

    [[nodiscard]] const_reference operator[](size_type index) const noexcept(noexcept(Safety<T>::Check(nullptr)))
    requires (static_cast<bool>(Storage<value_type>::RND_ACCESS))
    {
      Safety<T>::Check(m_pointer);
      return m_pointer[index];
    }

    smart_ptr& operator=(const smart_ptr&) = delete;

    smart_ptr& operator=(smart_ptr&& other) noexcept
    {
      Storage<T>::Delete(m_pointer);
      m_pointer = other.release();
    }

    [[nodiscard]] explicit operator void*() noexcept
    requires (static_cast<bool>(Conversion::VOID_POINTER))
    {
      return m_pointer;
    }

    [[nodiscard]] explicit operator const void*() const noexcept
    requires (static_cast<bool>(Conversion::VOID_POINTER))
    {
      return m_pointer;
    }

    [[nodiscard]] operator bool() const noexcept
    requires (static_cast<bool>(Conversion::BOOL))
    {
      return m_pointer != nullptr;
    }

    [[nodiscard]] bool operator!() const noexcept
    {
      return !m_pointer;
    }

    template<typename U>
    [[nodiscard]] bool operator==(const smart_ptr<U>& s_ptr) const noexcept
    {
      return static_cast<const void*>(m_pointer) == static_cast<const void*>(s_ptr);
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator==(const smart_ptr& s_ptr, const U* r_ptr) noexcept
    {
      return static_cast<const void*>(s_ptr.m_pointer) == static_cast<const void*>(r_ptr);
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator==(const U* r_ptr, const smart_ptr& s_ptr) noexcept
    {
      return static_cast<const void*>(r_ptr) == static_cast<const void*>(s_ptr.m_pointer);
    }

    template<typename U>
    [[nodiscard]] bool operator!=(const smart_ptr<U>& s_ptr) const noexcept(noexcept(s_ptr.get()))
    {
      return static_cast<const void*>(m_pointer) != static_cast<const void*>(s_ptr.get());
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator!=(const smart_ptr& s_ptr, const U* r_ptr) noexcept
    {
      return static_cast<const void*>(s_ptr.m_pointer) != static_cast<const void*>(r_ptr);
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator!=(const U* r_ptr, const smart_ptr& s_ptr) noexcept
    {
      return static_cast<const void*>(r_ptr) != static_cast<const void*>(s_ptr.m_pointer);
    }

  };

  /**
   * @brief Factory function for constructing smart_ptr with perfect forwarding
   *
   * @function make_smart_ptr
   * @tparam [in, out] T Is the target type
   * @tparam [in, out] Conversion -> Strategy struct/class
   * for obtaining the conversion policy
   * Default: all_conversion
   * @see all_conversion
   * @tparam [in, out] Safety -> Strategy struct/class
   * for obtaining the safety policy
   * Default: no_safety
   * @see no_safety
   * @tparam [in, out] Storage -> Strategy struct/class
   * for obtaining the storage policy
   * Default: single_storage
   * @see single_storage
   * @tparam [in] Args -> The parameter pack for the object constructors
   * (perfect forwarding)
   * @throw std::bad_alloc() if nop storage policy provided
   * or strategy dependent
   * @result smart_ptr
   ****************************************************************************/
  template<
           typename T,
           class Conversion = strategy::all_conversion,
           template<typename> class Safety = strategy::no_safety,
           template<typename> class Storage = strategy::single_storage,
           typename... Args
          >
  [[nodiscard]] smart_ptr<T, Conversion, Safety, Storage> make_smart_ptr(Args&&... args) noexcept(Storage<T>::RND_ACCESS == true &&
                                                                                                  noexcept(Storage<T>::Create(10000000000000UL)))
  {
    smart_ptr<T, Conversion, Safety, Storage> ptr{Storage<T>::Create(std::forward<Args>(args)...)};
    return ptr;
  }

} /* End namespace nop */

#endif /* End smart ptr header file */
