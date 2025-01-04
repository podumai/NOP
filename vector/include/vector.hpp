#ifndef NOP_VECTOR_HPP
#define NOP_VECTOR_HPP 1

#include <cstring>
#include "vector_extensions.hpp"
#include "exception.hpp"
#include "types.hpp"

namespace nop
{

  namespace vectorLimits
  {

#if __WORDSIZE == 64UL
    extern constexpr size_t MAX_SIZE{0x2000000000UL};
    extern constexpr size_t MAX_CAPACITY{0x400000000UL};
    extern constexpr size_t MID_CAPACITY{0x100000000UL};
#else
    extern constexpr size_t MAX_SIZE{0x20000000UL};
    extern constexpr size_t MAX_CAPACITY{0x4000000UL};
    extern constexpr size_t MID_CAPACITY{0x1000000UL};
#endif

  }

  template<typename T, class AllocatorType = std::allocator<u8>>
  class Vector;

  template<class AllocatorType>
  class Vector<bool, AllocatorType>
  {
  public:
    using pointer = u8*;
    using sizeType = size_t;
    using bitState = bool;
    using storageState = bool;
    using bitString = std::string;

    enum BMASK
    {
      BIT = 0b10000000,
      RESET = 0b00000000,
      SET = 0b11111111
    };

  private:
    class Iterator
    {
      friend Vector;
    public:
      using valueType = bool;
      using reference = u8&;
      using differenceType = ptrdiff_t;
      using iteratorCategory = std::forward_iterator_tag;

      enum class BMASK : sizeType
      {
        BIT = 0b10000000,
        MOD = 0b00000111
      };

    private:
      class ProxyIterator
      {
        friend Iterator;
      private:
        pointer m_byte;
        sizeType m_bit;

      public:
        ProxyIterator(pointer ptr, sizeType bitPosition)
          : m_byte{ptr}
          , m_bit{bitPosition}
        {}

        ProxyIterator& operator=(bitState value)
        {
          if (value == BIT_SET)
          {
            m_byte[byteDivision(m_bit)] |= static_cast<sizeType>(BMASK::BIT) >> byteModule(m_bit);
          }
          else
          {
            m_byte[byteDivision(m_bit)] &= ~(static_cast<sizeType>(BMASK::BIT) >> byteModule(m_bit));
          }

          return *this;
        }

        [[nodiscard]] bitState getBit() const noexcept
        {
          return m_byte[byteDivision(m_bit)] & static_cast<sizeType>(BMASK::BIT) >> byteModule(m_bit);
        }

        [[nodiscard]] bool operator==(const ProxyIterator& other) const noexcept
        {
          if (m_byte == nullptr || other.m_byte == nullptr)
          {
            return false;
          }
          else
          {
            return getBit() == other.getBit();
          }
        }

        [[nodiscard]] bool operator!=(const ProxyIterator& other)
        {
          return !(*this == other);
        }

        operator bool() const noexcept
        {
          return m_byte == nullptr ? false : getBit();
        }

        explicit operator char() const noexcept
        {
          return m_byte == nullptr ? '0' : getBit() + '0';
        }

      };

    private:
      ProxyIterator m_iter;

    public:
      Iterator(pointer ptr, sizeType bit_pos)
        : m_iter{ptr, bit_pos}
      {}

      Iterator& operator++() noexcept
      {
        ++m_iter.m_bit;
        return *this;
      }

      Iterator operator++(i32) noexcept
      {
        auto tmp{*this};
        ++m_iter.m_bit;
        return tmp;
      }

      [[nodiscard]] ProxyIterator& operator*() noexcept
      {
        return m_iter;
      }

      [[nodiscard]] bool operator==(const Iterator& other) const noexcept
      {
        if (m_iter.m_byte == other.m_iter.m_byte &&
            m_iter.m_bit == other.m_iter.m_bit)
        {
          return true;
        }
        else
        {
          return false;
        }
      }

