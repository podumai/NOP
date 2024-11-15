#ifndef __NOP_LIB_BITS_BVECTOR_H__
#define __NOP_LIB_BITS_BVECTOR_H__ 1

#include <string>
#include <cstring>
#include <memory>
#include <stdexcept>
#include "bvector_extensions.hpp"
#include "types.hpp"

namespace nop
{

  namespace bit
  {

    namespace bvector_limits
    {

#if __WORDSIZE == 64
      extern constexpr size_t MAX_SIZE = 0x2000000000;
      extern constexpr size_t MAX_CAPACITY = 0x400000000;
      extern constexpr size_t MID_CAPACITY = 0x100000000;
#else
      extern constexpr size_t MAX_SIZE = 0x20000000;
      extern constexpr size_t MAX_CAPACITY = 0x4000000;
      extern constexpr size_t MID_CAPACITY = 0x1000000;
#endif

    }

    template <class allocator_type = std::allocator<u8>>
    class bvector
    {
    public:
      using pointer = u8*;
      using size_type = size_t;
      using bit_state = bool;
      using storage_state = bool;
      using bit_string = std::string;

      enum BMASK
      {
        BIT = 0b10000000,
        RESET = 0b00000000,
        SET = 0b11111111
      };

    private:
      class iterator
      {
        friend bvector;
      public:
        using value_type = bool;
        using reference = u8&;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        enum class BMASK : size_type
        {
          BIT = 0b10000000,
          MOD = 0b00000111
        };

      private:
        struct proxy_iterator
        {
        public:
          pointer m_byte;
          size_type m_bit;

        public:
          proxy_iterator(pointer ptr, size_type bit_pos)
            : m_byte(ptr)
            , m_bit(bit_pos)
          {}

          proxy_iterator& operator=(bit_state value)
          {
            if (value == BIT_SET)
              m_byte[byte_division(m_bit)] |= static_cast<size_type>(BMASK::BIT) >> byte_module(m_bit);
            else
              m_byte[byte_division(m_bit)] &= ~(static_cast<size_type>(BMASK::BIT) >> byte_module(m_bit));

            return *this;
          }

          [[nodiscard]] bit_state get_bit() const noexcept
          {
            return m_byte[byte_division(m_bit)] & static_cast<size_type>(BMASK::BIT) >> byte_module(m_bit);
          }

          [[nodiscard]] bool operator==(const proxy_iterator& other) const noexcept
          {
            if (m_byte == nullptr || other.m_byte == nullptr)
              return false;
            else
              return get_bit() == other.get_bit();
          }

          [[nodiscard]] bool operator!=(const proxy_iterator& other)
          {
            return !(*this == other);
          }

          operator bool() const noexcept
          {
            return m_byte == nullptr ? false : get_bit();
          }

          explicit operator char() const noexcept
          {
            return m_byte == nullptr ? '0' : get_bit() + '0';
          }

        };

      private:
        proxy_iterator m_iter;

      public:
        iterator(pointer ptr, size_type bit_pos)
          : m_iter(ptr, bit_pos)
        {}

        iterator& operator++() noexcept
        {
          ++m_iter.m_bit;
          return *this;
        }

        iterator operator++(i32) noexcept
        {
          iterator tmp(*this);
          ++m_iter.m_bit;
          return tmp;
        }

        [[nodiscard]] proxy_iterator& operator*() noexcept
        {
          return m_iter;
        }

        [[nodiscard]] bool operator==(const iterator& other) const noexcept
        {
          if (m_iter.m_byte == other.m_iter.m_byte &&
              m_iter.m_bit == other.m_iter.m_bit)
            return true;
          else
            return false;
        }

        [[nodiscard]] bool operator!=(const iterator& other) const noexcept
        {
          return !(*this == other);
        }

      };

      class const_iterator
      {
      public:
        using value_type = bool;
        using reference = u8&;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        enum class BMASK : size_type
        {
          BIT = 0b10000000,
          MOD = 0b00000111
        };

