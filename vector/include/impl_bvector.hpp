#ifndef NOP_IMPL_VECTOR_BOOL_HPP /* Begin implementation vector bool header file */
#define NOP_IMPL_VECTOR_BOOL_HPP 1


#if !defined(__cplusplus)
  #error CXX compiler required
#endif

#include <iterator> /* iterator traits */
#include <memory> /* std::allocator<T> */
#include <bitset> /* std::bitset<N> */

#include "vector_macro_definitions.hpp"
#include "vector_bool_extension.hpp"
#include "exception.hpp"
#include "types.hpp"
#include "attributes.hpp"
#include "utils.hpp"

namespace nop /* Begin namespace nop */
{

  template<__NOP_TYPENAME__ T, class Alloc = std::allocator<T>>
  class __NOP_VECTOR__;

namespace vectorLimits /* Begin namespace vectorLimits */
{

#if __WORDSIZE == 64UL

  #if !defined(__NOP_DISABLE_WARNINGS__)
  #error Library "NOP" : vector class requires explicit allocator template parameter.
  #error Library "NOP" : To disable this message provide : __NOP_DISABLE_WARNINGS__
  #endif

  #if __cplusplus >= 201103L
    extern constexpr size_t MAX_SIZE{0x100000000UL};
    extern constexpr size_t MAX_CAPACITY{0x100000000UL / sizeof(size_t)};
    extern constexpr size_t MID_CAPACITY{MAX_CAPACITY >> 1UL};
  #else
    #define MAX_SIZE (0x2000000000UL)
    #define MAX_CAPACITY (0x400000000UL)
    #define MID_CAPACITY (0x100000000UL)
  #endif
#else
  #error Library "NOP" : vector class does not support 32 bit mode (or less)
  #if __cplusplus >= 201103L
    extern constexpr size_t MAX_SIZE{0x20000000UL};
    extern constexpr size_t MAX_CAPACITY{0x4000000UL};
    extern constexpr size_t MID_CAPACITY{0x1000000UL};
  #else
    #define MAX_SIZE (0x20000000UL)
    #define MAX_CAPACITY (0x4000000UL)
    #define MID_CAPACITY (0x1000000UL)
  #endif
#endif

} /* End namespace vectorLimits */

  template<class Alloc>
  class __NOP_VECTOR__<bool, Alloc>
  {
  public:
    class __NOP_VECTOR_ITERATOR__;
    class __NOP_VECTOR_CONST_ITERATOR__;
  public:
#if __cplusplus >= 201103L
    using __NOP_VECTOR_VALUE_TYPE__ = bool;
    using __NOP_VECTOR_ALLOCATOR_TYPE__ = Alloc;
    using __NOP_VECTOR_SIZE_TYPE__ = size_t;
    using __NOP_VECTOR_DIFFERENCE_TYPE__ = ptrdiff_t;
    using __NOP_VECTOR_REFERENCE__ = __NOP_VECTOR_VALUE_TYPE__&;
    using __NOP_VECTOR_CONST_REFERENCE__ = const __NOP_VECTOR_VALUE_TYPE__&;
    using __NOP_VECTOR_POINTER__ = size_t*;
    using __NOP_VECTOR_CONST_POINTER__ = const size_t*;
    using bitState = bool;
    using storageState = bool;
    using bitString = std::string;
#else
    typedef bool __NOP_VECTOR_VALUE_TYPE__;
    typedef __NOP_VECTOR_ALLOCATOR_TYPE__ Alloc;
    typedef size_t __NOP_VECTOR_SIZE_TYPE__;
    typedef ptrdiff_t __NOP_VECTOR_DIFFERENCE_TYPE__;
    typedef __NOP_VECTOR_VALUE_TYPE__& __NOP_VECTOR_REFERENCE__;
    typedef const __NOP_VECTOR_VALUE_TYPE__& __NOP_VECTOR_CONST_REFERENCE__;
    typedef size_t* __NOP_VECTOR_POINTER__;
    typedef const __NOP_VECTOR_POINTER__ __NOP_VECTOR_CONST_POINTER__;
    typedef bool bitState;
    typedef bool storageState;
    typedef std::string bitString;
#endif
    private:
    enum BitMask : __NOP_VECTOR_SIZE_TYPE__
    {
      Bit = 1UL,
      Reset = 0UL,
      Set = (0xffffffffUL << (__WORDSIZE >> 1UL)) | 0xffffffffUL
    };

  public:
    class alignas(sizeof(__NOP_VECTOR_POINTER__) + sizeof(__NOP_VECTOR_SIZE_TYPE__)) __NOP_VECTOR_ITERATOR__
    {
      friend __NOP_VECTOR__;
    public:
#if __cplusplus >= 201103L
      using __NOP_VECTOR_DIFFERENCE_TYPE__ = __NOP_VECTOR__::__NOP_VECTOR_DIFFERENCE_TYPE__;
      using __NOP_VECTOR_VALUE_TYPE__ = __NOP_VECTOR__::__NOP_VECTOR_VALUE_TYPE__;
      using __NOP_VECTOR_POINTER__ = __NOP_VECTOR__::__NOP_VECTOR_POINTER__;
      using __NOP_VECTOR_REFERENCE__ = __NOP_VECTOR__::__NOP_VECTOR_REFERENCE__;
      using iteratorCategory = std::random_access_iterator_tag;
      using iterator_category = std::random_access_iterator_tag; /* For standard library compatibility */
  #if __cplusplus >= 202002L
      using iteratorConcept = std::contiguous_iterator_tag;
      using iterator_concept = std::contiguous_iterator_tag; /* For standard library compatibility */
  #endif
#else
      typedef ptrdiff_t __NOP_VECTOR_DIFFERENCE_TYPE__;
      typedef bool __NOP_VECTOR_VALUE_TYPE__;
      typedef size_t* __NOP_VECTOR_POINTER__;
      typedef size_t& __NOP_VECTOR_REFERENCE__;
      typedef std::random_access_iterator_tag iteratorCategory;
      typedef std::random_access_iterator_tag iterator_category; /* For standard library compatibility */
#endif
    private:
      class alignas(sizeof(__NOP_VECTOR_POINTER__) + sizeof(__NOP_VECTOR_DIFFERENCE_TYPE__)) __NOP_VECTOR_PROXY_ITERATOR__
      {
        friend __NOP_VECTOR_ITERATOR__;
      private:
        __NOP_VECTOR_POINTER__ m_byte;
        __NOP_VECTOR_DIFFERENCE_TYPE__ m_bit;