      [[nodiscard]] bool operator!=(const Iterator& other) const noexcept
      {
        return !(*this == other);
      }

    };

    class ConstIterator
    {
    public:
      using valueType = bool;
      using reference = u8&;
      using differenceType = ptrdiff_t;
      using iteratorCategory = std::forward_iterator_tag;

      enum class BMASK : sizeType
      {
        BIT = 0b10000000,
        MOD = 0b00000111
      };

    private:
      pointer m_byte;
      sizeType m_bit;

    public:
      ConstIterator(pointer ptr, sizeType bitPosition)
        : m_byte{ptr}
        , m_bit{bitPosition}
      {}

      ConstIterator& operator++() noexcept
      {
        ++m_bit;
        return *this;
      }

      ConstIterator operator++(i32) noexcept
      {
        auto tmp{*this};
        ++m_bit;
        return tmp;
      }

      [[nodiscard]] bitState operator*() noexcept
      {
        return m_byte[byteDivision(m_bit)] & static_cast<sizeType>(BMASK::BIT) >> byteModule(m_bit);
      }

      [[nodiscard]] bool operator==(const ConstIterator& other) const noexcept
      {
        if (m_byte == other.m_byte && m_bit == other.m_bit)
        {
          return true;
        }
        else
        {
          return false;
        }
      }

      [[nodiscard]] bool operator!=(const ConstIterator& other) const noexcept
      {
        return !(*this == other);
      }

    };

  private:
    AllocatorType xmalloc;
    pointer m_storage;
    sizeType m_bits;
    sizeType m_bytes;

  private:
    [[nodiscard]] sizeType calculateCapacity(sizeType bitsNumber) const noexcept
    {
      return (bitsNumber >> 3UL) + (bitsNumber & 0b00000111UL ? 1UL : 0UL);
    }

    void setBit(sizeType index, bitState value)
    {
      if (value == BIT_SET)
      {
        m_storage[byteDivision(index)] |= BMASK::BIT >> byteModule(index);
      }
      else
      {
        m_storage[byteDivision(index)] &= ~(BMASK::BIT >> byteModule(index));
      }
    }

    [[nodiscard]] bool resizeFactor() const noexcept
    {
      return (m_bits >> 3UL) == m_bytes;
    }

  public:
    [[nodiscard]] Iterator begin() noexcept
    {
      return Iterator{m_storage, ZERO_VALUE};
    }

    [[nodiscard]] Iterator end() noexcept
    {
      return Iterator{m_storage, m_bits};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
      return ConstIterator{m_storage, ZERO_VALUE};
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
      return ConstIterator{m_storage, m_bits};
    }

    constexpr Vector() noexcept
      : m_storage{EMPTY_STORAGE}
      , m_bits{ZERO_VALUE}
      , m_bytes{ZERO_VALUE}
    {}

    Vector(sizeType bitsNumber, sizeType value = ZERO_VALUE)
      : m_bits{bitsNumber}
      , m_bytes{calculateCapacity(bitsNumber)}
    {
      if (m_bits > vectorLimits::MAX_SIZE)
      {
        throw err::LengthError{"Vector:Vector(sizeType, sizeType) -> invalid number of bits"};
      }
      else if (bitsNumber == ZERO_VALUE)
      {
        m_storage = EMPTY_STORAGE;
        return;
      }

#ifdef __ALLOCATOR_BASE_EXCEPTION__
      try
      {
        m_storage = xmalloc.allocate(m_bytes);
      }
      catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
      {
        throw err::BadAlloc{"Vector:Vector(sizeType, sizeType) -> Cannot allocate memory"};
      }
#else
      m_storage = xmalloc.allocate(m_bytes);
#endif

      std::memset(m_storage, BMASK::RESET, m_bytes);

      if (value != ZERO_VALUE)
      {
        std::memcpy(m_storage,
                    &value,
                    m_bytes >= sizeof(sizeType) ? sizeof(sizeType) : m_bytes);
      }
    }