      private:
        pointer m_byte;
        size_type m_bit;

      public:
        const_iterator(pointer ptr, size_type bit_pos)
          : m_byte(ptr)
          , m_bit(bit_pos)
        {}

        const_iterator& operator++() noexcept
        {
          ++m_bit;
          return *this;
        }

        const_iterator operator++(i32) noexcept
        {
          const_iterator tmp(*this);
          ++m_bit;
          return tmp;
        }

        [[nodiscard]] bit_state operator*() noexcept
        {
          return m_byte[byte_division(m_bit)] & static_cast<size_type>(BMASK::BIT) >> byte_module(m_bit);
        }

        [[nodiscard]] bool operator==(const const_iterator& other) const noexcept
        {
          if (m_byte == other.m_byte && m_bit == other.m_bit)
            return true;
          else
            return false;
        }

        [[nodiscard]] bool operator!=(const const_iterator& other) const noexcept
        {
          return !(*this == other);
        }

      };

    private:
      allocator_type xmalloc;
      pointer m_storage;
      size_type m_bits;
      size_type m_bytes;

    private:
      size_type calculate_capacity(size_type bits_number) const noexcept
      {
        return (bits_number >> 3) +
               (bits_number & static_cast<size_type>(0b00000111) ? 1 : 0);
      }

      void set_bit(size_type index, bit_state value)
      {
        if (value == BIT_SET)
          m_storage[byte_division(index)] |= BMASK::BIT >> byte_module(index);
        else
          m_storage[byte_division(index)] &= ~(BMASK::BIT >> byte_module(index));
      }

      bool resize_factor() const noexcept
      {
        return m_bits >> 3 == m_bytes;
      }

    public:
      iterator begin() noexcept
      {
        return iterator(m_storage, ZERO_VALUE);
      }

      iterator end() noexcept
      {
        return iterator(m_storage, m_bits);
      }

      const_iterator cbegin() const noexcept
      {
        return const_iterator(m_storage, ZERO_VALUE);
      }

      const_iterator cend() const noexcept
      {
        return const_iterator(m_storage, m_bits);
      }

      constexpr bvector() noexcept
        : m_storage(EMPTY_STORAGE)
        , m_bits(ZERO_VALUE)
        , m_bytes(ZERO_VALUE)
      {}

      bvector(size_type bits_number, size_type value = ZERO_VALUE)
        : m_bits(bits_number)
        , m_bytes(calculate_capacity(bits_number))
      {
        if (m_bits > bvector_limits::MAX_SIZE)
          throw std::length_error("bvector:bvector(size_type, size_type) -> invalid number of bits");
        else if (bits_number == ZERO_VALUE)
        {
          m_storage = EMPTY_STORAGE;
          return;
        }

        __EXCEPTION_HANDLER(m_storage = xmalloc.allocate(m_bytes))
        std::memset(m_storage, BMASK::RESET, m_bytes);

        if (value != ZERO_VALUE)
          std::memcpy(m_storage,
                      &value,
                      m_bytes >= sizeof(size_type) ? sizeof(size_type) : m_bytes);
      }

      bvector(const bvector& other)
        : m_bits(other.m_bits)
        , m_bytes(other.m_bytes)
      {
        __EXCEPTION_HANDLER(m_storage = xmalloc.allocate(m_bytes))
        std::memcpy(m_storage, other.m_storage, m_bytes);
      }

      bvector(bvector&& other) noexcept
        : m_storage(other.m_storage)
        , m_bits(other.m_bits)
        , m_bytes(other.m_bytes)
      {
        other.m_storage = EMPTY_STORAGE;
        other.m_bits = other.m_bytes = ZERO_VALUE;
      }

      constexpr ~bvector()
      {
        if (m_storage != EMPTY_STORAGE)
          xmalloc.deallocate(m_storage, m_bytes);
      }
      
