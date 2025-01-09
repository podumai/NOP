#ifndef NOP_IMPL_BOOL_VECTOR_HPP /* Begin Vector bool header file */
#define NOP_IMPL_BOOL_VECTOR_HPP 1

#if !defined(__cplusplus)
  #error CXX compiler required
#endif

#include <bits/stl_iterator_base_types.h> /* iterator traits */
#include <bits/stl_algo.h>

#if __cplusplus >= 201103L
  #include <bits/alloc_traits.h>
#endif

#include <bits/allocator.h>

#include "impl_vector_extension.hpp"
#include "exception.hpp"
#include "types.hpp"
#include "attributes.hpp"
#include "utils.hpp"

//#include "impl_vector.hpp"

namespace nop /* Begin namespace nop */
{

  template<__NOP_TYPENAME__ T, class AllocatorType = std::allocator<T>>
  class Vector;

namespace vectorLimits /* Begin namespace vectorLimits */
{

#if __WORDSIZE == 64UL
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

  template<class AllocatorType>
  class Vector<bool, AllocatorType>
  {
  public:
    class Iterator;
    class ConstIterator;
  public:
#if __cplusplus >= 201103L
    using valueType = bool;
    using value_type = bool; /* For standard library compatibility */
    using allocator_type = AllocatorType; /* For standard library compatibility */
    using sizeType = size_t;
    using size_type = size_t; /* For standard library compatibility */
    using differenceType = ptrdiff_t;
    using difference_type = ptrdiff_t; /* For standard library compatibility */
    using reference = value_type&; /* For standard library compatibility */
    using constReference = const valueType&;
    using const_reference = const value_type&; /* For standard library compatibility */
    using pointer = std::allocator_traits<AllocatorType>::pointer;
    using constPointer = std::allocator_traits<AllocatorType>::const_pointer;
    using const_pointer = std::allocator_traits<AllocatorType>::const_pointer; /* For standard library compatibility */
    using iterator = Iterator; /* For standard library compatibility */
    using const_iterator = ConstIterator; /* For standard library compatibility */
    using bitState = bool;
    using storageState = bool;
    using bitString = std::string;
#else
    typedef bool valueType;
    typedef bool value_type; /* For standard library compatibility */
    typedef AllocatorType allocator_type; /* For standard library compatibility */
    typedef size_t sizeType;
    typedef size_t size_type; /* For standard library compatibility */
    typedef ptrdiff_t differenceType;
    typedef ptrdiff_t difference_type; /* For standard library compatibility */
    typedef value_type& reference; /* For standard library compatibility */
    typedef const valueType& constReference;
    typedef const value_type& const_reference; /* For standard library compatibility */
    typedef AllocatorType::pointer pointer;
    typedef AllocatorType::const_pointer constPointer;
    typedef AllocatorType::const_pointer const_pointer; /* For standard library compatibility */
    typedef Iterator iterator; /* For standard library compatibility */
    typedef ConstIterator const_iterator; /* For standard library compatibility */
    typedef bool bitState;
    typedef bool storageState;
    typedef std::string bitString;
#endif

    enum BitMask : sizeType
    {
      Bit = 1UL,
      Reset = 0UL,
      Set = (0xffffffffUL << (__WORDSIZE >> 1UL)) | 0xffffffffUL
    };

  public:
    class alignas(sizeof(pointer) + sizeof(sizeType)) Iterator
    {
      friend Vector;
    public:
#if __cplusplus >= 201103L
      using differenceType = Vector::differenceType;
      using difference_type = Vector::differenceType; /* For standard library compatibility */
      using valueType = Vector::valueType;
      using value_type = Vector::valueType; /* For standard library compatibility */
      using pointer = Vector::pointer;
      using reference = Vector::reference;
      using iteratorCategory = std::random_access_iterator_tag;
      using iterator_category = std::random_access_iterator_tag; /* For standard library compatibility */
  #if __cplusplus >= 202002L
      using iterator_concept = std::contiguous_iterator_tag; /* For standard library compatibility */
  #endif
#else
      typedef ptrdiff_t differenceType;
      typedef ptrdiff_t difference_type; /* For standard library compatibility */
      typedef bool valueType;
      typedef bool value_type; /* For standard library compatibility */
      typedef size_t* pointer;
      typedef size_t& reference;
      typedef std::random_access_iterator_tag iteratorCategory;
      typedef std::random_access_iterator_tag iterator_category; /* For standard library compatibility */
#endif
    private:
      class alignas(sizeof(pointer) + sizeof(differenceType)) ProxyIterator
      {
        friend Iterator;
      private:
        pointer m_byte;
        differenceType m_bit;

