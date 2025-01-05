#ifndef NOP_SMART_PTR_HPP /* Begin smart ptr header file */
#define NOP_SMART_PTR_HPP 1

#include <type_traits>
#include <fmt/format.h>
#include <cassert>
#include "exception.hpp"
#include "types.hpp"

namespace nop /* Begin namespace nop */
{

  /*************************************************************************//**
   * @brief Strategy class/struct for conversion policy
   *
   * @class AllowConversion
   * AllowConversion Applies the implicit conversions to bool, void*
   *
   * @enum Consists compile-time constants for valid/invalid conversions
   ****************************************************************************/
  class AllowConversion
  {
  public:
    enum
    {
      VOID_POINTER = true,
      BOOL = true
    };
  };

  /*************************************************************************//**
   * @brief Strategy class for conversion policy
   *
   * @class DisallowConversion
   * DisallowConversion Denies the implicit conversions to bool, void*
   *
   * @enum Consists compile-time constants for valid/invalid conversions
   ****************************************************************************/
  class DisallowConversion
  {
  public:
    enum
    {
      VOID_POINTER = false,
      BOOL = false
    };
  };
  
  /*************************************************************************//**
   * @brief Strategy class for store policy
   *
   * @class SingleStorage
   * SingleStorage Create, Delete and provide compile-time constant
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
   ****************************************************************************/
  template <typename T>
  class SingleStorage 
  {
  public:
    using pointer = T*;

  public:
    template<typename... Args>
    [[nodiscard]] static pointer Create(Args... args) noexcept(false)
    {
      return new T(std::forward<Args>(args)...);
    }

    static void Delete(T* pointer) noexcept
    {
      if (pointer != nullptr)
      {
        delete pointer;
      }
    }

    enum { RND_ACCESS = false };
  };

  /*************************************************************************//**
   * @brief Strategy class for store policy
   *
   * @class MultipleStorage
   * MultipleStorage Create, Delete and provide compile-time constant
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
   ****************************************************************************/
  template<typename T>
  class MultipleStorage
  {
  public:
    using pointer = T*;

  public:
    /* Maybe a bit expensive */
    template<typename... Args>
    [[nodiscard]] static pointer Create(size_t initSize, Args... args) noexcept(false)
    {
      pointer ptr{new T[initSize]};
      if constexpr (pointer tempPtr{ptr}; sizeof... (Args) > 0UL)
      {
        for (size_t i{}; i < initSize; ++i)
        {
          tempPtr = new(tempPtr) T(std::forward<Args>(args)...);
          ++tempPtr;
        }
      }
      return ptr;
    }

    static void Delete(T* pointer) noexcept
    {
      if (pointer != nullptr)
      {
        delete[] pointer;
      }
    }

    enum { RND_ACCESS = true };
  };

  /*************************************************************************//**
  * @brief Template strategy class for safety policy
  *
  * @class ExceptionSafety
  * ExceptionSafety checks the validness of the provided pointer
  *
  * @tparam T -> The target type
  *
  * @function Check
  * @param [in] ptr -> Pointer that need to be checked
  * @throw nop::err::InvalidArgument
  * @return None
  *****************************************************************************/
  template<typename T>
  class ExceptionSafety
  {
  public:
    using pointer = T*;

  public:
    static void Check(pointer ptr) noexcept(false)
    {
      if (ptr == nullptr)
      {
        throw err::InvalidArgument{
              fmt::format("ExceptionSafety:Check(pointer ptr) -> {}"
                          , static_cast<const void*>(ptr))};
      }
    }
  };

  /*************************************************************************//**
   * @brief Template strategy class for safety policy
   *
   * @class AssertSafety
   * AssertSafety uses the C assert macro to check the validness
   * of the provided pointer
   *
   * @tparam T -> The target type
   *
   * @function Check
   * @param [in] ptr -> Pointer that need to be checked
   * @return None
   ****************************************************************************/
  template<typename T>
  class AssertSafety
  {
  public:
    using pointer = T*;

  public:
    static void Check(pointer ptr) noexcept
    {
      assert(ptr != nullptr);
    }
  };

  /*************************************************************************//**
   * @brief Template strategy class for safety policy
   *
   * @class NoSafety
   * NoSafety does not check the passed parameter and acts like stub
   *
   * @tparam T -> The target type
   *
   * @function Check
   * @param [in] ptr -> Pointer that need to be checked
   * @return None
   ****************************************************************************/
  template<typename T>
  class NoSafety
  {
  public:
    using pointer = T*;

  public:
    static void Check([[maybe_unused]] pointer ptr) noexcept
    {}
  };

  /************************************************************************//**
   * @brief Template smart pointer
   *
   * @class SmartPtr
   * SmartPtr Recieves four strategies and maintain it's storage depending
   * on provided strategies
   *
   * @tparam T -> The target type
   * @tparam Conversion -> Strategy struct/class for conversion policy
   * Default: AllowConversion
   * @see AllowConversion
   * @tparam Safety -> Strategy struct/class for safety policy
   * Default: NoSafety
   * @see NoSafety
   * @tparam Storage -> Strategy struct/class for storage policy
   * Default: SingleStorage
   * @see SingleStorage
   ****************************************************************************/
  template<
           typename T,
// TODO: Implement new class-strategies -> Ownership
// {template<typename> class Ownership = unique_ownership}
           class Conversion = AllowConversion,
           template<typename> class Safety = NoSafety,
           template<typename> class Storage = SingleStorage
          >
  class SmartPtr
  {
  public:
    using sizeType = size_t;
    using valueType = T;
    using pointer = T*;
    using constPointer = const T*;
    using reference = T&;
    using constReference = const T&;