      [[nodiscard]] constexpr size_type size() const noexcept
      {
        return m_bits;
      }

      [[nodiscard]] constexpr size_type capacity() const noexcept
      {
        return m_bytes;
      }

      [[nodiscard]] constexpr size_type max_size() const noexcept
      {
        return bvector_limits::MAX_SIZE;
      }

      [[nodiscard]] constexpr pointer data() noexcept
      {
        return m_storage;
      }
      
      [[nodiscard]] constexpr allocator_type get_allocator() const noexcept
      {
        return xmalloc;
      }

      [[nodiscard]] constexpr size_type count() const noexcept
      {
        if (m_storage == EMPTY_STORAGE)
          return ZERO_VALUE;

        pointer end{m_storage + calculate_capacity(m_bits) - 1};
        size_type bit_count{};

        for (pointer begin{m_storage}; begin != end; ++begin)
        {
          size_type byte{((*begin >> 1) & static_cast<size_type>(0b01010101)) +
                         (*begin & static_cast<size_type>(0b01010101))};
          byte = ((byte >> 2) & static_cast<size_type>(0b00110011)) +
                 (byte & static_cast<size_type>(0b00110011));
          byte = ((byte >> 4) & static_cast<size_type>(0b00001111)) +
                 (byte & static_cast<size_type>(0b00001111));
          bit_count += byte;
        }

        const size_type remaining_bits{byte_module(m_bits) ? byte_module(m_bits) : 8};

        for (size_type current_bit{}; current_bit != remaining_bits; ++current_bit)
          if (*end & BMASK::BIT >> current_bit)
            ++bit_count;

        return bit_count;
      }

      void shrink_to_fit()
      {
        if (m_storage == EMPTY_STORAGE)
          return;
        else if (m_bits == ZERO_VALUE)
        {
          clear();
          return;
        }

        const size_type current_bytes{calculate_capacity(m_bits)};

        if (current_bytes < m_bytes)
        {
          pointer tmp_ptr;
          __EXCEPTION_HANDLER(tmp_ptr = xmalloc.allocate(current_bytes))
          std::memcpy(tmp_ptr, m_storage, current_bytes);
          xmalloc.deallocate(m_storage, m_bytes);

          m_storage = tmp_ptr;
          m_bytes = current_bytes;
        }
      }

      [[nodiscard]] constexpr storage_state any() const noexcept
      {
        if (m_storage == EMPTY_STORAGE || m_bits == ZERO_VALUE)
          return !NONE_SET;

        const pointer end{m_storage + calculate_capacity(m_bits) - 1};

        for (pointer begin{m_storage}; begin != end; ++begin)
          if (*begin)
            return ANY_SET;

        const size_type remaining_bits{byte_module(m_bits) ? byte_module(m_bits) : 8};

        for (size_type current_bit{}; current_bit != remaining_bits; ++current_bit)
          if (*end & BMASK::BIT >> current_bit)
            return ANY_SET;

        return !NONE_SET;
      }

      [[nodiscard]] constexpr storage_state none() const noexcept
      {
        if (m_storage == EMPTY_STORAGE || m_bits == ZERO_VALUE)
          return NONE_SET;

        pointer end{m_storage + calculate_capacity(m_bits) - 1};

        for (pointer begin{m_storage}; begin != end; ++begin)
          if (*begin)
            return !ANY_SET;

        const size_type remaining_bits{byte_module(m_bits) ? byte_module(m_bits) : 8};

        for (size_type current_bit{}; current_bit != remaining_bits; ++current_bit)
          if (*end & BMASK::BIT >> current_bit)
            return !ANY_SET;

        return NONE_SET;
      }

      [[nodiscard]] constexpr storage_state empty() const noexcept
      {
        return m_bits == ZERO_VALUE;
      }

      // Modifiers
      void clear()
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = EMPTY_STORAGE;
        m_bits = m_bytes = ZERO_VALUE;
      }

