#ifndef NOP_SMART_PTR_HPP /* Begin smart ptr header file */
#define NOP_SMART_PTR_HPP 1

#include <type_traits>
#include <fmt/format.h>
#include <cassert>
#include "exception.hpp"
#include "types.hpp"

namespace nop /* Begin namespace nop */
{

  /**
   * @brief Strategy class/struct for conversion policy
   *
   * @class AllowConversion
   * @enum Consist valid conversions
   */
  struct AllowConversion
  {
  public:
    enum
    {
      VOID_POINTER = true,
      BOOL = true
    };
  };

  /**
   * @brief Strategy class/struct for conversion policy
   *
   * @class DisallowConversion
   * @enum Consist valid conversions
   */
  struct DisallowConversion
  {
  public:
    enum
    {
      VOID_POINTER = false,
      BOOL = false
    };
  };
  
  /**
   * @brief Strategy class/struct for store policy
   *
   * @class SingleStorage
   * @enum Consist the information about operator subscript overloading
   */
  template <typename T>
  struct SingleStorage 
  {
  public:
    using pointer = T*;

  public:
    template<typename... Args>
    [[nodiscard]] static pointer Create(Args... args)
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

  /**
   * @brief Strategy class/struct for store policy
   *
   * @class MultipleStorage
   * @enum Consist the information about operator subscript overloading
   */
  template<typename T>
  struct MultipleStorage
  {
  public:
    using pointer = T*;

  public:
    /**
     * Maybe a bit expensive
     */
    template<typename... Args>
    [[nodiscard]] static pointer Create(size_t initSize, Args... args)
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

  template<typename T>
  struct ExceptionSafety
  {
  public:
    using pointer = T*;

  public:
    static void Check(pointer ptr)
    {
      if (ptr == nullptr)
      {
        throw err::InvalidArgument{
              fmt::format("ExceptionSafety:Check(pointer ptr) -> {}"
                          , static_cast<const void*>(ptr))};
      }
    }
  };

  template<typename T>
  struct AssertSafety
  {
  public:
    using pointer = T*;

  public:
    static void Check(pointer ptr) noexcept
    {
      assert(ptr != nullptr);
    }
  };

  template<typename T>
  struct NoSafety
  {
  public:
    using pointer = T*;

  public:
    static void Check([[maybe_unused]] pointer ptr) noexcept
    {}
  };

  /**
   * @brief Template smart pointer maintaing it's storage depending on provided strategies
   *
   * @class SmartPtr
   *
   * @tparam T is the object type to be stored
   * @tparam Conversion is the strategy class for conversion policy
   * @tparam Storage is the strategy class for storage policy
   */
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

    [[nodiscard]] pointer get()
    {
      Safety<T>::Check(m_pointer);
      return m_pointer;
    }

    [[nodiscard]] constPointer get() const
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

    [[nodiscard]] reference operator*()
    {
      Safety<T>::Check(m_pointer);
      return *m_pointer;
    }

    [[nodiscard]] constReference operator*() const
    {
      Safety<T>::Check(m_pointer);
      return *m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] std::enable_if_t<std::is_class_v<U> == true, T>* operator->()
    {
      Safety<T>::Check(m_pointer);
      return m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] const std::enable_if_t<std::is_class_v<U> == true, T>* operator->() const
    {
      Safety<T>::Check(m_pointer);
      return m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] std::enable_if_t<Storage<U>::RND_ACCESS == true, T>& operator[](sizeType index)
    {
      Safety<T>::Check(m_pointer);
      return m_pointer[index];
    }

    template<typename U = T>
    [[nodiscard]] const std::enable_if_t<Storage<U>::RND_ACCESS == true, T>& operator[](sizeType index) const
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
      return static_cast<const void*>(m_pointer) == static_cast<const void*>(s_ptr.get());
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator==(const SmartPtr& s_ptr, const U* r_ptr)
    {
      return static_cast<const void*>(s_ptr.m_pointer) == static_cast<const void*>(r_ptr);
    }

    template<typename U>
    [[nodiscard]] inline friend bool operator==(const U* r_ptr, const SmartPtr& s_ptr)
    {
      return static_cast<const void*>(r_ptr) == static_cast<const void*>(s_ptr.m_pointer);
    }

    template<typename U>
    [[nodiscard]] bool operator!=(const SmartPtr<U>& s_ptr) const noexcept
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

  /**
   * @brief Factory function for constructing smart ptr with perfect forwarding
   *
   * @tparam T Is the target type
   * @tparam Conversion struct/class strategy for obtaining the conversion policy
   * Default: AllowConversion
   * @tparam Safety struct/class strategy for obtaining the safety policy
   * Default: NoSafety
   * @tparam Storage struct/class strategy for obtaining the storage policy
   * Default: SingleStorage
   * @tparam Args Is the parameter pack for the object constructors (perfect forward)
   *
   * @result SmartPtr rvalue reference
   */
  template<
           typename T,
           class Conversion = AllowConversion,
           template<typename> class Safety = NoSafety,
           template<typename> class Storage = SingleStorage,
           typename... Args
          >
  [[nodiscard]] SmartPtr<T, Conversion, Safety, Storage> makeSmartPtr(Args&&... args)
  {
    SmartPtr<T, Conversion, Safety, Storage> ptr{Storage<T>::Create(std::forward<Args>(args)...)};
    return ptr;
  }

} /* End namespace nop */

#endif /* End smart ptr header file */