  private:
    pointer m_pointer;

  public:
    constexpr SmartPtr() noexcept
      : m_pointer{nullptr}
    {}

    explicit SmartPtr(pointer ptr) noexcept
      : m_pointer{ptr}
    {}

    SmartPtr(const SmartPtr&) = delete;

    SmartPtr(SmartPtr&& other) noexcept
      : m_pointer{other.release()}
    {}

    ~SmartPtr()
    {
      Storage<T>::Delete(m_pointer);
    }

    [[nodiscard]] pointer get() noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<T>::Check(m_pointer);
      return m_pointer;
    }

    [[nodiscard]] constPointer get() const noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<T>::Check(m_pointer);
      return m_pointer;
    }

    void reset(pointer ptr = nullptr) noexcept
    {
      Storage<T>::Delete(m_pointer);
      m_pointer = ptr;
    }

    pointer release() noexcept
    {
      pointer tmp_ptr{m_pointer};
      m_pointer = nullptr;
      return tmp_ptr;
    }

    [[nodiscard]] reference operator*() noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<T>::Check(m_pointer);
      return *m_pointer;
    }

    [[nodiscard]] constReference operator*() const noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<T>::Check(m_pointer);
      return *m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] std::enable_if_t<std::is_class_v<U> == true, T>* operator->() noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<T>::Check(m_pointer);
      return m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] const std::enable_if_t<std::is_class_v<U> == true, T>* operator->() const noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<T>::Check(m_pointer);
      return m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] std::enable_if_t<Storage<U>::RND_ACCESS == true, T>& operator[](sizeType index) noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<T>::Check(m_pointer);
      return m_pointer[index];
    }

    template<typename U = T>
    [[nodiscard]] const std::enable_if_t<Storage<U>::RND_ACCESS == true, T>& operator[](sizeType index) const noexcept(noexcept(Safety<T>::Check(nullptr)))
    {
      Safety<T>::Check(m_pointer);
      return m_pointer[index];
    }

    SmartPtr& operator=(const SmartPtr&) = delete;

    SmartPtr& operator=(SmartPtr&& other) noexcept
    {
      Storage<T>::Delete(m_pointer);
      m_pointer = other.release();
    }

    [[nodiscard]] operator std::enable_if_t<Conversion::VOID_POINTER == true, void*>() noexcept
    {
      return m_pointer;
    }

    [[nodiscard]] operator const std::enable_if_t<Conversion::VOID_POINTER == true, void*>() const noexcept
    {
      return m_pointer;
    }

    [[nodiscard]] operator std::enable_if_t<Conversion::BOOL == true, bool>() const noexcept
    {
      return m_pointer != nullptr;
    }

    [[nodiscard]] bool operator!() const noexcept
    {
      return !m_pointer;
    }

    template<typename U>
    [[nodiscard]] bool operator==(const SmartPtr<U>& s_ptr) const noexcept
    {
      return static_cast<const void*>(m_pointer) == static_cast<const void*>(s_ptr);
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator==(const SmartPtr& s_ptr, const U* r_ptr) noexcept
    {
      return static_cast<const void*>(s_ptr.m_pointer) == static_cast<const void*>(r_ptr);
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator==(const U* r_ptr, const SmartPtr& s_ptr) noexcept
    {
      return static_cast<const void*>(r_ptr) == static_cast<const void*>(s_ptr.m_pointer);
    }

    template<typename U>
    [[nodiscard]] bool operator!=(const SmartPtr<U>& s_ptr) const noexcept(noexcept(s_ptr.get()))
    {
      return static_cast<const void*>(m_pointer) != static_cast<const void*>(s_ptr.get());
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator!=(const SmartPtr& s_ptr, const U* r_ptr) noexcept
    {
      return static_cast<const void*>(s_ptr.m_pointer) != static_cast<const void*>(r_ptr);
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator!=(const U* r_ptr, const SmartPtr& s_ptr) noexcept
    {
      return static_cast<const void*>(r_ptr) != static_cast<const void*>(s_ptr.m_pointer);
    }

  };

  /************************************************************************//**
   * @brief Factory function for constructing smart ptr with perfect forwarding
   *
   * @function makeSmartPtr
   * @tparam [in, out] T Is the target type
   * @tparam [in, out] Conversion -> Strategy struct/class
   * for obtaining the conversion policy
   * Default: AllowConversion
   * @see AllowConversion
   * @tparam [in, out] Safety -> Strategy struct/class
   * for obtaining the safety policy
   * Default: NoSafety
   * @see NoSafety
   * @tparam [in, out] Storage -> Strategy struct/class
   * for obtaining the storage policy
   * Default: SingleStorage
   * @see SingleStorage
   * @tparam [in] Args -> The parameter pack for the object constructors
   * (perfect forward)
   * @throw std::bad_alloc() if nop storage policy provided
   * or strategy dependent
   * @result SmartPtr rvalue reference
   ****************************************************************************/
  template<
           typename T,
           class Conversion = AllowConversion,
           template<typename> class Safety = NoSafety,
           template<typename> class Storage = SingleStorage,
           typename... Args
          >
  [[nodiscard]] SmartPtr<T, Conversion, Safety, Storage> makeSmartPtr(Args&&... args) noexcept(Storage<T>::RND_ACCESS == true &&
                                                                                      noexcept(Storage<T>::Create(10000000000000UL)))
  {
    SmartPtr<T, Conversion, Safety, Storage> ptr{Storage<T>::Create(std::forward<Args>(args)...)};
    return ptr;
  }

} /* End namespace nop */

#endif /* End smart ptr header file */
