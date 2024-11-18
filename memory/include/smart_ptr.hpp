#ifndef __NOP_LIB_MEMORY_SMART_PTR__
#define __NOP_LIB_MEMORY_SMART_PTR__ 1

#include <ostream>
#include <type_traits>
#include "types.hpp"

namespace nop
{

  struct allow_conversion
  {
    enum
    {
      VOID_POINTER = true,
      BOOL = true
    };
  };

  struct disallow_conversion
  {
    enum
    {
      VOID_POINTER = false,
      BOOL = false
    };
  };

  template <typename T>
  struct single_storage 
  {
    static T* Create() noexcept
    {
      return nullptr;
    }

    static void Delete(T* pointer) noexcept
    {
      if (pointer != nullptr)
        delete pointer;
    }

    enum { OPERATOR_SUBSCRIPT = false };
  };

  template<typename T>
  struct multiple_storage
  {
    static T* Create(nop::size_t size = 0)
    {
      if (size > 0)
        return new T[size];
      else
        return nullptr;
    }

    static void Delete(T* pointer) noexcept
    {
      if (pointer != nullptr)
        delete[] pointer;
    }

    enum { OPERATOR_SUBSCRIPT = true };
  };

  template<
           typename T,
// TODO: Implement new class-strategies -> Ownership
// {template<typename> class Ownership = unique_ownership}
           class Conversion = allow_conversion,
// TODO: Implement new class-strategies -> Safety
// {template<typename> class Safety = no_check}
           template <typename> class Storage = single_storage
          >
  class smart_ptr
  {
  public:
    using size_type = nop::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

  private:
    pointer m_pointer;

  public:
    smart_ptr()
      : m_pointer(nullptr)
    {}

    explicit smart_ptr(pointer ptr)
      : m_pointer(ptr)
    {}

    smart_ptr(const smart_ptr&) = delete;
    smart_ptr(smart_ptr&&) = delete;

    ~smart_ptr()
    {
      Storage<T>::Delete(m_pointer);
    }

    [[nodiscard]] pointer get() noexcept
    {
      return m_pointer;
    }

    [[nodiscard]] const_pointer get() const noexcept
    {
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

    [[nodiscard]] reference operator*() noexcept
    {
      return *m_pointer;
    }

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return *m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] std::enable_if_t<std::is_class_v<U>, T>* operator->() noexcept
    {
      return m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] const std::enable_if_t<std::is_class_v<U>, T>* operator->() const noexcept
    {
      return m_pointer;
    }

    template<typename U = T>
    [[nodiscard]] std::enable_if_t<Storage<U>::OPERATOR_SUBSCRIPT == true, T>& operator[](size_type index) noexcept
    {
      return m_pointer[index];
    }

    template<typename U = T>
    [[nodiscard]] const std::enable_if_t<Storage<U>::OPERATOR_SUBSCRIPT == true, T>& operator[](size_type index) const noexcept
    {
      return m_pointer[index];
    }

    smart_ptr& operator=(const smart_ptr&) = delete;
    smart_ptr& operator=(smart_ptr&&) = delete;

    template<typename U = void*>
    [[nodiscard]] operator std::enable_if_t<Conversion::VOID_POINTER == true, U>() noexcept
    {
      return m_pointer;
    }

    template<typename U = bool>
    [[nodiscard]] operator std::enable_if_t<Conversion::BOOL == true, U>() const noexcept
    {
      return m_pointer != nullptr;
    }

    [[nodiscard]] bool operator!() const noexcept
    {
      return !m_pointer;
    }

    [[nodiscard]] inline friend bool operator==(const smart_ptr& s_ptr, pointer r_ptr)
    {
      return s_ptr.m_pointer == r_ptr;
    }
    
    [[nodiscard]] inline friend bool operator==(const_pointer r_ptr, const smart_ptr& s_ptr)
    {
      return r_ptr == s_ptr.m_pointer;
    }

    [[nodiscard]] inline friend bool operator!=(const smart_ptr& s_ptr, pointer r_ptr)
    {
      return s_ptr.m_pointer != r_ptr;
    }

    [[nodiscard]] inline friend bool operator!=(pointer r_ptr, const smart_ptr& s_ptr)
    {
      return r_ptr != s_ptr.m_pointer;
    }

  };

}

#endif