      void resize(size_type bits_number, bit_state value = BIT_UNSET)
      {
        if (bits_number == ZERO_VALUE)
        {
          clear();
          return;
        }
        else if (bits_number == m_bits)
          return;

        const size_type new_size{calculate_capacity(bits_number)};
        pointer tmp_ptr;

        __EXCEPTION_HANDLER(tmp_ptr = xmalloc.allocate(new_size))

        if (m_storage != EMPTY_STORAGE)
        {
          std::memcpy(tmp_ptr,
                      m_storage,
                      m_bytes > new_size ? new_size : m_bytes);
          xmalloc.deallocate(m_storage, m_bytes);
        }

        m_storage = tmp_ptr;

        if (m_bytes < new_size)
          std::memset(m_storage + m_bytes,
                      value == BIT_SET ? BMASK::SET : BMASK::RESET,
                      new_size - m_bytes);

        m_bits = bits_number;
        m_bytes = new_size;
      }

      void reserve(size_type bytes_number)
      {
        const size_type new_size{m_bytes + bytes_number};

        if (new_size > bvector_limits::MAX_CAPACITY)
          throw std::length_error("bvector:reserve() -> invalid number of bytes");
        else if (bytes_number == ZERO_VALUE || m_bytes == bvector_limits::MAX_CAPACITY)
          return;

        pointer tmp_ptr;

        __EXCEPTION_HANDLER(tmp_ptr = xmalloc.allocate(new_size))

        if (m_storage != EMPTY_STORAGE)
        {
          std::memcpy(tmp_ptr, m_storage, m_bytes);
          xmalloc.deallocate(m_storage, m_bytes);
        }

        m_storage = tmp_ptr;
        m_bytes = new_size;
      }

      void push_back(bit_state value)
      {
        if (ZERO_VALUE < m_bytes && m_bytes < bvector_limits::MAX_CAPACITY)
        {
          if (resize_factor())
          {
            size_type addition_size;

            if (m_bytes < bvector_limits::MID_CAPACITY)
              addition_size = m_bytes;
            else if (!(m_bytes + (addition_size = m_bytes >> 1) < bvector_limits::MAX_CAPACITY))
              addition_size = bvector_limits::MAX_CAPACITY - m_bytes;

            __EXCEPTION_HANDLER(reserve(addition_size))
          }

          set_bit(m_bits, value);
          ++m_bits;
        }
        else if (m_bytes > ZERO_VALUE)
        {
          if (m_bits != bvector_limits::MAX_SIZE)
          {
            set_bit(m_bits, value);
            ++m_bits;
          }
          else
            set_bit(m_bits - 1, value);
        }
        else
        {
          __EXCEPTION_HANDLER(reserve(1))

          set_bit(0, value);
          ++m_bits;
        }
      }

      [[nodiscard]] bit_state pop_back()
      {
        if (m_bits == ZERO_VALUE)
          throw std::out_of_range("bvector:pop_back() -> invalid number of bits");

        --m_bits;
        return m_storage[byte_division(m_bits)] & BMASK::BIT >> byte_module(m_bits);
      }

      bvector& set(size_type index, bit_state value = BIT_UNSET)
      {
        if (index >= m_bits)
          throw std::out_of_range("bvector:set(size_type, bit_state) -> index is out of range");

        set_bit(index, value);

        return *this;
      }

      bvector& set()
      {
        if (m_bits == ZERO_VALUE)
          throw std::out_of_range("bvector:set() -> invalid number of bits");

        std::memset(m_storage,
                    BMASK::SET,
                    calculate_capacity(m_bits));
        return *this;
      }

      bvector& reset(size_type index)
      {
        if (index >= m_bits)
          throw std::out_of_range("bvector:reset(size_type) -> index is out of range");

        m_storage[byte_division(index)] &= ~(BMASK::BIT >> byte_module(index));
        return *this;
      }