      public:
        __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
        __NOP_VECTOR_PROXY_ITERATOR__(__NOP_VECTOR_POINTER__ ptr, __NOP_VECTOR_DIFFERENCE_TYPE__ bitPosition) __NOP_ATTRIBUTE_NOEXCEPT__
          : m_byte(ptr)
          , m_bit(bitPosition)
        {}
#if __cplusplus >= 201103L
        __NOP_VECTOR_PROXY_ITERATOR__(const __NOP_VECTOR_PROXY_ITERATOR__&) = default;
        __NOP_VECTOR_PROXY_ITERATOR__(__NOP_VECTOR_PROXY_ITERATOR__&&) = default;
        ~__NOP_VECTOR_PROXY_ITERATOR__() = default;
#else
        __NOP_VECTOR_PROXY_ITERATOR__(const __NOP_VECTOR_PROXY_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
          : m_byte(other.m_byte)
          , m_byte(other.m_bit)
        {}
#endif
        __NOP_ATTRIBUTE_ALWAYS_INLINE__
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        __NOP_VECTOR_PROXY_ITERATOR__& operator++() __NOP_ATTRIBUTE_NOEXCEPT__
        {
          ++m_bit;
          return *this;
        }

        __NOP_ATTRIBUTE_ALWAYS_INLINE__
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        __NOP_VECTOR_PROXY_ITERATOR__& operator--() __NOP_ATTRIBUTE_NOEXCEPT__
        {
          --m_bit;
          return *this;
        }

        __NOP_ATTRIBUTE_ALWAYS_INLINE__
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        void swap(__NOP_VECTOR_PROXY_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
        {
          std::swap(m_byte, other.m_byte);
          std::swap(m_bit, other.m_bit);
        }

        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        __NOP_VECTOR_PROXY_ITERATOR__& operator=(bitState value) __NOP_ATTRIBUTE_NOEXCEPT__
        {
          if (value == BIT_SET)
          {
            m_byte[byteDivision(static_cast<__NOP_VECTOR_SIZE_TYPE__>(m_bit), UL)] |= __NOP_VECTOR__::BitMask::Bit << byteModule(static_cast<__NOP_VECTOR_SIZE_TYPE__>(m_bit), UL);
          }
          else
          {
            m_byte[byteDivision(static_cast<__NOP_VECTOR_SIZE_TYPE__>(m_bit), UL)] &= ~(__NOP_VECTOR__::BitMask::Bit << byteModule(static_cast<__NOP_VECTOR_SIZE_TYPE__>(m_bit), UL));
          }

          return *this;
        }

        __NOP_ATTRIBUTE_NODISCARD__
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        bitState getBit() const __NOP_ATTRIBUTE_NOEXCEPT__
        {
          return m_byte[byteDivision(static_cast<__NOP_VECTOR_SIZE_TYPE__>(m_bit), UL)] & __NOP_VECTOR__::BitMask::Bit << byteModule(static_cast<__NOP_VECTOR_SIZE_TYPE__>(m_bit), UL);
        }

#if __cplusplus >= 201103L
        __NOP_VECTOR_PROXY_ITERATOR__& operator=(const __NOP_VECTOR_PROXY_ITERATOR__&) = default;
        __NOP_VECTOR_PROXY_ITERATOR__& operator=(__NOP_VECTOR_PROXY_ITERATOR__&&) = default;
#else
        __NOP_VECTOR_PROXY_ITERATOR__& operator=(const __NOP_VECTOR_PROXY_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
        {
          m_byte = other.m_byte;
          m_bit = other.m_bit;
          return *this;
        }
#endif
        __NOP_ATTRIBUTE_NODISCARD__
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        bool operator==(const __NOP_VECTOR_PROXY_ITERATOR__& other) const __NOP_ATTRIBUTE_NOEXCEPT__
        {
          if (m_byte == __NOP_NULLPTR__ || other.m_byte == __NOP_NULLPTR__)
          {
            return false;
          }
          else
          {
            return getBit() == other.getBit();
          }
        }

        __NOP_ATTRIBUTE_NODISCARD__
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        bool operator!=(const __NOP_VECTOR_PROXY_ITERATOR__& other) const __NOP_ATTRIBUTE_NOEXCEPT__
        {
          return !(*this == other);
        }

        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        operator bool() const __NOP_ATTRIBUTE_NOEXCEPT__
        {
          return m_byte == __NOP_NULLPTR__ ? false : getBit();
        }

#if __cplusplus >= 201103L
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        explicit
#endif
        operator char() const __NOP_ATTRIBUTE_NOEXCEPT__
        {
          return m_byte == __NOP_NULLPTR__ ? '0' : getBit() + '0';
        }

      };

    private:
      __NOP_VECTOR_PROXY_ITERATOR__ m_iter;

    public:
      __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__(__NOP_VECTOR_POINTER__ ptr, __NOP_VECTOR_DIFFERENCE_TYPE__ bitPosition) __NOP_ATTRIBUTE_NOEXCEPT__
        : m_iter(ptr, bitPosition)
      {}
#if __cplusplus >= 201103L
      __NOP_VECTOR_ITERATOR__(const __NOP_VECTOR_ITERATOR__&) = default;
      __NOP_VECTOR_ITERATOR__(__NOP_VECTOR_ITERATOR__&&) = default;
      ~__NOP_VECTOR_ITERATOR__() = default;
#else
      __NOP_VECTOR_ITERATOR__(const __NOP_VECTOR_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
        : m_iter(other.ptr, other.bitPos)
      {}
#endif

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      void swap(__NOP_VECTOR_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_iter.swap(other.m_iter);
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__& operator++() __NOP_ATTRIBUTE_NOEXCEPT__
      {
        ++m_iter;
        return *this;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__ operator++(i32) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(*this);
        ++m_iter;
        return temp__NOP_VECTOR_ITERATOR__;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__& operator+=(__NOP_VECTOR_DIFFERENCE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_iter.m_bit += index;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__ operator+(__NOP_VECTOR_DIFFERENCE_TYPE__ index) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(*this);
        temp__NOP_VECTOR_ITERATOR__ += index;
        return temp__NOP_VECTOR_ITERATOR__;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend __NOP_VECTOR_ITERATOR__ operator+(__NOP_VECTOR_DIFFERENCE_TYPE__ index, const __NOP_VECTOR_ITERATOR__& iter) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(iter);
        temp__NOP_VECTOR_ITERATOR__ += index;
        return temp__NOP_VECTOR_ITERATOR__;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__& operator--() __NOP_ATTRIBUTE_NOEXCEPT__
      {
        --m_iter;
        return *this;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__ operator--(i32) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(*this);
        --m_iter;
        return temp__NOP_VECTOR_ITERATOR__;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__& operator-=(__NOP_VECTOR_DIFFERENCE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_iter.m_bit -= index;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_ITERATOR__ operator-(__NOP_VECTOR_DIFFERENCE_TYPE__ index) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(*this);
        temp__NOP_VECTOR_ITERATOR__ -= index;
        return temp__NOP_VECTOR_ITERATOR__;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend __NOP_VECTOR_ITERATOR__ operator-(__NOP_VECTOR_DIFFERENCE_TYPE__ index, const __NOP_VECTOR_ITERATOR__& iter) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(iter);
        temp__NOP_VECTOR_ITERATOR__ -= index;
        return temp__NOP_VECTOR_ITERATOR__;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_DIFFERENCE_TYPE__ operator-(const __NOP_VECTOR_ITERATOR__& other) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_iter.m_bit - other.m_iter.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator<(const __NOP_VECTOR_ITERATOR__& lhs, const __NOP_VECTOR_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        if (lhs.m_iter.m_bit < rhs.m_iter.m_bit)
        {
          return true;
        }
        return false;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator<=(const __NOP_VECTOR_ITERATOR__& lhs, const __NOP_VECTOR_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        if (lhs.m_iter.m_bit <= rhs.m_iter.m_bit)
        {
          return true;
        }
        return false;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator>(const __NOP_VECTOR_ITERATOR__& lhs, const __NOP_VECTOR_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        if (lhs.m_iter.m_bit > rhs.m_iter.m_bit)
        {
          return true;
        }
        return false;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator>=(const __NOP_VECTOR_ITERATOR__& lhs, const __NOP_VECTOR_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        if (lhs.m_iter.m_bit >= rhs.m_iter.m_bit)
        {
          return true;
        }
        return false;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator==(const __NOP_VECTOR_ITERATOR__& lhs, const __NOP_VECTOR_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_iter.m_bit == rhs.m_iter.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator!=(const __NOP_VECTOR_ITERATOR__& lhs, const __NOP_VECTOR_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_iter.m_bit != rhs.m_iter.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_PROXY_ITERATOR__& operator*() __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_iter;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_PROXY_ITERATOR__ operator[](__NOP_VECTOR_DIFFERENCE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return __NOP_VECTOR_PROXY_ITERATOR__(m_iter.m_byte, index);
      }

#if __cplusplus >= 201103L
      __NOP_VECTOR_ITERATOR__& operator=(const __NOP_VECTOR_ITERATOR__&) = default;
      __NOP_VECTOR_ITERATOR__& operator=(__NOP_VECTOR_ITERATOR__&&) = default;
#else
      __NOP_VECTOR_ITERATOR__& operator(const __NOP_VECTOR_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_iter = other.m_iter;
        return *this;
      }
#endif

    };

    class alignas(sizeof(__NOP_VECTOR_POINTER__) + sizeof(__NOP_VECTOR_SIZE_TYPE__)) __NOP_VECTOR_CONST_ITERATOR__
    {
    public:
#if __cplusplus >= 201103L
      using __NOP_VECTOR_DIFFERENCE_TYPE__ = __NOP_VECTOR__::__NOP_VECTOR_DIFFERENCE_TYPE__;
      using __NOP_VECTOR_VALUE_TYPE__ = __NOP_VECTOR__::__NOP_VECTOR_VALUE_TYPE__;
      using __NOP_VECTOR_POINTER__ = __NOP_VECTOR__::__NOP_VECTOR_CONST_POINTER__;
      using __NOP_VECTOR_REFERENCE__ = __NOP_VECTOR__::__NOP_VECTOR_VALUE_TYPE__;
      using iteratorCategory = std::random_access_iterator_tag;
      using iterator_category = std::random_access_iterator_tag; /* For standard library compatibility */
  #if __cplusplus >= 202002L
      using iteratorConcept = std::contiguous_iterator_tag;
      using iterator_concept = std::contiguous_iterator_tag; /* For standard library compatibility */
  #endif
#else
      typedef ptrdiff_t __NOP_VECTOR_DIFFERENCE_TYPE__;
      typedef ptrdiff_t difference_type; /* For standard library compatibility */
      typedef bool __NOP_VECTOR_VALUE_TYPE__;
      typedef bool value_type; /* For standard library compatibility */
      typedef __NOP_VECTOR__::__NOP_VECTOR_POINTER__ __NOP_VECTOR_POINTER__;
      typedef __NOP_VECTOR__::__NOP_VECTOR_VALUE_TYPE__ __NOP_VECTOR_REFERENCE__;
      typedef std::random_access_iterator_tag iteratorCategory;
      typedef std::random_access_iterator_tag iterator_category; /* For standard library compatibility */
#endif

    private:
      __NOP_VECTOR_POINTER__ m_byte;
      __NOP_VECTOR_DIFFERENCE_TYPE__ m_bit;

    public:
      __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__(__NOP_VECTOR_POINTER__ ptr, __NOP_VECTOR_DIFFERENCE_TYPE__ bitPosition) __NOP_ATTRIBUTE_NOEXCEPT__
        : m_byte(ptr)
        , m_bit(bitPosition)
      {}
#if __cplusplus >= 201103L
      __NOP_VECTOR_CONST_ITERATOR__(const __NOP_VECTOR_CONST_ITERATOR__&) = default;
      __NOP_VECTOR_CONST_ITERATOR__(__NOP_VECTOR_CONST_ITERATOR__&&) = default;
      ~__NOP_VECTOR_CONST_ITERATOR__() = default;
#else
      __NOP_VECTOR_CONST_ITERATOR__(const __NOP_VECTOR_CONST_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
        : m_byte(other.m_byte)
        , m_bit(other.m_bit)
      {}
#endif
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__& operator++() __NOP_ATTRIBUTE_NOEXCEPT__
      {
        ++m_bit;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__ operator++(i32) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(*this);
        ++m_bit;
        return *this;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__& operator+=(__NOP_VECTOR_DIFFERENCE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_bit += index;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__ operator+(__NOP_VECTOR_DIFFERENCE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(*this);
        m_bits += index;
        return temp__NOP_VECTOR_ITERATOR__;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend __NOP_VECTOR_CONST_ITERATOR__ operator+(__NOP_VECTOR_DIFFERENCE_TYPE__ index, const __NOP_VECTOR_CONST_ITERATOR__& iter) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return __NOP_VECTOR_CONST_ITERATOR__(iter.m_byte, iter.m_bit + index);
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__& operator--() __NOP_ATTRIBUTE_NOEXCEPT__
      {
         --m_bit;
         return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__ operator--(i32) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(*this);
        --m_bit;
        return *this;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__& operator-=(__NOP_VECTOR_DIFFERENCE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_bit -= index;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_CONST_ITERATOR__ operator-(__NOP_VECTOR_DIFFERENCE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto temp__NOP_VECTOR_ITERATOR__(*this);
        temp__NOP_VECTOR_ITERATOR__ -= index;
        return temp__NOP_VECTOR_ITERATOR__;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend __NOP_VECTOR_CONST_ITERATOR__ operator-(__NOP_VECTOR_DIFFERENCE_TYPE__ index, const __NOP_VECTOR_CONST_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return __NOP_VECTOR_CONST_ITERATOR__(other.m_byte, other.m_bit - index);
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_DIFFERENCE_TYPE__ operator-(const __NOP_VECTOR_CONST_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_bit - other.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator>(const __NOP_VECTOR_CONST_ITERATOR__& lhs, const __NOP_VECTOR_CONST_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_bit > rhs.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator>=(const __NOP_VECTOR_CONST_ITERATOR__& lhs, const __NOP_VECTOR_CONST_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_bit >= rhs.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator<(const __NOP_VECTOR_CONST_ITERATOR__& lhs, const __NOP_VECTOR_CONST_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_bit < rhs.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator<=(const __NOP_VECTOR_CONST_ITERATOR__& lhs, const __NOP_VECTOR_CONST_ITERATOR__& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_bit <= rhs.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      bool operator==(const __NOP_VECTOR_CONST_ITERATOR__& other) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_bit == other.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      bool operator!=(const __NOP_VECTOR_CONST_ITERATOR__& other) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_bit != other.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      bitState operator*() const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_byte[byteDivision(m_bit, UL)] & __NOP_VECTOR__::BitMask::Bit << byteModule(m_bit, UL);
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      __NOP_VECTOR_REFERENCE__ operator[](__NOP_VECTOR_DIFFERENCE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_byte[byteDivision(index, UL)] & __NOP_VECTOR__::BitMask::Bit << byteModule(index, UL);
      }

#if __cplusplus >= 201103L
      __NOP_VECTOR_CONST_ITERATOR__& operator=(const __NOP_VECTOR_CONST_ITERATOR__&) = default;
      __NOP_VECTOR_CONST_ITERATOR__& operator=(__NOP_VECTOR_CONST_ITERATOR__&&) = default;
#else
      __NOP_VECTOR_CONST_ITERATOR__& operator=(const __NOP_VECTOR_CONST_ITERATOR__& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_byte = other.m_byte;
        m_bit = other.m_bit;
        return *this;
      }
#endif

    };

  private:
    __NOP_ATTRIBUTE_NO_UNIQUE_ADDRESS__ __NOP_VECTOR_ALLOCATOR_TYPE__ xmalloc;
    __NOP_VECTOR_POINTER__ m_storage;
    __NOP_VECTOR_SIZE_TYPE__ m_bits;
    __NOP_VECTOR_SIZE_TYPE__ m_bytes;

  private:
    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_SIZE_TYPE__ calculateCapacity(__NOP_VECTOR_SIZE_TYPE__ bitsNumber) const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return byteDivision(bitsNumber, UL) + (byteModule(bitsNumber, UL) ? 1UL : 0UL);
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void setBit(__NOP_VECTOR_SIZE_TYPE__ index, bitState value) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      if (value == BIT_SET)
      {
        m_storage[byteDivision(index, UL)] |= BitMask::Bit << byteModule(index, UL);
      }
      else
      {
        m_storage[byteDivision(index, UL)] &= ~(BitMask::Bit << byteModule(index, UL));
      }
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bool resizeFactor() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return byteDivision(m_bits, UL) == m_bytes;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void copyData(__NOP_VECTOR_POINTER__ source, __NOP_VECTOR_POINTER__ destination, __NOP_VECTOR_SIZE_TYPE__ n) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      __NOP_VECTOR_POINTER__ end = source + n;
#if __cplusplus >= 202002L /* The support for this attribute is in progress */
      [[likely]]
#endif
      while (source < end)
      {
        *destination = *source;
        ++source;
        ++destination;
      }
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void fillData(__NOP_VECTOR_POINTER__ source, __NOP_VECTOR_SIZE_TYPE__ n, __NOP_VECTOR_SIZE_TYPE__ fillValue) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      __NOP_VECTOR_POINTER__ end = source + n;
#if __cplusplus >= 202002L /* The support for this attribute is in progress */
      [[likely]]
#endif
      while (source < end)
      {
        *source = fillValue;
        ++source;
      }
    }
#if __cplusplus >= 201103L
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void growInit(__NOP_VECTOR_SIZE_TYPE__ bytesNumber)
    {
      const __NOP_VECTOR_SIZE_TYPE__ newSize = m_bytes + bytesNumber;
      __NOP_VECTOR_POINTER__ tempPtr;

#ifdef __ALLOCATOR_BASE_EXCEPTION__
      try
      {
        tempPtr = static_cast<__NOP_VECTOR_POINTER__>(xmalloc.allocate(newSize));
      }
      catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
      {
        __NOP_THROW_EXCEPTION(err::BadAlloc, "__NOP_VECTOR__:reserve(__NOP_VECTOR_SIZE_TYPE__) -> Cannot allocate memory");
      }
#else
      tempPtr = static_cast<__NOP_VECTOR_POINTER__>(xmalloc.allocate(newSize));
#endif

      if (m_storage != __NOP_NULLPTR__)
      {
        copyData(m_storage, tempPtr, m_bytes);
        xmalloc.deallocate(m_storage, m_bytes);
      }

      fillData(tempPtr + m_bytes, newSize - m_bytes, BitMask::Reset);

      m_storage = tempPtr;
      m_bytes = newSize;
    }
#endif
  public:
    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_ITERATOR__ begin() __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return __NOP_VECTOR_ITERATOR__(m_storage, ZERO_VALUE);
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_ITERATOR__ end() __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return __NOP_VECTOR_ITERATOR__(m_storage, static_cast<__NOP_VECTOR_DIFFERENCE_TYPE__>(m_bits));
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_CONST_ITERATOR__ cbegin() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return __NOP_VECTOR_CONST_ITERATOR__(m_storage, ZERO_VALUE);
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_CONST_ITERATOR__ cend() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return __NOP_VECTOR_CONST_ITERATOR__(m_storage, m_bits);
    }

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    __NOP_VECTOR__() __NOP_ATTRIBUTE_NOEXCEPT_EXPR(__NOP_ATTRIBUTE_NOEXCEPT_EXPR(__NOP_VECTOR_ALLOCATOR_TYPE__()))
      : xmalloc(__NOP_VECTOR_ALLOCATOR_TYPE__())
      , m_storage(__NOP_NULLPTR__)
      , m_bits(ZERO_VALUE)
      , m_bytes(ZERO_VALUE)
    {}

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    explicit __NOP_VECTOR__(__attribute__((unused)) const __NOP_VECTOR_ALLOCATOR_TYPE__& allocator) __NOP_ATTRIBUTE_NOEXCEPT__
      : xmalloc(allocator)
      , m_storage(__NOP_NULLPTR__)
      , m_bits(ZERO_VALUE)
      , m_bytes(ZERO_VALUE)
    {}

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    __NOP_VECTOR__(__NOP_VECTOR_SIZE_TYPE__ bitsNumber, __NOP_VECTOR_SIZE_TYPE__ value = ZERO_VALUE, __attribute__((unused)) const __NOP_VECTOR_ALLOCATOR_TYPE__& allocator = __NOP_VECTOR_ALLOCATOR_TYPE__())
     : xmalloc(allocator)
      , m_bits(bitsNumber)
      , m_bytes(calculateCapacity(bitsNumber))
    {
      if (m_bits > vectorLimits::MAX_SIZE)
      {
        __NOP_THROW_EXCEPTION(err::LengthError, "__NOP_VECTOR__:__NOP_VECTOR__(__NOP_VECTOR_SIZE_TYPE__, __NOP_VECTOR_SIZE_TYPE__) -> invalid number of bits");
      }
      else if (bitsNumber == ZERO_VALUE)
      {
        return;
      }

#if defined(__ALLOCATOR_BASE_EXCEPTION__)
      try
      {
        m_storage = xmalloc.allocate(m_bytes);
      }
      catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
      {
        __NOP_THROW_EXCEPTION(err::LengthError, "__NOP_VECTOR__:__NOP_VECTOR__(__NOP_VECTOR_SIZE_TYPE__, __NOP_VECTOR_SIZE_TYPE__) -> Cannot allocate memory");
      }
#else
      m_storage = xmalloc.allocate(m_bytes);
#endif
      for (__NOP_VECTOR_SIZE_TYPE__ i = 0UL; i < m_bytes; ++i)
      {
        m_storage[i] = BitMask::Reset;
      }

      if (value != ZERO_VALUE)
      {
        m_storage[0UL] = value;
      }
    }

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    __NOP_VECTOR__(const __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& other)
      : xmalloc(__NOP_VECTOR_ALLOCATOR_TYPE__())
      , m_storage(__NOP_NULLPTR__)
      , m_bits(other.m_bits)
      , m_bytes(other.m_bytes)
    {
      if (m_bytes > ZERO_VALUE)
      {
#if defined(__ALLOCATOR_BASE_EXCEPTION__)
        try
        {
          m_storage = xmalloc.allocate(m_bytes);
        }
        catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
        {
          __NOP_THROW_EXCEPTION(err::BadAlloc, "__NOP_VECTOR__:__NOP_VECTOR__(const __NOP_VECTOR__&) -> Cannot allocate memory");
        }
#else
        m_storage = xmalloc.allocate(m_bytes);
#endif
        copyData(other.m_storage, m_storage, m_bytes);
      }
    }

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    __NOP_VECTOR__(const __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& other, const __NOP_VECTOR_ALLOCATOR_TYPE__& allocator)
      : xmalloc(allocator)
      , m_storage(__NOP_NULLPTR__)
      , m_bits(other.m_bits)
      , m_bytes(other.m_bytes)
    {
      if (m_bytes > ZERO_VALUE)
      {
#if defined(__ALLOCATOR_BASE_EXCEPTION__)
        try
        {
          m_storage = xmalloc.allocate(m_bytes);
        }
        catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
        {
          __NOP_THROW_EXCEPTION(err::BadAlloc, "__NOP_VECTOR__:__NOP_VECTOR__(const __NOP_VECTOR__&, const Allocator&) -> Cannot allocate memory");
        }
#else
        m_storage = xmalloc.allocate(m_bytes);
#endif
        copyData(other.m_storage, m_storage, m_bytes);
      }
    }

#if __cplusplus >= 201103L
    __NOP_ATTRIBUTE_MCTOR_CONSTEXPR__
    __NOP_VECTOR__(__NOP_VECTOR__&& other) __NOP_ATTRIBUTE_NOEXCEPT_EXPR(__NOP_ATTRIBUTE_NOEXCEPT_EXPR(__NOP_VECTOR_ALLOCATOR_TYPE__{}))
      : xmalloc{__NOP_VECTOR_ALLOCATOR_TYPE__{}}
      , m_storage{other.m_storage}
      , m_bits{other.m_bits}
      , m_bytes{other.m_bytes}
    {
      other.m_storage = __NOP_NULLPTR__;
      other.m_bits = other.m_bytes = ZERO_VALUE;
    }
#endif

#if __cplusplus >= 201402L
    __NOP_ATTRIBUTE_MCTOR_CONSTEXPR__
    __NOP_VECTOR__(__NOP_VECTOR__&& other, const __NOP_VECTOR_ALLOCATOR_TYPE__& allocator) __NOP_ATTRIBUTE_NOEXCEPT__
      : xmalloc{allocator}
      , m_storage{other.m_storage}
      , m_bits{other.m_bits}
      , m_bytes{other.m_bytes}
    {
      other.m_storage = __NOP_NULLPTR__;
      other.m_bits = other.m_bytes = ZERO_VALUE;
    }
#endif

    __NOP_ATTRIBUTE_DTOR_CONSTEXPR__
    ~__NOP_VECTOR__()
    {
      if (m_storage != __NOP_NULLPTR__)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = __NOP_NULLPTR__;
      }
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_SIZE_TYPE__ size() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_bits;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_SIZE_TYPE__ capacity() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_bytes << 6UL;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_SIZE_TYPE__ __NOP_VECTOR_MAX_SIZE__() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return vectorLimits::MAX_SIZE;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_POINTER__ data() __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_storage;
    }

#if __cplusplus >= 201402L
    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_CONST_POINTER__ data() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_storage;
    }
#endif

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_ALLOCATOR_TYPE__ __NOP_VECTOR_GET_ALLOCATOR__() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return xmalloc;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR_SIZE_TYPE__ count() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        return ZERO_VALUE;
      }

      __NOP_VECTOR_POINTER__ end = m_storage + calculateCapacity(m_bits) - 1UL;
      __NOP_VECTOR_SIZE_TYPE__ bitCount = 0UL;

      for (__NOP_VECTOR_POINTER__ begin = m_storage; begin < end; ++begin)
      {
        bitCount += std::bitset<__WORDSIZE>(*begin).count(); /* Works for now */
      }

      const __NOP_VECTOR_SIZE_TYPE__ remainingBits = byteModule(m_bits, UL) ? byteModule(m_bits, UL) : __WORDSIZE;

      for (__NOP_VECTOR_SIZE_TYPE__ currentBit = 0UL; currentBit < remainingBits; ++currentBit)
      {
        if (*end & BitMask::Bit << currentBit)
        {
          ++bitCount;
        }
      }

      return bitCount;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void __NOP_VECTOR_SHRINK_TO_FIT__()
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        return;
      }
      else if (m_bits == ZERO_VALUE)
      {
        clear();
        return;
      }

      const __NOP_VECTOR_SIZE_TYPE__ currentBytes = calculateCapacity(m_bits);

      if (currentBytes < m_bytes)
      {
        __NOP_VECTOR_POINTER__ tempPtr;

#ifdef __ALLOCATOR_BASE_EXCEPTION__
        try
        {
          tempPtr = static_cast<__NOP_VECTOR_POINTER__>(xmalloc.allocate(currentBytes));
        }
        catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
        {
          __NOP_THROW_EXCEPTION(err::BadAlloc, "__NOP_VECTOR__:shrinkToFit() -> Cannot allocate memory");
        }
#else
        tempPtr = static_cast<__NOP_VECTOR_POINTER__>(xmalloc.allocate(currentBytes));
#endif
        copyData(m_storage, tempPtr, m_bytes);
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = tempPtr;
        m_bytes = currentBytes;
      }
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    storageState any() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      if (m_storage == __NOP_NULLPTR__ || m_bits == ZERO_VALUE)
      {
        return !NONE_SET;
      }
      __NOP_VECTOR_POINTER__ end = m_storage + calculateCapacity(m_bits) - 1UL;

      for (__NOP_VECTOR_POINTER__ begin = m_storage; begin != end; ++begin)
      {
        if (*begin)
        {
          return ANY_SET;
        }
      }
      const __NOP_VECTOR_SIZE_TYPE__ remainingBits = byteModule(m_bits, UL) ? byteModule(m_bits, UL) : __WORDSIZE;

      for (__NOP_VECTOR_SIZE_TYPE__ currentBit = 0UL; currentBit != remainingBits; ++currentBit)
      {
        if (*end & BitMask::Bit >> currentBit)
        {
          return ANY_SET;
        }
      }

      return !NONE_SET;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    storageState none() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      if (m_storage == __NOP_NULLPTR__ || m_bits == ZERO_VALUE)
      {
        return NONE_SET;
      }
      __NOP_VECTOR_POINTER__ end = m_storage + calculateCapacity(m_bits) - 1UL;

      for (__NOP_VECTOR_POINTER__ begin = m_storage; begin != end; ++begin)
      {
        if (*begin)
        {
          return !ANY_SET;
        }
      }
      const __NOP_VECTOR_SIZE_TYPE__ remainingBits = byteModule(m_bits, UL) ? byteModule(m_bits, UL) : __WORDSIZE;

      for (__NOP_VECTOR_SIZE_TYPE__ currentBit = 0UL; currentBit != remainingBits; ++currentBit)
      {
        if (*end & __NOP_VECTOR__::BitMask::Bit >> currentBit)
        {
          return !ANY_SET;
        }
      }

      return NONE_SET;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    storageState empty() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_bits == ZERO_VALUE;
    }

    // Modifiers
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void clear() __NOP_ATTRIBUTE_NOEXCEPT__
    {
      if (m_storage != __NOP_NULLPTR__)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = __NOP_NULLPTR__;
        m_bits = m_bytes = ZERO_VALUE;
      }
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void resize(__NOP_VECTOR_SIZE_TYPE__ bitsNumber, bitState value = BIT_UNSET)
    {
      if (bitsNumber == ZERO_VALUE)
      {
        clear();
        return;
      }
      else if (bitsNumber == m_bits)
      {
        return;
      }

      const __NOP_VECTOR_SIZE_TYPE__ newSize = calculateCapacity(bitsNumber);

      if (m_bytes < newSize)
      {
        __NOP_VECTOR_POINTER__ tempPtr;

#ifdef __ALLOCATOR_BASE_EXCEPTION__
        try
        {
          tempPtr = static_cast<__NOP_VECTOR_POINTER__>(xmalloc.allocate(newSize));
        }
        catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
        {
          __NOP_THROW_EXCEPTION(err::BadAlloc, "__NOP_VECTOR__:resize(__NOP_VECTOR_SIZE_TYPE__, bitState) -> Cannot allocate memory");
        }
#else
        tempPtr = static_cast<__NOP_VECTOR_POINTER__>(xmalloc.allocate(newSize));
#endif

        if (m_storage != __NOP_NULLPTR__)
        {
          copyData(m_storage, tempPtr, m_bytes);
          xmalloc.deallocate(m_storage, m_bytes);
        }

        m_storage = tempPtr;

        fillData(m_storage + m_bytes,
                 newSize - m_bytes,
                 value == BIT_SET ? BitMask::Set : BitMask::Reset);
        m_bytes = newSize;
        m_bits = bitsNumber;

        return;
      }

      while (m_bits < bitsNumber)
      {
        setBit(m_bits, value);
        ++m_bits;
      }
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void reserve(__NOP_VECTOR_SIZE_TYPE__ bytesNumber)
    {
      const __NOP_VECTOR_SIZE_TYPE__ newSize = m_bytes + bytesNumber;

      if (newSize > vectorLimits::MAX_CAPACITY)
      {
        __NOP_THROW_EXCEPTION(err::LengthError, "__NOP_VECTOR__:reserve() -> invalid number of bytes");
      }
      else if (bytesNumber == ZERO_VALUE ||
               m_bytes == vectorLimits::MAX_CAPACITY)
      {
        return;
      }

      __NOP_VECTOR_POINTER__ tempPtr;

#ifdef __ALLOCATOR_BASE_EXCEPTION__
      try
      {
        tempPtr = static_cast<__NOP_VECTOR_POINTER__>(xmalloc.allocate(newSize));
      }
      catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
      {
        __NOP_THROW_EXCEPTION(err::BadAlloc, "__NOP_VECTOR__:reserve(__NOP_VECTOR_SIZE_TYPE__) -> Cannot allocate memory");
      }
#else
      tempPtr = static_cast<__NOP_VECTOR_POINTER__>(xmalloc.allocate(newSize));
#endif

      if (m_storage != __NOP_NULLPTR__)
      {
        copyData(m_storage, tempPtr, m_bytes);
        xmalloc.deallocate(m_storage, m_bytes);
      }

      m_storage = tempPtr;
      m_bytes = newSize;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void __NOP_VECTOR_PUSH_BACK__(bitState value)
    {
      if (ZERO_VALUE < m_bytes && m_bytes < vectorLimits::MAX_CAPACITY)
      {
        if (resizeFactor())
        {
          __NOP_VECTOR_SIZE_TYPE__ additionSize;

          if (m_bytes < vectorLimits::MID_CAPACITY)
          {
            additionSize = m_bytes;
          }
          else if (!(m_bytes + (additionSize = m_bytes >> 1UL) < vectorLimits::MAX_CAPACITY))
          {
            additionSize = vectorLimits::MAX_CAPACITY - m_bytes;
          }
#if __cplusplus >= 201103L
          growInit(additionSize);
#else
          reserve(additionSize);
#endif
        }

        setBit(m_bits, value);
        ++m_bits;
      }
      else if (m_bytes > ZERO_VALUE)
      {
        if (m_bits != vectorLimits::MAX_SIZE)
        {
          setBit(m_bits, value);
          ++m_bits;
        }
        else
        {
          setBit(m_bits - 1UL, value);
        }
      }
      else
      {
#if __cplusplus >= 201103L
        growInit(1UL);
#else
        reserve(1UL);
#endif
        setBit(0UL, value);
        ++m_bits;
      }
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void __NOP_VECTOR_POP_BACK__()
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:popBack() -> invalid number of bits");
      }
      --m_bits;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& set(__NOP_VECTOR_SIZE_TYPE__ index, bitState value = BIT_UNSET)
    {
      if (index >= m_bits)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:set(__NOP_VECTOR_SIZE_TYPE__, bitState) -> index is out of range");
      }

      setBit(index, value);
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& set()
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:set() -> invalid number of bits");
      }

      fillData(m_storage, calculateCapacity(m_bits), BitMask::Set);
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& reset(__NOP_VECTOR_SIZE_TYPE__ index)
    {
      if (index >= m_bits)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:reset(__NOP_VECTOR_SIZE_TYPE__) -> index is out of range");
      }

      m_storage[byteDivision(index, UL)] &= ~(BitMask::Bit << byteModule(index, UL));
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& reset()
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:reset() -> invalid number of bits");
      }

      fillData(m_storage, calculateCapacity(m_bits), BitMask::Reset);
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& flip(__NOP_VECTOR_SIZE_TYPE__ index)
    {
      if (index >= m_bits)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:flip(__NOP_VECTOR_SIZE_TYPE__) -> index is out of range");
      }

      m_storage[byteDivision(index, UL)] ^= BitMask::Bit << byteModule(index, UL);
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& flip()
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:flip() -> invalid number of bits");
      }

      __NOP_VECTOR_POINTER__ end = m_storage + calculateCapacity(m_bits);

      for (__NOP_VECTOR_POINTER__ begin = m_storage; begin != end; ++begin)
      {
        *begin ^= BitMask::Set;
      }

      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void swap(__NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& other) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      if (this == &other)
      {
        return;
      }

      std::swap(m_storage, other.m_storage);
      std::swap(m_bits, other.m_bits);
      std::swap(m_bytes, other.m_bytes);
    }

    // Element access
    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
#if __cplusplus >= 201402L
    decltype(auto)
#else
    __NOP_TYPENAME__ __NOP_VECTOR_ITERATOR__::__NOP_VECTOR_PROXY_ITERATOR__
#endif
    operator[](__NOP_VECTOR_SIZE_TYPE__ index) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return __NOP_TYPENAME__ __NOP_VECTOR_ITERATOR__::__NOP_VECTOR_PROXY_ITERATOR__(m_storage, static_cast<__NOP_VECTOR_DIFFERENCE_TYPE__>(index));
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitState operator[](__NOP_VECTOR_SIZE_TYPE__ index) const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_storage[byteDivision(index, UL)] & BitMask::Bit << byteModule(index, UL);
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitState front() const
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:front() -> invalid number of bits");
      }

      return *m_storage & BitMask::Bit;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitState back() const
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:back() -> invalid number of bits");
      }

      return m_storage[byteDivision(m_bits - 1UL, UL)] & BitMask::Bit << byteModule(m_bits - 1UL, UL);
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_TYPENAME__ __NOP_VECTOR_ITERATOR__::__NOP_VECTOR_PROXY_ITERATOR__ at(__NOP_VECTOR_SIZE_TYPE__ index)
    {
      if (index >= m_bits || m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:at(__NOP_VECTOR_SIZE_TYPE__) -> index is out of range");
      }
      return __NOP_TYPENAME__ __NOP_VECTOR_ITERATOR__::__NOP_VECTOR_PROXY_ITERATOR__(m_storage, static_cast<__NOP_VECTOR_DIFFERENCE_TYPE__>(index));
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitState at(__NOP_VECTOR_SIZE_TYPE__ index) const
    {
      if (index >= m_bits || m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:at(__NOP_VECTOR_SIZE_TYPE__) -> index is out of range");
      }

      return m_storage[byteDivision(index, UL)] & BitMask::Bit << byteModule(index, UL);
    }

    // Operations
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& operator=(const __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& other)
    {
      if (this != &other)
      {
        if (m_bytes != other.m_bytes)
        {
          xmalloc.deallocate(m_storage, m_bytes);

#if defined(__ALLOCATOR_BASE_EXCEPTION__)
          try
          {
            m_storage = xmalloc.allocate(other.m_bytes);
          }
          catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
          {
            __NOP_THROW_EXCEPTION(err::BadAlloc, "__NOP_VECTOR__:copy assignment operator -> Cannot allocate memory");
          }
#else
          m_storage = xmalloc.allocate(other.m_bytes);
#endif
        }

        copyData(m_storage, other.m_storage, other.m_bytes);
        m_bits = other.m_bits;
        m_bytes = other.m_bytes;
      }

      return *this;
    }

#if __cplusplus >= 201103L
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& operator=(__NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>&& other) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      if (m_storage != __NOP_NULLPTR__)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = __NOP_NULLPTR__;
      }

      std::swap(m_storage, other.m_storage);
      m_bits = other.m_bits;
      m_bytes = other.m_bytes;
      other.m_bits = other.m_bytes = ZERO_VALUE;

      return *this;
    }
#endif
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& operator&=(const __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::InvalidArgument, "__NOP_VECTOR__:operator(&=) -> invalid storage size");
      }
      __NOP_VECTOR_POINTER__ beginLhs = m_storage;
      __NOP_VECTOR_POINTER__ beginRhs = rhs.m_storage;
      __NOP_VECTOR_POINTER__ end = m_storage + m_bytes;

      while (beginLhs != end)
      {
        *beginLhs &= *beginRhs;
        ++beginLhs;
        ++beginRhs;
      }

      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& operator|=(const __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::InvalidArgument, "__NOP_VECTOR__:operator(|=) -> invalid storage size");
      }

      __NOP_VECTOR_POINTER__ beginLhs = m_storage;
      __NOP_VECTOR_POINTER__ beginRhs = rhs.m_storage;
      __NOP_VECTOR_POINTER__ end = m_storage + m_bytes;

      while (beginLhs != end)
      {
        *beginLhs |= *beginRhs;
        ++beginLhs;
        ++beginRhs;
      }

      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& operator^=(const __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::InvalidArgument, "__NOP_VECTOR__:operator(^=) -> invalid storage size");
      }

      __NOP_VECTOR_POINTER__ beginLhs = m_storage;
      __NOP_VECTOR_POINTER__ beginRhs = rhs.m_storage;
      __NOP_VECTOR_POINTER__ end = m_storage + m_bytes;

      while (beginLhs != end)
      {
        *beginLhs ^= *beginRhs;
        ++beginLhs;
        ++beginRhs;
      }

      return *this;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__> operator~() const
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:operator(~) -> invalid storage pointer (nullptr)");
      }

      auto tempObj(*this);
      __NOP_VECTOR_POINTER__ end = tempObj.m_storage + tempObj.m_bytes;

      for (__NOP_VECTOR_POINTER__ begin = tempObj.m_storage; begin != end; ++begin)
      {
        *begin ^= BitMask::Set;
      }

      return tempObj;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& operator>>=(__NOP_VECTOR_SIZE_TYPE__ bitOffset)
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:operator(>>=) -> invalid storage pointer (nullptr)");
      }
      else if (bitOffset >= m_bits)
      {
        fillData(m_storage, calculateCapacity(m_bits), BitMask::Reset);
      }
      else if (bitOffset != ZERO_VALUE)
      {
        __NOP_VECTOR_SIZE_TYPE__ totalShifts = bitOffset - 1UL;

        for (__NOP_VECTOR_SIZE_TYPE__ bit = m_bits - 1UL; bit > totalShifts; --bit)
        {
          __NOP_VECTOR_SIZE_TYPE__ bitShift = bit - bitOffset;
          __NOP_VECTOR_SIZE_TYPE__ byte = m_storage[byteDivision(bitShift, UL)] &
                          static_cast<__NOP_VECTOR_SIZE_TYPE__>(BitMask::Bit) << byteModule(bitShift, UL);
          setBit(bit, byte);
        }

        fillData(m_storage, byteDivision(bitOffset, UL), BitMask::Reset);

        for (__NOP_VECTOR_SIZE_TYPE__ bit = bitOffset - byteModule(bitOffset, UL); bit != bitOffset; ++bit)
        {
          setBit(bit, BIT_UNSET);
        }
      }

      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_VECTOR__<bool, __NOP_VECTOR_ALLOCATOR_TYPE__>& operator<<=(__NOP_VECTOR_SIZE_TYPE__ bitOffset)
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "__NOP_VECTOR__:operator(<<=) -> invalid storage pointer (nullptr)");
      }
      else if (bitOffset >= m_bits)
      {
        fillData(m_storage, calculateCapacity(m_bits), BitMask::Reset);
      }
      else if (bitOffset != ZERO_VALUE)
      {
        __NOP_VECTOR_SIZE_TYPE__ totalShifts = m_bits - bitOffset;
        for (__NOP_VECTOR_SIZE_TYPE__ bit = 0UL; bit < totalShifts; ++bit)
        {
          __NOP_VECTOR_SIZE_TYPE__ bitShift = bit + bitOffset;
          __NOP_VECTOR_SIZE_TYPE__ state = m_storage[byteDivision(bitShift, UL)] &
                          (static_cast<__NOP_VECTOR_SIZE_TYPE__>(BitMask::Bit) << byteModule(bitShift, UL));
          setBit(bit, state);
        }
        __NOP_VECTOR_SIZE_TYPE__ byteShift(byteDivision(bitOffset, UL));
        fillData(m_storage + (calculateCapacity(m_bits) - byteShift),
                 byteShift,
                 BitMask::Reset);
        __NOP_VECTOR_SIZE_TYPE__ bitShift = m_bits - (byteShift << 3UL);
        for (__NOP_VECTOR_SIZE_TYPE__ bit = m_bits - bitOffset; bit != bitShift; ++bit)
        {
          setBit(bit, BIT_UNSET);
        }
      }

      return *this;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitString __NOP_VECTOR_TO_STRING__() const
    {
      bitString storageRepresentation;

      try
      {
        storageRepresentation.reserve(m_bits);
      }
      catch (const std::exception& error)
      {
        __NOP_THROW_EXCEPTION(err::BadAlloc, "__NOP_VECTOR__:toString() -> Cannot allocate memory\n");
      }

      for (__NOP_VECTOR_SIZE_TYPE__ currentBit = 0UL; currentBit != m_bits; ++currentBit)
      {
        storageRepresentation.push_back(
        static_cast<bool>(m_storage[byteDivision(currentBit, UL)] & BitMask::Bit <<
        byteModule(currentBit, UL)) + '0');
      }

      return storageRepresentation;
    }

  };

} /* End namespace nop */

template<class Alloc1, class Alloc2>
__NOP_ATTRIBUTE_NODISCARD__ __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
bool operator==(const nop::__NOP_VECTOR__<bool, Alloc1>& lhs,
                const nop::__NOP_VECTOR__<bool, Alloc2>& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
{
  if (lhs.size() == rhs.size())
  {
    auto beginLhs = lhs.data();
    auto end = beginLhs + lhs.capacity();
    auto beginRhs = rhs.data();
#if __cplusplus >= 202002L /* The support for this attribute is in progress */
    [[likely]]
#endif
    while (beginLhs < end)
    {
      if (*beginLhs != *beginRhs)
      {
        return false;
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

template<class Alloc>
__NOP_ATTRIBUTE_NODISCARD__ __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
bool operator!=(const nop::__NOP_VECTOR__<bool, Alloc>& lhs,
                const nop::__NOP_VECTOR__<bool, Alloc>& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
{
  return !(lhs == rhs);
}

template<class Alloc>
__NOP_ATTRIBUTE_NODISCARD__ __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::__NOP_VECTOR__<bool, Alloc> operator&(const nop::__NOP_VECTOR__<bool, Alloc>& lhs,
                                           const nop::__NOP_VECTOR__<bool, Alloc>& rhs)
{
  auto temp__NOP_VECTOR__(lhs);
  return temp__NOP_VECTOR__ &= rhs;
}

template<class Alloc>
__NOP_ATTRIBUTE_NODISCARD__ __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::__NOP_VECTOR__<bool, Alloc> operator|(const nop::__NOP_VECTOR__<bool, Alloc>& lhs,
                                           const nop::__NOP_VECTOR__<bool, Alloc>& rhs)
{
  auto temp__NOP_VECTOR__(lhs);
  return temp__NOP_VECTOR__ |= rhs;
}

template<class Alloc>
__NOP_ATTRIBUTE_NODISCARD__ __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::__NOP_VECTOR__<bool, Alloc> operator^(const nop::__NOP_VECTOR__<bool, Alloc>& lhs,
                                           const nop::__NOP_VECTOR__<bool, Alloc>& rhs)
{
  auto temp__NOP_VECTOR__(lhs);
  return temp__NOP_VECTOR__ ^= rhs;
}

template<class Alloc>
__NOP_ATTRIBUTE_NODISCARD__ __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::__NOP_VECTOR__<bool, Alloc> operator<<(const nop::__NOP_VECTOR__<bool, Alloc>& vectorObj,
                                            const nop::size_t bitOffset)
{
  auto temp__NOP_VECTOR__(vectorObj);
  return temp__NOP_VECTOR__ <<= bitOffset;
}

template<class Alloc>
__NOP_ATTRIBUTE_NODISCARD__ __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::__NOP_VECTOR__<bool, Alloc> operator>>(const nop::__NOP_VECTOR__<bool, Alloc>& vectorObj,
                                            const nop::size_t bitOffset)
{
  auto temp__NOP_VECTOR__(vectorObj);
  return temp__NOP_VECTOR__ >>= bitOffset;
}

namespace std /* Begin namespace std */
{

#if __cplusplus >= 202302L
  template<class Alloc>
  struct formatter<nop::__NOP_VECTOR__<bool, Alloc>> : formatter<string>
  {
    __NOP_ATTRIBUTE_NODISCARD__
    auto format(const nop::__NOP_VECTOR__<bool, Alloc> vectorObj, format_context& ctx) const
    {
      return formatter<string>::format(vectorObj.toString(), ctx);
    }
  };
#endif

  template<__NOP_TYPENAME__ T, class Alloc>
  __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
  void swap(nop::__NOP_VECTOR__<T, Alloc>& lhs, nop::__NOP_VECTOR__<T, Alloc>& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
  {
    lhs.swap(rhs);
  }

} /* End namespace std */

#include "undef_vector_bool_extension.hpp"
#include "undef_vector_macro_definitions.hpp"

#endif /* End implementation vector bool header file */