    Vector(const Vector& other)
      : m_bits{other.m_bits}
      , m_bytes{other.m_bytes}
    {
#ifdef __ALLOCATOR_BASE_EXCEPTION__
      try
      {
        m_storage = xmalloc.allocate(m_bytes);
      }
      catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
      {
        throw err::BadAlloc{"Vector:Vector(const Vector&) -> Cannot allocate memory"};
      }
#else
      m_storage = xmalloc.allocate(m_bytes);
#endif

      std::memcpy(m_storage, other.m_storage, m_bytes);
    }

    Vector(Vector&& other) noexcept
      : m_storage{other.m_storage}
      , m_bits{other.m_bits}
      , m_bytes{other.m_bytes}
    {
      other.m_storage = EMPTY_STORAGE;
      other.m_bits = other.m_bytes = ZERO_VALUE;
    }

    ~Vector()
    {
      if (m_storage != EMPTY_STORAGE)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = EMPTY_STORAGE;
      }
    }
    
    [[nodiscard]] constexpr sizeType size() const noexcept
    {
      return m_bits;
    }

    [[nodiscard]] constexpr sizeType capacity() const noexcept
    {
      return m_bytes;
    }

    [[nodiscard]] constexpr sizeType maxSize() const noexcept
    {
      return vectorLimits::MAX_SIZE;
    }

    [[nodiscard]] constexpr pointer data() noexcept
    {
      return m_storage;
    }
    
    [[nodiscard]] constexpr AllocatorType getAllocator() const noexcept
    {
      return xmalloc;
    }

    [[nodiscard]] constexpr sizeType count() const noexcept
    {
      if (m_storage == EMPTY_STORAGE)
      {
        return ZERO_VALUE;
      }

      pointer end{m_storage + calculateCapacity(m_bits) - 1UL};
      sizeType bitCount{};

      for (pointer begin{m_storage}; begin != end; ++begin)
      {
        sizeType byte{((*begin >> 1UL) & 0x55UL) + (*begin & 0x55UL)};
        byte = ((byte >> 2UL) & 0x33UL) + (byte & 0x33UL);
        byte = ((byte >> 4UL) & 0xfUL) + (byte & 0xfUL);
        bitCount += byte;
      }

      const sizeType remainingBits{byteModule(m_bits) ? byteModule(m_bits) : 8UL};

      for (sizeType currentBit{}; currentBit != remainingBits; ++currentBit)
      {
        if ((*end & BMASK::BIT) >> currentBit)
        {
          ++bitCount;
        }
      }

      return bitCount;
    }

    void shrinkToFit()
    {
      if (m_storage == EMPTY_STORAGE)
      {
        return;
      }
      else if (m_bits == ZERO_VALUE)
      {
        clear();
        return;
      }

      const sizeType currentBytes{calculateCapacity(m_bits)};

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
          throw err::BadAlloc{"Vector:shrinkToFit() -> Cannot allocate memory"};
        }
#else
        tempPtr = static_cast<pointer>(xmalloc.allocate(currentBytes));
#endif