      bvector& reset()
      {
        if (m_bits == ZERO_VALUE)
          throw std::out_of_range("bvector:reset() -> invalid number of bits");

        std::memset(m_storage,
                    BMASK::RESET,
                    calculate_capacity(m_bits));
        return *this;
      }

      bvector& flip(size_type index)
      {
        if (index >= m_bits)
          throw std::out_of_range("bvector:flip(size_type) -> index is out of range");

        m_storage[byte_division(index)] ^= BMASK::BIT >> byte_module(index);
        return *this;
      }

      bvector& flip()
      {
        if (m_bits == ZERO_VALUE)
          throw std::out_of_range("bvector:flip() -> invalid number of bits");

        pointer end{m_storage + calculate_capacity(m_bits)};

        for (pointer begin{m_storage}; begin != end; ++begin)
          *begin ^= BMASK::SET;

        return *this;
      }

      void swap(bvector& other) noexcept
      {
        if (this == &other)
          return;

        std::swap(m_storage, other.m_storage);
        std::swap(m_bits, other.m_bits);
        std::swap(m_bytes, other.m_bytes);
      }

      // Element access
      [[nodiscard]] iterator::proxy_iterator operator[](size_type index) noexcept
      {
        return typename iterator::proxy_iterator(m_storage, index);
      }

      [[nodiscard]] bit_state operator[](size_type index) const noexcept
      {
        return m_storage[byte_division(index)] & BMASK::BIT >> byte_module(index);
      }

      [[nodiscard]] bit_state front() const
      {
        if (m_bits == ZERO_VALUE)
          throw std::out_of_range("bvector:front() -> invalid number of bits");

        return *m_storage & BMASK::BIT;
      }

      [[nodiscard]] bit_state back() const
      {
        if (m_bits == ZERO_VALUE)
          throw std::out_of_range("bvector:back() -> invalid number of bits");

        return m_storage[byte_division(m_bits - 1)] & BMASK::BIT >> byte_module(m_bits - 1);
      }

      [[nodiscard]] bit_state at(size_type index) const
      {
        if (index >= m_bits || m_storage == EMPTY_STORAGE)
          throw std::out_of_range("bvector:at(size_type) -> index is out of range");

        return m_storage[byte_division(index)] & BMASK::BIT >> byte_module(index);
      }

      // Operations
      bvector& operator=(const bvector& other)
      {
        if (this != &other)
        {
          if (m_bytes != other.m_bytes)
          {
            xmalloc.deallocate(m_storage, m_bytes);
            __EXCEPTION_HANDLER(m_storage = xmalloc.allocate(other.m_bytes))
          }

          std::memcpy(m_storage,
                      other.m_storage,
                      other.m_bytes);
          m_bits = other.m_bits;
          m_bytes = other.m_bytes;
        }

        return *this;
      }

      bvector& operator=(bvector&& other)
      {
        xmalloc.deallocate(m_storage, m_bytes);
        m_storage = EMPTY_STORAGE;
        std::swap(m_storage, other.m_storage);

        m_bits = other.m_bits;
        m_bytes = other.m_bytes;
        other.m_bits = other.m_bytes = ZERO_VALUE;

        return *this;
      }

      bvector& operator&=(const bvector& rhs)
      {
        if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
          throw std::invalid_argument("bvector:operator(&=) -> invalid storage size");

        pointer begin_lhs{m_storage};
        pointer begin_rhs{rhs.m_storage};
        pointer end{m_storage + m_bytes};

        while (begin_lhs != end)
        {
          *begin_lhs &= *begin_rhs;
          ++begin_lhs;
          ++begin_rhs;
        }

        return *this;
      }