      public:
        __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
        ProxyIterator(pointer ptr, differenceType bitPosition) __NOP_ATTRIBUTE_NOEXCEPT__
          : m_byte(ptr)
          , m_bit(bitPosition)
        {}
#if __cplusplus >= 201103L
        ProxyIterator(const ProxyIterator&) = default;
        ProxyIterator(ProxyIterator&&) = default;
        ~ProxyIterator() = default;
#else
        ProxyIterator(const ProxyIterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
          : m_byte(other.m_byte)
          , m_byte(other.m_bit)
        {}
#endif
        __NOP_ATTRIBUTE_ALWAYS_INLINE__
        ProxyIterator& operator++() __NOP_ATTRIBUTE_NOEXCEPT__
        {
          ++m_bit;
          return *this;
        }

        __NOP_ATTRIBUTE_ALWAYS_INLINE__
        ProxyIterator& operator--() __NOP_ATTRIBUTE_NOEXCEPT__
        {
          --m_bit;
          return *this;
        }

        __NOP_ATTRIBUTE_ALWAYS_INLINE__
        void swap(ProxyIterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
        {
          std::swap(m_byte, other.m_byte);
          std::swap(m_bit, other.m_bit);
        }

        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        ProxyIterator& operator=(bitState value) __NOP_ATTRIBUTE_NOEXCEPT__
        {
          if (value == BIT_SET)
          {
            m_byte[byteDivision(static_cast<sizeType>(m_bit), UL)] |= Vector::BitMask::Bit << byteModule(static_cast<sizeType>(m_bit), UL);
          }
          else
          {
            m_byte[byteDivision(static_cast<sizeType>(m_bit), UL)] &= ~(Vector::BitMask::Bit << byteModule(static_cast<sizeType>(m_bit), UL));
          }

          return *this;
        }

        __NOP_ATTRIBUTE_NODISCARD__
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        bitState getBit() const __NOP_ATTRIBUTE_NOEXCEPT__
        {
          return m_byte[byteDivision(static_cast<sizeType>(m_bit), UL)] & Vector::BitMask::Bit << byteModule(static_cast<sizeType>(m_bit), UL);
        }

#if __cplusplus >= 201103L
        ProxyIterator& operator=(const ProxyIterator&) = default;
        ProxyIterator& operator=(ProxyIterator&&) = default;
#else
        ProxyIterator& operator=(const ProxyIterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
        {
          m_byte = other.m_byte;
          m_bit = other.m_bit;
          return *this;
        }
#endif
        __NOP_ATTRIBUTE_NODISCARD__
        __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
        bool operator==(const ProxyIterator& other) const __NOP_ATTRIBUTE_NOEXCEPT__
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
        bool operator!=(const ProxyIterator& other) const __NOP_ATTRIBUTE_NOEXCEPT__
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
      ProxyIterator m_iter;

    public:
      __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
      Iterator(pointer ptr, differenceType bitPosition) __NOP_ATTRIBUTE_NOEXCEPT__
        : m_iter(ptr, bitPosition)
      {}
#if __cplusplus >= 201103L
      Iterator(const Iterator&) = default;
      Iterator(Iterator&&) = default;
      ~Iterator() = default;
#else
      Iterator(const Iterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
        : m_iter(other.ptr, other.bitPos)
      {}
#endif

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      void swap(Iterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_iter.swap(other.m_iter);
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      Iterator& operator++() __NOP_ATTRIBUTE_NOEXCEPT__
      {
        ++m_iter;
        return *this;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      Iterator operator++(i32) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(*this);
        ++m_iter;
        return tempIterator;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      Iterator& operator+=(differenceType index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_iter.m_bit += index;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      Iterator operator+(differenceType index) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(*this);
        tempIterator += index;
        return tempIterator;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend Iterator operator+(differenceType index, const Iterator& iter) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(iter);
        tempIterator += index;
        return tempIterator;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      Iterator& operator--() __NOP_ATTRIBUTE_NOEXCEPT__
      {
        --m_iter;
        return *this;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      Iterator operator--(i32) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(*this);
        --m_iter;
        return tempIterator;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      Iterator& operator-=(differenceType index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_iter.m_bit -= index;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      Iterator operator-(differenceType index) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(*this);
        tempIterator -= index;
        return tempIterator;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend Iterator operator-(differenceType index, const Iterator& iter) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(iter);
        tempIterator -= index;
        return tempIterator;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      differenceType operator-(const Iterator& other) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_iter.m_bit - other.m_iter.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator<(const Iterator& lhs, const Iterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        if (lhs.m_iter.m_bit < rhs.m_iter.m_bit)
        {
          return true;
        }
        return false;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator<=(const Iterator& lhs, const Iterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        if (lhs.m_iter.m_bit <= rhs.m_iter.m_bit)
        {
          return true;
        }
        return false;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator>(const Iterator& lhs, const Iterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        if (lhs.m_iter.m_bit > rhs.m_iter.m_bit)
        {
          return true;
        }
        return false;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator>=(const Iterator& lhs, const Iterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        if (lhs.m_iter.m_bit >= rhs.m_iter.m_bit)
        {
          return true;
        }
        return false;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator==(const Iterator& lhs, const Iterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_iter.m_bit == rhs.m_iter.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator!=(const Iterator& lhs, const Iterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_iter.m_bit != rhs.m_iter.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ProxyIterator& operator*() __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_iter;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ProxyIterator operator[](differenceType index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return ProxyIterator(m_iter.m_byte, index);
      }

#if __cplusplus >= 201103L
      Iterator& operator=(const Iterator&) = default;
      Iterator& operator=(Iterator&&) = default;
#else
      Iterator& operator(const Iterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_iter = other.m_iter;
        return *this;
      }
#endif

    };

    class alignas(sizeof(pointer) + sizeof(sizeType)) ConstIterator
    {
    public:
#if __cplusplus >= 201103L
      using differenceType = Vector::differenceType;
      using difference_type = Vector::differenceType; /* For standard library compatibility */
      using valueType = Vector::valueType;
      using value_type = Vector::valueType; /* For standard library compatibility */
      using pointer = Vector::constPointer;
      using reference = Vector::valueType;
      using iteratorCategory = std::random_access_iterator_tag;
      using iterator_category = std::random_access_iterator_tag; /* For standard library compatibility */
  #if __cplusplus >= 202002L
      using iterator_concept = std::contiguous_iterator_tag; /* For standard library compatibility */
  #endif
#else
      typedef ptrdiff_t differenceType;
      typedef ptrdiff_t difference_type; /* For standard library compatibility */
      typedef bool valueType;
      typedef bool value_type; /* For standard library compatibility */
      typedef Vector::pointer pointer;
      typedef Vector::valueType reference;
      typedef std::random_access_iterator_tag iteratorCategory;
      typedef std::random_access_iterator_tag iterator_category; /* For standard library compatibility */
#endif

    private:
      pointer m_byte;
      differenceType m_bit;

    public:
      __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
      ConstIterator(pointer ptr, differenceType bitPosition) __NOP_ATTRIBUTE_NOEXCEPT__
        : m_byte(ptr)
        , m_bit(bitPosition)
      {}
#if __cplusplus >= 201103L
      ConstIterator(const ConstIterator&) = default;
      ConstIterator(ConstIterator&&) = default;
      ~ConstIterator() = default;
#else
      ConstIterator(const ConstIterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
        : m_byte(other.m_byte)
        , m_bit(other.m_bit)
      {}
#endif
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ConstIterator& operator++() __NOP_ATTRIBUTE_NOEXCEPT__
      {
        ++m_bit;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ConstIterator operator++(i32) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(*this);
        ++m_bit;
        return *this;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ConstIterator& operator+=(differenceType index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_bit += index;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ConstIterator operator+(differenceType index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(*this);
        m_bits += index;
        return tempIterator;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend ConstIterator operator+(differenceType index, const ConstIterator& iter) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return tempIterator(iter.m_byte, iter.m_bit + index);
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ConstIterator& operator--() __NOP_ATTRIBUTE_NOEXCEPT__
      {
         --m_bit;
         return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ConstIterator operator--(i32) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(*this);
        --m_bit;
        return *this;
      }

      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ConstIterator& operator-=(differenceType index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_bit -= index;
        return *this;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      ConstIterator operator-(differenceType index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        auto tempIterator(*this);
        tempIterator -= index;
        return tempIterator;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend ConstIterator operator-(differenceType index, const ConstIterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return tempIterator(other.m_byte, other.m_bit - index);
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      differenceType operator-(const ConstIterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_bit - other.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator>(const ConstIterator& lhs, const ConstIterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_bit > rhs.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator>=(const ConstIterator& lhs, const ConstIterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_bit >= rhs.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator<(const ConstIterator& lhs, const ConstIterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_bit < rhs.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      friend bool operator<=(const ConstIterator& lhs, const ConstIterator& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return lhs.m_bit <= rhs.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      bool operator==(const ConstIterator& other) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_bit == other.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      bool operator!=(const ConstIterator& other) const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_bit != other.m_bit;
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      bitState operator*() const __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_byte[byteDivision(m_bit, UL)] & Vector::BitMask::Bit << byteModule(m_bit, UL);
      }

      __NOP_ATTRIBUTE_NODISCARD__
      __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
      reference operator[](differenceType index) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        return m_byte[byteDivision(index, UL)] & Vector::BitMask::Bit << byteModule(index, UL);
      }

#if __cplusplus >= 201103L
      ConstIterator& operator=(const ConstIterator&) = default;
      ConstIterator& operator=(ConstIterator&&) = default;
#else
      ConstIterator& operator=(const ConstIterator& other) __NOP_ATTRIBUTE_NOEXCEPT__
      {
        m_byte = other.m_byte;
        m_bit = other.m_bit;
        return *this;
      }
#endif

    };

  private:
    __NOP_ATTRIBUTE_NO_UNIQUE_ADDRESS__ AllocatorType xmalloc;
    pointer m_storage;
    sizeType m_bits;
    sizeType m_bytes;

  private:
    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    sizeType calculateCapacity(sizeType bitsNumber) const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return byteDivision(bitsNumber, UL) + (byteModule(bitsNumber, UL) ? 1UL : 0UL);
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void setBit(sizeType index, bitState value) __NOP_ATTRIBUTE_NOEXCEPT__
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
    void copyData(pointer source, pointer destination, sizeType n) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      pointer end = source + n;
      while (source < end)
      {
        *destination = *source;
        ++source;
        ++destination;
      }
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void fillData(pointer source, sizeType n, sizeType fillValue) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      pointer end = source + n;
      while (source < end)
      {
        *source = fillValue;
        ++source;
      }
    }

  public:
    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Iterator begin() __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return Iterator(m_storage, ZERO_VALUE);
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Iterator end() __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return Iterator(m_storage, static_cast<differenceType>(m_bits));
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    ConstIterator cbegin() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return ConstIterator(m_storage, ZERO_VALUE);
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    ConstIterator cend() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return ConstIterator(m_storage, m_bits);
    }

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    Vector() __NOP_ATTRIBUTE_NOEXCEPT_EXPR(__NOP_ATTRIBUTE_NOEXCEPT_EXPR(AllocatorType()))
      : xmalloc(AllocatorType())
      , m_storage(__NOP_NULLPTR__)
      , m_bits(ZERO_VALUE)
      , m_bytes(ZERO_VALUE)
    {}

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    explicit Vector(const AllocatorType& allocator) __NOP_ATTRIBUTE_NOEXCEPT__
      : xmalloc(allocator)
      , m_storage(__NOP_NULLPTR__)
      , m_bits(ZERO_VALUE)
      , m_bytes(ZERO_VALUE)
    {}

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    Vector(sizeType bitsNumber, sizeType value = ZERO_VALUE, const AllocatorType& allocator = AllocatorType())
      : xmalloc(allocator)
      , m_bits(bitsNumber)
      , m_bytes(calculateCapacity(bitsNumber))
    {
      if (m_bits > vectorLimits::MAX_SIZE)
      {
        __NOP_THROW_EXCEPTION(err::LengthError, "Vector:Vector(sizeType, sizeType) -> invalid number of bits");
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
        __NOP_THROW_EXCEPTION(err::LengthError, "Vector:Vector(sizeType, sizeType) -> Cannot allocate memory");
      }
#else
      m_storage = xmalloc.allocate(m_bytes);
#endif
      for (sizeType i = 0UL; i < m_bytes; ++i)
      {
        m_storage[i] = BitMask::Reset;
      }

      if (value != ZERO_VALUE)
      {
        m_storage[0UL] = value;
      }
    }

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    Vector(const Vector<bool, AllocatorType>& other)
      : xmalloc(AllocatorType())
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
          __NOP_THROW_EXCEPTION(err::BadAlloc, "Vector:Vector(const Vector&) -> Cannot allocate memory");
        }
#else
        m_storage = xmalloc.allocate(m_bytes);
#endif
        copyData(other.m_storage, m_storage, m_bytes);
      }
    }

    __NOP_ATTRIBUTE_CTOR_CONSTEXPR__
    Vector(const Vector<bool, AllocatorType>& other, const AllocatorType& allocator)
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
          __NOP_THROW_EXCEPTION(err::BadAlloc, "Vector:Vector(const Vector&, const Allocator&) -> Cannot allocate memory");
        }
#else
        m_storage = xmalloc.allocate(m_bytes);
#endif
        copyData(other.m_storage, m_storage, m_bytes);
      }
    }

#if __cplusplus >= 201103L
    __NOP_ATTRIBUTE_MCTOR_CONSTEXPR__
    Vector(Vector&& other) __NOP_ATTRIBUTE_NOEXCEPT_EXPR(__NOP_ATTRIBUTE_NOEXCEPT_EXPR(AllocatorType{}))
      : xmalloc{AllocatorType{}}
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
    Vector(Vector&& other, const AllocatorType& allocator) __NOP_ATTRIBUTE_NOEXCEPT__
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
    ~Vector()
    {
      if (m_storage != __NOP_NULLPTR__)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = __NOP_NULLPTR__;
      }
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    sizeType size() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_bits;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    sizeType capacity() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_bytes;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    sizeType maxSize() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return vectorLimits::MAX_SIZE;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    pointer data() __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_storage;
    }

#if __cplusplus >= 201402L
    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    constPointer data() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_storage;
    }
#endif

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    AllocatorType getAllocator() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return xmalloc;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    sizeType count() const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        return ZERO_VALUE;
      }

      enum ParallelSumMask : sizeType
      {
#if __WORDSIZE == 64UL
        One = 0x5555555555555555UL,
        Two = 0x3333333333333333UL,
        Three = 0x0f0f0f0f0f0f0f0fUL,
        Four = 0x00ff00ff00FF00UL,
        Five = 0x0000ffff0000ffffUL,
        Six = 0x00000000ffffffffUL
#else
        One = 0x55555555UL,
        Two = 0x33333333UL,
        Three = 0x0F0F0F0FUL,
        Four = 0x00FF00FFUL,
        Five = 0x0000FFFFUL
#endif
      };
/*******************************************************************/
// TODO: Does not work properly
      pointer end = m_storage + calculateCapacity(m_bits) - 1UL;
      sizeType bitCount = 0UL;

      for (pointer begin = m_storage; begin < end; ++begin)
      {
        sizeType bits = ((*begin >> 1UL) & ParallelSumMask::One) +
                         (*begin & ParallelSumMask::One);
        bits = ((bits >> 2UL) & ParallelSumMask::Two) +
                (bits & ParallelSumMask::Two);
        bits = ((bits >> 4UL) & ParallelSumMask::Three) +
                (bits & ParallelSumMask::Three);
        bits = ((bits >> 8UL) & ParallelSumMask::Four) +
                (bits & ParallelSumMask::Three);
        bits = ((bits >> 16UL) & ParallelSumMask::Five) +
                (bits & ParallelSumMask::Five);
#if __WORDSIZE == 64UL
        bits = ((bits >> 32UL) & ParallelSumMask::Six) +
                (bits & ParallelSumMask::Six);
#endif
        /*for (sizeType bits = *begin; bits > 0UL; bits = (bits & (bits - 1UL)))
        {
          ++bitCount;
        }*/
        bitCount += bits;
      }
/*******************************************************************/

      const sizeType remainingBits = byteModule(m_bits, UL) ? byteModule(m_bits, UL) : __WORDSIZE;

      for (sizeType currentBit = 0UL; currentBit < remainingBits; ++currentBit)
      {
        if (*end & BitMask::Bit << currentBit)
        {
          ++bitCount;
        }
      }

      return bitCount;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void shrinkToFit()
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

      const sizeType currentBytes = calculateCapacity(m_bits);

      if (currentBytes < m_bytes)
      {
        pointer tempPtr;

#ifdef __ALLOCATOR_BASE_EXCEPTION__
        try
        {
          tempPtr = static_cast<pointer>(xmalloc.allocate(currentBytes));
        }
        catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
        {
          __NOP_THROW_EXCEPTION(err::BadAlloc, "Vector:shrinkToFit() -> Cannot allocate memory");
        }
#else
        tempPtr = static_cast<pointer>(xmalloc.allocate(currentBytes));
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
      pointer end = m_storage + calculateCapacity(m_bits) - 1UL;

      for (pointer begin = m_storage; begin != end; ++begin)
      {
        if (*begin)
        {
          return ANY_SET;
        }
      }
      const sizeType remainingBits = byteModule(m_bits, UL) ? byteModule(m_bits, UL) : __WORDSIZE;

      for (sizeType currentBit = 0UL; currentBit != remainingBits; ++currentBit)
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
      pointer end = m_storage + calculateCapacity(m_bits) - 1UL;

      for (pointer begin = m_storage; begin != end; ++begin)
      {
        if (*begin)
        {
          return !ANY_SET;
        }
      }
      const sizeType remainingBits = byteModule(m_bits, UL) ? byteModule(m_bits, UL) : __WORDSIZE;

      for (sizeType currentBit = 0UL; currentBit != remainingBits; ++currentBit)
      {
        if (*end & Vector::BitMask::Bit >> currentBit)
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
    void resize(sizeType bitsNumber, bitState value = BIT_UNSET)
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

      const sizeType newSize = calculateCapacity(bitsNumber);

      if (m_bytes < newSize)
      {
        pointer tempPtr;

#ifdef __ALLOCATOR_BASE_EXCEPTION__
        try
        {
          tempPtr = static_cast<pointer>(xmalloc.allocate(newSize));
        }
        catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
        {
          __NOP_THROW_EXCEPTION(err::BadAlloc, "Vector:resize(sizeType, bitState) -> Cannot allocate memory");
        }
#else
        tempPtr = static_cast<pointer>(xmalloc.allocate(newSize));
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
    void reserve(sizeType bytesNumber)
    {
      const sizeType newSize = m_bytes + bytesNumber;

      if (newSize > vectorLimits::MAX_CAPACITY)
      {
        __NOP_THROW_EXCEPTION(err::LengthError, "Vector:reserve() -> invalid number of bytes");
      }
      else if (bytesNumber == ZERO_VALUE ||
               m_bytes == vectorLimits::MAX_CAPACITY)
      {
        return;
      }

      pointer tempPtr;

#ifdef __ALLOCATOR_BASE_EXCEPTION__
      try
      {
        tempPtr = static_cast<pointer>(xmalloc.allocate(newSize));
      }
      catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
      {
        __NOP_THROW_EXCEPTION(err::BadAlloc, "Vector:reserve(sizeType) -> Cannot allocate memory");
      }
#else
      tempPtr = static_cast<pointer>(xmalloc.allocate(newSize));
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
    void pushBack(bitState value)
    {
      if (ZERO_VALUE < m_bytes && m_bytes < vectorLimits::MAX_CAPACITY)
      {
        if (resizeFactor())
        {
          sizeType additionSize;

          if (m_bytes < vectorLimits::MID_CAPACITY)
          {
            additionSize = m_bytes;
          }
          else if (!(m_bytes + (additionSize = m_bytes >> 1UL) < vectorLimits::MAX_CAPACITY))
          {
            additionSize = vectorLimits::MAX_CAPACITY - m_bytes;
          }

          reserve(additionSize);
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
        reserve(1UL);
        setBit(0UL, value);
        ++m_bits;
      }
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void popBack()
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:popBack() -> invalid number of bits");
      }
      --m_bits;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& set(sizeType index, bitState value = BIT_UNSET)
    {
      if (index >= m_bits)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:set(sizeType, bitState) -> index is out of range");
      }

      setBit(index, value);
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& set()
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:set() -> invalid number of bits");
      }

      fillData(m_storage, calculateCapacity(m_bits), BitMask::Set);
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& reset(sizeType index)
    {
      if (index >= m_bits)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:reset(sizeType) -> index is out of range");
      }

      m_storage[byteDivision(index, UL)] &= ~(BitMask::Bit << byteModule(index, UL));
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& reset()
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:reset() -> invalid number of bits");
      }

      fillData(m_storage, calculateCapacity(m_bits), BitMask::Reset);
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& flip(sizeType index)
    {
      if (index >= m_bits)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:flip(sizeType) -> index is out of range");
      }

      m_storage[byteDivision(index, UL)] ^= BitMask::Bit << byteModule(index, UL);
      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& flip()
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:flip() -> invalid number of bits");
      }

      pointer end = m_storage + calculateCapacity(m_bits);

      for (pointer begin = m_storage; begin != end; ++begin)
      {
        *begin ^= BitMask::Set;
      }

      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    void swap(Vector<bool, AllocatorType>& other) __NOP_ATTRIBUTE_NOEXCEPT__
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
    __NOP_TYPENAME__ Iterator::ProxyIterator
#endif
    operator[](sizeType index) __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return __NOP_TYPENAME__ Iterator::ProxyIterator(m_storage, static_cast<differenceType>(index));
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitState operator[](sizeType index) const __NOP_ATTRIBUTE_NOEXCEPT__
    {
      return m_storage[byteDivision(index, UL)] & BitMask::Bit << byteModule(index, UL);
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitState front() const
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:front() -> invalid number of bits");
      }

      return *m_storage & BitMask::Bit;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitState back() const
    {
      if (m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:back() -> invalid number of bits");
      }

      return m_storage[byteDivision(m_bits - 1UL, UL)] & BitMask::Bit << byteModule(m_bits - 1UL, UL);
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    __NOP_TYPENAME__ Iterator::ProxyIterator at(sizeType index)
    {
      if (index >= m_bits || m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:at(sizeType) -> index is out of range");
      }
      return __NOP_TYPENAME__ Iterator::ProxyIterator(m_storage, static_cast<differenceType>(index));
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitState at(sizeType index) const
    {
      if (index >= m_bits || m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:at(sizeType) -> index is out of range");
      }

      return m_storage[byteDivision(index, UL)] & BitMask::Bit << byteModule(index, UL);
    }

    // Operations
    Vector<bool, AllocatorType>& operator=(const Vector<bool, AllocatorType>& other)
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
            __NOP_THROW_EXCEPTION(err::BadAlloc, "Vector:copy assignment operator -> Cannot allocate memory");
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
    Vector<bool, AllocatorType>& operator=(Vector<bool, AllocatorType>&& other) __NOP_ATTRIBUTE_NOEXCEPT__
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
    Vector<bool, AllocatorType>& operator&=(const Vector<bool, AllocatorType>& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::InvalidArgument, "Vector:operator(&=) -> invalid storage size");
      }
      pointer beginLhs = m_storage;
      pointer beginRhs = rhs.m_storage;
      pointer end = m_storage + m_bytes;

      while (beginLhs != end)
      {
        *beginLhs &= *beginRhs;
        ++beginLhs;
        ++beginRhs;
      }

      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& operator|=(const Vector<bool, AllocatorType>& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::InvalidArgument, "Vector:operator(|=) -> invalid storage size");
      }

      pointer beginLhs = m_storage;
      pointer beginRhs = rhs.m_storage;
      pointer end = m_storage + m_bytes;

      while (beginLhs != end)
      {
        *beginLhs |= *beginRhs;
        ++beginLhs;
        ++beginRhs;
      }

      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& operator^=(const Vector<bool, AllocatorType>& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        __NOP_THROW_EXCEPTION(err::InvalidArgument, "Vector:operator(^=) -> invalid storage size");
      }

      pointer beginLhs = m_storage;
      pointer beginRhs = rhs.m_storage;
      pointer end = m_storage + m_bytes;

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
    Vector<bool, AllocatorType> operator~() const
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:operator(~) -> invalid storage pointer (nullptr)");
      }

      auto tempObj(*this);
      pointer end = tempObj.m_storage + tempObj.m_bytes;

      for (pointer begin = tempObj.m_storage; begin != end; ++begin)
      {
        *begin ^= BitMask::Set;
      }

      return tempObj;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& operator>>=(sizeType bitOffset)
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:operator(>>=) -> invalid storage pointer (nullptr)");
      }
      else if (bitOffset >= m_bits)
      {
        fillData(m_storage, calculateCapacity(m_bits), BitMask::Reset);
      }
      else if (bitOffset != ZERO_VALUE)
      {
        sizeType totalShifts = bitOffset - 1UL;

        for (sizeType bit = m_bits - 1UL; bit > totalShifts; --bit)
        {
          sizeType bitShift = bit - bitOffset;
          sizeType byte = m_storage[byteDivision(bitShift, UL)] &
                          static_cast<sizeType>(BitMask::Bit) << byteModule(bitShift, UL);
          setBit(bit, byte);
        }

        fillData(m_storage, byteDivision(bitOffset, UL), BitMask::Reset);

        for (sizeType bit = bitOffset - byteModule(bitOffset, UL); bit != bitOffset; ++bit)
        {
          setBit(bit, BIT_UNSET);
        }
      }

      return *this;
    }

    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    Vector<bool, AllocatorType>& operator<<=(sizeType bitOffset)
    {
      if (m_storage == __NOP_NULLPTR__)
      {
        __NOP_THROW_EXCEPTION(err::OutOfRange, "Vector:operator(<<=) -> invalid storage pointer (nullptr)");
      }
      else if (bitOffset >= m_bits)
      {
        fillData(m_storage, calculateCapacity(m_bits), BitMask::Reset);
      }
      else if (bitOffset != ZERO_VALUE)
      {
        sizeType totalShifts = m_bits - bitOffset;
        for (sizeType bit = 0UL; bit < totalShifts; ++bit)
        {
          sizeType bitShift = bit + bitOffset;
          sizeType state = m_storage[byteDivision(bitShift, UL)] &
                          (static_cast<sizeType>(BitMask::Bit) << byteModule(bitShift, UL));
          setBit(bit, state);
        }
        sizeType byteShift(byteDivision(bitOffset, UL));
        fillData(m_storage + (calculateCapacity(m_bits) - byteShift),
                 byteShift,
                 BitMask::Reset);
        sizeType bitShift = m_bits - (byteShift << 3UL);
        for (sizeType bit = m_bits - bitOffset; bit != bitShift; ++bit)
        {
          setBit(bit, BIT_UNSET);
        }
      }

      return *this;
    }

    __NOP_ATTRIBUTE_NODISCARD__
    __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
    bitString toString() const
    {
      bitString storageRepresentation;

      try
      {
        storageRepresentation.reserve(m_bits);
      }
      catch (const std::exception& error)
      {
        __NOP_THROW_EXCEPTION(err::BadAlloc, "Vector:toString() -> Cannot allocate memory\n");
      }

      for (sizeType currentBit = 0UL; currentBit != m_bits; ++currentBit)
      {
        storageRepresentation.push_back(
        static_cast<bool>(m_storage[byteDivision(currentBit, UL)] & BitMask::Bit <<
        byteModule(currentBit, UL)) + '0');
      }

      return storageRepresentation;
    }

  };

} /* End namespace nop */

#include "undef_impl_vector_extension.hpp"

template<class AllocatorType1, class AllocatorType2>
__NOP_ATTRIBUTE_NODISCARD__
__NOP_ATTRIBUTE_FUNC_CONSTEXPR__
bool operator==(const nop::Vector<bool, AllocatorType1>& lhs,
                const nop::Vector<bool, AllocatorType2>& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
{
  if (lhs.size() == rhs.size())
  {
    auto beginLhs = lhs.data();
    auto end = beginLhs + lhs.capacity();
    auto beginRhs = rhs.data();

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

template<class AllocatorType>
__NOP_ATTRIBUTE_NODISCARD__
__NOP_ATTRIBUTE_FUNC_CONSTEXPR__
bool operator!=(const nop::Vector<bool, AllocatorType>& lhs,
                const nop::Vector<bool, AllocatorType>& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
{
  return !(lhs == rhs);
}

template<class AllocatorType>
__NOP_ATTRIBUTE_NODISCARD__
__NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::Vector<bool, AllocatorType> operator&(const nop::Vector<bool, AllocatorType>& lhs,
                                           const nop::Vector<bool, AllocatorType>& rhs)
{
  auto tempVector(lhs);
  return tempVector &= rhs;
}

template<class AllocatorType>
__NOP_ATTRIBUTE_NODISCARD__
__NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::Vector<bool, AllocatorType> operator|(const nop::Vector<bool, AllocatorType>& lhs,
                                           const nop::Vector<bool, AllocatorType>& rhs)
{
  auto tempVector(lhs);
  return tempVector |= rhs;
}

template<class AllocatorType>
__NOP_ATTRIBUTE_NODISCARD__
__NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::Vector<bool, AllocatorType> operator^(const nop::Vector<bool, AllocatorType>& lhs,
                                           const nop::Vector<bool, AllocatorType>& rhs)
{
  auto tempVector(lhs);
  return tempVector ^= rhs;
}

template<class AllocatorType>
__NOP_ATTRIBUTE_NODISCARD__
__NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::Vector<bool, AllocatorType> operator<<(const nop::Vector<bool, AllocatorType>& vectorObj,
                                            const nop::size_t bitOffset)
{
  auto tempVector(vectorObj);
  return tempVector <<= bitOffset;
}

template<class AllocatorType>
__NOP_ATTRIBUTE_NODISCARD__
__NOP_ATTRIBUTE_FUNC_CONSTEXPR__
nop::Vector<bool, AllocatorType> operator>>(const nop::Vector<bool, AllocatorType>& vectorObj,
                                            const nop::size_t bitOffset)
{
  auto tempVector(vectorObj);
  return tempVector >>= bitOffset;
}

namespace std /* Begin namespace std */
{

#if __cplusplus >= 202302L
  template<class AllocatorType>
  struct formatter<nop::Vector<bool, AllocatorType>> : formatter<string>
  {
    __NOP_ATTRIBUTE_NODISCARD__
    auto format(const nop::Vector<bool, AllocatorType> vectorObj, format_context& ctx) const
    {
      return formatter<string>::format(vectorObj.toString(), ctx);
    }
  };
#endif

  template<__NOP_TYPENAME__ T, class AllocatorType>
  __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
  void swap(nop::Vector<T, AllocatorType>& lhs, nop::Vector<T, AllocatorType>& rhs) __NOP_ATTRIBUTE_NOEXCEPT__
  {
    lhs.swap(rhs);
  }

} /* End namespace std */

#endif /* End Vector bool header file */