        std::memcpy(tempPtr, m_storage, currentBytes);
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = tempPtr;
        m_bytes = currentBytes;
      }
    }

    [[nodiscard]] constexpr storageState any() const noexcept
    {
      if (m_storage == EMPTY_STORAGE || m_bits == ZERO_VALUE)
      {
        return !NONE_SET;
      }

      const pointer end{m_storage + calculateCapacity(m_bits) - 1UL};

      for (pointer begin{m_storage}; begin != end; ++begin)
      {
        if (*begin)
        {
          return ANY_SET;
        }
      }

      const sizeType remaining_bits{byteModule(m_bits) ? byteModule(m_bits) : 8UL};

      for (sizeType current_bit{}; current_bit != remaining_bits; ++current_bit)
      {
        if (*end & BMASK::BIT >> current_bit)
        {
          return ANY_SET;
        }
      }

      return !NONE_SET;
    }

    [[nodiscard]] constexpr storageState none() const noexcept
    {
      if (m_storage == EMPTY_STORAGE || m_bits == ZERO_VALUE)
      {
        return NONE_SET;
      }

      pointer end{m_storage + calculateCapacity(m_bits) - 1UL};

      for (pointer begin{m_storage}; begin != end; ++begin)
      {
        if (*begin)
        {
          return !ANY_SET;
        }
      }

      const sizeType remaining_bits{byteModule(m_bits) ? byteModule(m_bits) : 8UL};

      for (sizeType current_bit{}; current_bit != remaining_bits; ++current_bit)
      {
        if (*end & BMASK::BIT >> current_bit)
        {
          return !ANY_SET;
        }
      }

      return NONE_SET;
    }

    [[nodiscard]] constexpr storageState empty() const noexcept
    {
      return m_bits == ZERO_VALUE;
    }

    // Modifiers
    void clear()
    {
      if (m_storage != EMPTY_STORAGE)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = EMPTY_STORAGE;
        m_bits = m_bytes = ZERO_VALUE;
      }
    }

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

      const sizeType newSize{calculateCapacity(bitsNumber)};
      pointer tempPtr;

#ifdef __ALLOCATOR_BASE_EXCEPTION__
      try
      {
        tempPtr = static_cast<pointer>(xmalloc.allocate(newSize));
      }
      catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
      {
        throw err::BadAlloc{"Vector:resize(sizeType, bitState) -> Cannot allocate memory"};
      }
#else
      tempPtr = static_cast<pointer>(xmalloc.allocate(newSize));
#endif

      if (m_storage != EMPTY_STORAGE)
      {
        std::memcpy(tempPtr,
                    m_storage,
                    m_bytes > newSize ? newSize : m_bytes);
        xmalloc.deallocate(m_storage, m_bytes);
      }

      m_storage = tempPtr;

      if (m_bytes < newSize)
      {
        std::memset(m_storage + m_bytes,
                    value == BIT_SET ? BMASK::SET : BMASK::RESET,
                    newSize - m_bytes);
      }

      m_bits = bitsNumber;
      m_bytes = newSize;
    }

    void reserve(sizeType bytesNumber)
    {
      const sizeType newSize{m_bytes + bytesNumber};

      if (newSize > vectorLimits::MAX_CAPACITY)
      {
        throw err::LengthError{"Vector:reserve() -> invalid number of bytes"};
      }
      else if (bytesNumber == ZERO_VALUE || m_bytes == vectorLimits::MAX_CAPACITY)
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
        throw err::BadAlloc{"Vector:reserve(sizeType) -> Cannot allocate memory"};
      }
#else
      tempPtr = static_cast<pointer>(xmalloc.allocate(newSize));