      bvector& operator|=(const bvector& rhs)
      {
        if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
          throw std::invalid_argument("bvector:operator(|=) -> invalid storage size");

        pointer begin_lhs{m_storage};
        pointer begin_rhs{rhs.m_storage};
        pointer end{m_storage + m_bytes};

        while (begin_lhs != end)
        {
          *begin_lhs |= *begin_rhs;
          ++begin_lhs;
          ++begin_rhs;
        }

        return *this;
      }

      bvector& operator^=(const bvector& rhs)
      {
        if (m_bits != rhs.m_bits || m_bits == ZERO_VALUE || rhs.m_bits == ZERO_VALUE)
          throw std::invalid_argument("bvector:operator(^=) -> invalid storage size");

        pointer begin_lhs{m_storage};
        pointer begin_rhs{rhs.m_storage};
        pointer end{m_storage + m_bytes};

        while (begin_lhs != end)
        {
          *begin_lhs ^= *begin_rhs;
          ++begin_lhs;
          ++begin_rhs;
        }

        return *this;
      }

      [[nodiscard]] bvector<allocator_type> operator~() const
      {
        try
        {
          if (m_storage == EMPTY_STORAGE)
            throw std::out_of_range("bvector:operator(~) -> invalid storage pointer (nullptr)");

          bvector<allocator_type> tmp_obj(*this);
          pointer end{tmp_obj.m_storage + tmp_obj.m_bytes};

          for (pointer begin{tmp_obj.m_storage}; begin != end; ++begin)
            *begin ^= BMASK::SET;

          return tmp_obj;
        }
        catch (const std::bad_alloc& error)
        {
          throw;
        }
      }

      bvector& operator>>=(size_type bit_offset)
      {
        if (m_storage == EMPTY_STORAGE)
          throw std::out_of_range("bvector:operator(>>=) -> invalid storage pointer (nullptr)");
        else if (bit_offset >= m_bits)
          std::memset(m_storage,
                      BMASK::RESET,
                      calculate_capacity(m_bits));
        else if (bit_offset != ZERO_VALUE)
        {
          size_type total_shifts{bit_offset - 1};

          for (size_type bit{m_bits - 1}; bit != total_shifts; --bit)
          {
            size_type bit_shift{bit - bit_offset};
            size_type byte{m_storage[byte_division(bit_shift)] &
                           static_cast<size_type>(BMASK::BIT) >> byte_module(bit_shift)};

            set_bit(bit, byte);
          }

          std::memset(m_storage,
                      BMASK::RESET,
                      byte_division(bit_offset));

          for (size_type bit{bit_offset - byte_module(bit_offset)}; bit != bit_offset; ++bit)
            set_bit(bit, BIT_UNSET);
        }

        return *this;
      }

      bvector& operator<<=(size_type bit_offset)
      {
        if (m_storage == EMPTY_STORAGE)
          throw std::out_of_range("bvector:operator(<<=) -> invalid storage pointer (nullptr)");
        else if (bit_offset >= m_bits)
          std::memset(m_storage,
                      BMASK::RESET,
                      calculate_capacity(m_bits));
        else if (bit_offset != ZERO_VALUE)
        {
          size_type total_shifts{m_bits - bit_offset};
          for (size_type bit{}; bit != total_shifts; ++bit)
          {
            size_type bit_shift{bit + bit_offset};
            size_type state{m_storage[byte_division(bit_shift)] &
                           static_cast<size_type>(BMASK::BIT) >> byte_module(bit_shift)};
            
            set_bit(bit, state);
          }

          size_type byte_shift{byte_division(bit_offset)};
          std::memset(m_storage + (calculate_capacity(m_bits) - byte_shift),
                      BMASK::RESET,
                      byte_shift);
          
          size_type bit_shift{m_bits - (byte_shift << 3)};
          for (size_type bit{m_bits - bit_offset}; bit != bit_shift; ++bit)
            set_bit(bit, BIT_UNSET);
        }

        return *this;
      }

