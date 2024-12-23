#ifndef __NOP_SMART_PTR_HPP__
#define __NOP_SMART_PTR_HPP__ 1

#include <type_traits>
#include <stdexcept>
#include "types.hpp"

namespace nop
{

  struct AllowConversion
  {
    enum
    {
      VOID_POINTER = true,
      BOOL = true
    };
  };

  struct DisallowConversion
  {
    enum
    {
      VOID_POINTER = false,
      BOOL = false
    };
  };

  template <typename T>
  struct SingleStorage 
  {
    static void Delete(T* pointer) noexcept
    {
      if (pointer != nullptr)
        delete pointer;
    }

    enum { OPERATOR_SUBSCRIPT = false };
  };

  template<typename T>
  struct MultipleStorage
  {
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
           class Conversion = AllowConversion,
// TODO: Implement new class-strategies -> Safety
// {template<typename> class Safety = no_check}
           template<typename> class Storage = SingleStorage
          >
  class SmartPtr
  {
  public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

  private:
    pointer m_pointer;

  public:
    SmartPtr()
      : m_pointer{nullptr}
    {}

    explicit SmartPtr(pointer ptr)
      : m_pointer{ptr}
    {}

    SmartPtr(const SmartPtr&) = delete;

    SmartPtr(SmartPtr&& other)
      : m_pointer{other.release()}
    {}

    ~SmartPtr()
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

    SmartPtr& operator=(const SmartPtr&) = delete;

    SmartPtr& operator=(SmartPtr&& other)
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

  template<
           typename T,
           class Conversion = AllowConversion,
           template<typename> class Storage = SingleStorage,
           typename... Args
          >
  [[nodiscard]] SmartPtr<T, Conversion, Storage> makeSmartPtr(Args&&... args)
  {
    SmartPtr<T, Conversion, Storage> ptr{new T(std::forward<Args>(args)...)};
    return ptr;
  }

}

#endif