#endif

      if (m_storage != EMPTY_STORAGE)
      {
        std::memcpy(tempPtr, m_storage, m_bytes);
        xmalloc.deallocate(m_storage, m_bytes);
      }

      m_storage = tempPtr;
      m_bytes = newSize;
    }

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

    void popBack()
    {
      if (m_bits == ZERO_VALUE)
      {
        throw err::OutOfRange{"Vector:popBack() -> invalid number of bits"};
      }
      --m_bits;
    }

    Vector& set(sizeType index, bitState value = BIT_UNSET)
    {
      if (index >= m_bits)
      {
        throw err::OutOfRange{"Vector:set(sizeType, bitState) -> index is out of range"};
      }

      setBit(index, value);
      return *this;
    }

    Vector& set()
    {
      if (m_bits == ZERO_VALUE)
      {
        throw err::OutOfRange{"Vector:set() -> invalid number of bits"};
      }

      std::memset(m_storage, BMASK::SET, calculateCapacity(m_bits));
      return *this;
    }

    Vector& reset(sizeType index)
    {
      if (index >= m_bits)
      {
        throw err::OutOfRange{"Vector:reset(sizeType) -> index is out of range"};
      }

      m_storage[byteDivision(index)] &= ~(BMASK::BIT >> byteModule(index));
      return *this;
    }

    Vector& reset()
    {
      if (m_bits == ZERO_VALUE)
      {
        throw err::OutOfRange{"Vector:reset() -> invalid number of bits"};
      }

      std::memset(m_storage, BMASK::RESET, calculateCapacity(m_bits));
      return *this;
    }

    Vector& flip(sizeType index)
    {
      if (index >= m_bits)
      {
        throw err::OutOfRange{"Vector:flip(sizeType) -> index is out of range"};
      }

      m_storage[byteDivision(index)] ^= BMASK::BIT >> byteModule(index);
      return *this;
    }

    Vector& flip()
    {
      if (m_bits == ZERO_VALUE)
      {
        throw err::OutOfRange{"Vector:flip() -> invalid number of bits"};
      }

      pointer end{m_storage + calculateCapacity(m_bits)};

      for (pointer begin{m_storage}; begin != end; ++begin)
      {
        *begin ^= BMASK::SET;
      }

      return *this;
    }

    void swap(Vector& other) noexcept
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
    [[nodiscard]] decltype(auto) operator[](sizeType index) noexcept
    {
      return typename Iterator::ProxyIterator{m_storage, index};
    }

    [[nodiscard]] bitState operator[](sizeType index) const noexcept
    {
      return m_storage[byteDivision(index)] & BMASK::BIT >> byteModule(index);
    }

    [[nodiscard]] bitState front() const
    {
      if (m_bits == ZERO_VALUE)
      {
        throw err::OutOfRange{"Vector:front() -> invalid number of bits"};
      }

      return *m_storage & BMASK::BIT;
    }

    [[nodiscard]] bitState back() const
    {
      if (m_bits == ZERO_VALUE)
      {
        throw err::OutOfRange{"Vector:back() -> invalid number of bits"};
      }

      return m_storage[byteDivision(m_bits - 1UL)] & BMASK::BIT >> byteModule(m_bits - 1UL);
    }

    [[nodiscard]] bitState at(sizeType index) const
    {
      if (index >= m_bits || m_storage == EMPTY_STORAGE)
      {
        throw err::OutOfRange{"Vector:at(sizeType) -> index is out of range"};
      }

      return m_storage[byteDivision(index)] & BMASK::BIT >> byteModule(index);
    }

    // Operations
    Vector<bool, AllocatorType>& operator=(const Vector<bool, AllocatorType>& other)
    {
      if (this != &other)
      {
        if (m_bytes != other.m_bytes)
        {
          xmalloc.deallocate(m_storage, m_bytes);

#ifdef __ALLOCATOR_BASE_EXCEPTION__
          try
          {
            m_storage = xmalloc.allocate(other.m_bytes);
          }
          catch (const __ALLOCATOR_BASE_EXCEPTION__& error)
          {
            throw err::BadAlloc{"Vector:copy assignment operator -> Cannot allocate memory"};
          }
#else
          m_storage = xmalloc.allocate(other.m_bytes);
#endif
        }

        std::memcpy(m_storage, other.m_storage, other.m_bytes);
        m_bits = other.m_bits;
        m_bytes = other.m_bytes;
      }

      return *this;
    }

    Vector<bool, AllocatorType>& operator=(Vector<bool, AllocatorType>&& other) noexcept
    {
      if (m_storage != EMPTY_STORAGE)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = EMPTY_STORAGE;
      }
      std::swap(m_storage, other.m_storage);
      m_bits = other.m_bits;
      m_bytes = other.m_bytes;
      other.m_bits = other.m_bytes = ZERO_VALUE;

      return *this;
    }

    Vector<bool, AllocatorType>& operator&=(const Vector<bool, AllocatorType>& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        throw err::InvalidArgument{"Vector:operator(&=) -> invalid storage size"};
      }

      pointer beginLhs{m_storage};
      pointer beginRhs{rhs.m_storage};
      pointer end{m_storage + m_bytes};

      while (beginLhs != end)
      {
        *beginLhs &= *beginRhs;
        ++beginLhs;
        ++beginRhs;
      }

      return *this;
    }

    Vector& operator|=(const Vector& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        throw err::InvalidArgument{"Vector:operator(|=) -> invalid storage size"};
      }

      pointer beginLhs{m_storage};
      pointer beginRhs{rhs.m_storage};
      pointer end{m_storage + m_bytes};

      while (beginLhs != end)
      {
        *beginLhs |= *beginRhs;
        ++beginLhs;
        ++beginRhs;
      }

      return *this;
    }

    Vector& operator^=(const Vector& rhs)
    {
      if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
      {
        throw err::InvalidArgument{"Vector:operator(^=) -> invalid storage size"};
      }

      pointer beginLhs{m_storage};
      pointer beginRhs{rhs.m_storage};
      pointer end{m_storage + m_bytes};

      while (beginLhs != end)
      {
        *beginLhs ^= *beginRhs;
        ++beginLhs;
        ++beginRhs;
      }

      return *this;
    }

    [[nodiscard]] Vector operator~() const
    {
      if (m_storage == EMPTY_STORAGE)
      {
        throw err::OutOfRange{"Vector:operator(~) -> invalid storage pointer (nullptr)"};
      }

      auto tempObj{*this};
      pointer end{tempObj.m_storage + tempObj.m_bytes};

      for (pointer begin{tempObj.m_storage}; begin != end; ++begin)
      {
        *begin ^= BMASK::SET;
      }

      return tempObj;
    }

    Vector& operator>>=(sizeType bitOffset)
    {
      if (m_storage == EMPTY_STORAGE)
      {
        throw err::OutOfRange{"Vector:operator(>>=) -> invalid storage pointer (nullptr)"};
      }
      else if (bitOffset >= m_bits)
      {
        std::memset(m_storage, BMASK::RESET, calculateCapacity(m_bits));
      }
      else if (bitOffset != ZERO_VALUE)
      {
        sizeType totalShifts{bitOffset - 1UL};

        for (sizeType bit{m_bits - 1UL}; bit != totalShifts; --bit)
        {
          sizeType bitShift{bit - bitOffset};
          sizeType byte{m_storage[byteDivision(bitShift)] &
                        static_cast<sizeType>(BMASK::BIT) >> byteModule(bitShift)};

          setBit(bit, byte);
        }

        std::memset(m_storage,
                    BMASK::RESET,
                    byteDivision(bitOffset));

        for (sizeType bit{bitOffset - byteModule(bitOffset)}; bit != bitOffset; ++bit)
        {
          setBit(bit, BIT_UNSET);
        }
      }

      return *this;
    }

    Vector& operator<<=(sizeType bitOffset)
    {
      if (m_storage == EMPTY_STORAGE)
      {
        throw err::OutOfRange{"Vector:operator(<<=) -> invalid storage pointer (nullptr)"};
      }
      else if (bitOffset >= m_bits)
      {
        std::memset(m_storage, BMASK::RESET, calculateCapacity(m_bits));
      }
      else if (bitOffset != ZERO_VALUE)
      {
        sizeType totalShifts{m_bits - bitOffset};
        for (sizeType bit{}; bit != totalShifts; ++bit)
        {
          sizeType bitShift{bit + bitOffset};
          sizeType state{m_storage[byteDivision(bitShift)] &
                         (static_cast<sizeType>(BMASK::BIT) >> byteModule(bitShift))};
          setBit(bit, state);
        }

        sizeType byteShift{byteDivision(bitOffset)};
        std::memset(m_storage + (calculateCapacity(m_bits) - byteShift),
                    BMASK::RESET,
                    byteShift);
        
        sizeType bitShift{m_bits - (byteShift << 3UL)};
        for (sizeType bit{m_bits - bitOffset}; bit != bitShift; ++bit)
        {
          setBit(bit, BIT_UNSET);
        }
      }

      return *this;
    }

    [[nodiscard]] bitString toString() const
    {
      bitString storageRepresentation;
      
      try
      {
        storageRepresentation.reserve(m_bits);
      }
      catch (const std::exception& error)
      {
        throw err::BadAlloc{"Vector:toString() -> Cannot allocate memory\n"};
      }

      for (sizeType currentBit{}; currentBit != m_bits; ++currentBit)
      {
        storageRepresentation.push_back(((m_storage[byteDivision(currentBit)] & BMASK::BIT >> byteModule(currentBit)) > 0) + '0');
        //static_cast<bool>(m_storage[byteDivision(currentBit)] & BMASK::BIT >>
        //byteModule(currentBit)) + '0');
      }

      return storageRepresentation;
    }

  };

}