      [[nodiscard]] bit_string to_string() const
      {
        bit_string storage_representation;

        __EXCEPTION_HANDLER(storage_representation.reserve(m_bits))

        for (size_type current_bit{}; current_bit != m_bits; ++current_bit)
          storage_representation.push_back(
          static_cast<bool>(m_storage[byte_division(current_bit)] & BMASK::BIT >>
          byte_module(current_bit)) + '0');

        return storage_representation;
      }

    };

  }

}

#undef EMPTY_STORAGE
#undef ZERO_VALUE
#undef BIT_SET
#undef BIT_UNSET
#undef ANY_SET
#undef NONE_SET
#undef __EXCEPTION_HANDLER

template<class allocator_type = std::allocator<nop::u8>>
[[nodiscard]] bool operator==(const nop::bit::bvector<allocator_type>& lhs,
                              const nop::bit::bvector<allocator_type>& rhs)
{
  if (lhs.size() != rhs.size())
    return false;
  else
    return std::memcmp(lhs.data(),
                       rhs.data(),
                       lhs.capacity()) == 0;
}

template<class allocator_type = std::allocator<nop::u8>>
[[nodiscard]] bool operator!=(const nop::bit::bvector<allocator_type>& lhs,
                              const nop::bit::bvector<allocator_type>& rhs)
{
  return !(lhs == rhs);
}

template<class allocator_type = std::allocator<nop::u8>>
[[nodiscard]] nop::bit::bvector<allocator_type> operator&(const nop::bit::bvector<allocator_type>& lhs,
                                                          const nop::bit::bvector<allocator_type>& rhs)
{
  try
  {
    nop::bit::bvector<allocator_type> tmp_bvector(lhs);
    return tmp_bvector &= rhs;
  }
  catch (const std::exception& error)
  {
    throw;
  }
}

template<class allocator_type = std::allocator<nop::u8>>
[[nodiscard]] nop::bit::bvector<allocator_type> operator|(const nop::bit::bvector<allocator_type>& lhs,
                                                          const nop::bit::bvector<allocator_type>& rhs)
{
  try
  {
    nop::bit::bvector<allocator_type> tmp_bvector(lhs);
    return tmp_bvector |= rhs;
  }
  catch (const std::exception& error)
  {
    throw;
  }
}

template<class allocator_type = std::allocator<nop::u8>>
[[nodiscard]] nop::bit::bvector<allocator_type> operator^(const nop::bit::bvector<allocator_type>& lhs,
                                                          const nop::bit::bvector<allocator_type>& rhs)
{
  try
  {
    nop::bit::bvector<allocator_type> tmp_bvector(lhs);
    return tmp_bvector ^= rhs;
  }
  catch (const std::exception& error)
  {
    throw;
  }
}

template<class allocator_type = std::allocator<nop::u8>>
[[nodiscard]] nop::bit::bvector<allocator_type> operator<<(const nop::bit::bvector<allocator_type>& bvector_obj,
                                                           const nop::size_t bit_offset)
{
  try
  {
    nop::bit::bvector<allocator_type> tmp_bvector(bvector_obj);
    return tmp_bvector <<= bit_offset;
  }
  catch (const std::exception& error)
  {
    throw;
  }
}

template<class allocator_type = std::allocator<nop::u8>>
[[nodiscard]] nop::bit::bvector<allocator_type> operator>>(const nop::bit::bvector<allocator_type>& bvector_obj,
                                                           const nop::size_t bit_offset)
{
  try
  {
    nop::bit::bvector<allocator_type> tmp_bvector(bvector_obj);
    return tmp_bvector >>= bit_offset;
  }
  catch(const std::exception& error)
  {
    throw;
  }
}

namespace std
{

  template<class allocator_type>
  struct formatter<nop::bit::bvector<allocator_type>> : formatter<string>
  {
    auto format(const nop::bit::bvector<allocator_type> bvector_obj, format_context& ctx) const
    {
      return formatter<string>::format(bvector_obj.to_string(), ctx);
    }
  };

}

#endif