#include "undef_vector_extensions.hpp"

template<class AllocatorType = std::allocator<nop::u8>>
[[nodiscard]] bool operator==(const nop::Vector<bool, AllocatorType>& lhs,
                              const nop::Vector<bool, AllocatorType>& rhs)
{
  if (lhs.size() == rhs.size())
  {
    return std::memcmp(lhs.data(), rhs.data(), lhs.capacity()) == 0;
  }
  else
  {
    return false;
  }
}

template<class AllocatorType = std::allocator<nop::u8>>
[[nodiscard]] bool operator!=(const nop::Vector<bool, AllocatorType>& lhs,
                              const nop::Vector<bool, AllocatorType>& rhs)
{
  return !(lhs == rhs);
}

template<class AllocatorType = std::allocator<nop::u8>>
[[nodiscard]] nop::Vector<bool, AllocatorType> operator&(const nop::Vector<bool, AllocatorType>& lhs,
                                                         const nop::Vector<bool, AllocatorType>& rhs)
{
  auto tempVector{lhs};
  return tempVector &= rhs;
}

template<class AllocatorType = std::allocator<nop::u8>>
[[nodiscard]] nop::Vector<bool, AllocatorType> operator|(const nop::Vector<bool, AllocatorType>& lhs,
                                                         const nop::Vector<bool, AllocatorType>& rhs)
{
  auto tempVector{lhs};
  return tempVector |= rhs;
}

template<class AllocatorType = std::allocator<nop::u8>>
[[nodiscard]] nop::Vector<bool, AllocatorType> operator^(const nop::Vector<bool, AllocatorType>& lhs,
                                                         const nop::Vector<bool, AllocatorType>& rhs)
{
  auto tempVector{lhs};
  return tempVector ^= rhs;
}

template<class AllocatorType = std::allocator<nop::u8>>
[[nodiscard]] nop::Vector<bool, AllocatorType> operator<<(const nop::Vector<bool, AllocatorType>& vectorObj,
                                                          const nop::size_t bitOffset)
{
  auto tempVector{vectorObj};
  return tempVector <<= bitOffset;
}

template<class AllocatorType = std::allocator<nop::u8>>
[[nodiscard]] nop::Vector<bool, AllocatorType> operator>>(const nop::Vector<bool, AllocatorType>& vectorObj,
                                                          const nop::size_t bitOffset)
{
  auto tempVector{vectorObj};
  return tempVector >>= bitOffset;
}

namespace std
{

  template<class AllocatorType>
  struct formatter<nop::Vector<bool, AllocatorType>> : formatter<string>
  {
    auto format(const nop::Vector<bool, AllocatorType> vectorObj, format_context& ctx) const
    {
      return formatter<string>::format(vectorObj.toString(), ctx);
    }
  };

}

#endif
