#ifndef __NOP_LIB_BITS_BITSET_H__
#define __NOP_LIB_BITS_BITSET_H__ 1

#define calculate_capacity(bits) (((bits) >> 3) + ((bits) & 0b00000111 ? 1 : 0))
#define byte_division(bits) ((bits) >> 3)
#define byte_module(bits) ((bits) & 0b00000111)

#define BIT_SET   (true)
#define BIT_UNSET (false)
#define ZERO_VALUE (0)

#include <cstring>
#include <stdexcept>
#include <string>
#include <ostream>
#include "types.hpp"

namespace nop
{

  namespace bit
  {

    template <size_t num_bits>
    class bitset
    {
    public:
      using bin_string = std::string;
      using bit_state = bool;
      using pointer = u8*;
      using const_pointer = const u8*;
      using size_type = size_t;
      using byte = u8;

      enum class BMASK : size_type
      {
        BIT = 0b10000000,
        SET = 0b11111111,
        RESET = 0b00000000
      };

    private:
      class iterator
      {
        friend bitset;
      public:
        using reference = u8&;
        using iterator_category = std::forward_iterator_tag;

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
            if (value)
              m_byte[byte_division(m_bit)] |= static_cast<size_type>(BMASK::BIT) >> byte_module(m_bit);
            else
              m_byte[byte_division(m_bit)] &= ~(static_cast<size_type>(BMASK::BIT) >> byte_module(m_bit));

            return *this;
          }

          bool operator==(const proxy_iterator& other) const noexcept
          {
            if (m_byte == other.m_byte && m_bit == other.m_bit)
              return true;
            else
              return false;
          }

          bool operator!=(const proxy_iterator& other) const noexcept
          {
            return !(*this == other);
          }

          operator bool() const noexcept
          {
            return m_byte[byte_division(m_bit)] & static_cast<size_type>(BMASK::BIT) >> byte_module(m_bit);
          }

          explicit operator char() const noexcept
          {
            return bool(*this) + '0';
          }

        };

      private:
        proxy_iterator m_bit_iterator;

      public:
        iterator(pointer ptr, size_type bit_pos)
          : m_bit_iterator(ptr, bit_pos)
        {}

        proxy_iterator& operator*() noexcept
        {
          return m_bit_iterator;
        }

        iterator& operator++() noexcept
        {
          ++m_bit_iterator.m_bit;
          return *this;
        }

        iterator operator++(i32) noexcept
        {
          iterator tmp(*this);
          ++m_bit_iterator.m_bit;
          return tmp;
        }

        bool operator==(const iterator& other) const noexcept
        {
          return m_bit_iterator == other.m_bit_iterator;
        }

        bool operator!=(const iterator& other) const noexcept
        {
          return !(*this == other);
        }

      };

    private:
      byte m_storage[calculate_capacity(num_bits)];
    
    private:
      void set_bit(size_type index, bit_state value) const noexcept
      {
        if (value == BIT_SET)
          m_storage[byte_division(index)] |= static_cast<size_type>(BMASK::BIT) >> byte_module(index);
        else
          m_storage[byte_division(index)] &= ~(static_cast<size_type>(BMASK::BIT) >> byte_module(index));
      }

    public:
      constexpr bitset() noexcept
      {
        (void)std::memset(m_storage,
                          static_cast<size_type>(BMASK::RESET),
                          calculate_capacity(num_bits));
      }

      constexpr bitset(size_type value) noexcept
       : bitset()
      {
        (void)std::memcpy(m_storage,
                          &value,
                          calculate_capacity(num_bits) >= sizeof(size_type) ?
                          sizeof(size_type) : calculate_capacity(num_bits));
      }

      constexpr bitset(bitset&& other) noexcept
      {
        (void)std::memcpy(m_storage,
                          other.m_storage,
                          calculate_capacity(num_bits));
        (void)std::memset(other.m_storage,
                          static_cast<size_type>(BMASK::RESET),
                          calculate_capacity(num_bits));
      }

      [[nodiscard]] iterator begin() noexcept
      {
        return iterator(m_storage, ZERO_VALUE);
      }

      [[nodiscard]] iterator end() noexcept
      {
        return iterator(m_storage, num_bits);
      }

      [[nodiscard]] constexpr const_pointer data() const noexcept
      {
        return m_storage;
      }

      [[nodiscard]] constexpr size_type size() const noexcept
      {
        return num_bits;
      }

      [[nodiscard]] constexpr size_type count() noexcept
      {
        pointer end{m_storage + calculate_capacity(num_bits) - 1};
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

        constexpr size_type remaining_bits{byte_module(num_bits) ? byte_module(num_bits) : 8};

        for (size_type current_bit{}; current_bit != remaining_bits; ++current_bit)
          if (*end & static_cast<size_type>(BMASK::BIT) >> current_bit)
            ++bit_count;

        return bit_count;
      }

      [[nodiscard]] constexpr bool all() const noexcept
      {
        if (count() == num_bits)
          return true;
        else
          return false;
      }

      [[nodiscard]] constexpr bool any() const noexcept
      {
        pointer end{m_storage + calculate_capacity(num_bits)};

        for (pointer begin{m_storage}; begin != end; ++begin)
          if (*begin)
            return true;

        return false;
      }

      [[nodiscard]] constexpr bool none() const noexcept
      {
        pointer end{m_storage + calculate_capacity(num_bits)};

        for (pointer begin{m_storage}; begin != end; ++begin)
          if (*begin)
            return false;

        return true;
      }

      [[nodiscard]] constexpr iterator::proxy_iterator operator[](size_type index) noexcept
      {
        return typename iterator::proxy_iterator(m_storage, index);
      }

      [[nodiscard]] constexpr bit_state operator[](size_type index) const noexcept
      {
        return m_storage[byte_division(index)] & static_cast<size_type>(BMASK::BIT) >> byte_module(index);
      }

      [[nodiscard]] constexpr bit_state test(size_type index) const
      {
        if (index >= num_bits)
          std::out_of_range("bitset:test(size_type) -> index is out of range");

        return m_storage[byte_division(index)] & static_cast<size_type>(BMASK::BIT) >> byte_module(index);
      }

      constexpr bitset& set() noexcept
      {
        (void)std::memset(m_storage,
                          static_cast<size_type>(BMASK::SET),
                          calculate_capacity(num_bits));

        return *this;
      }

      constexpr bitset& set(size_type index, bit_state value = BIT_SET)
      {
        if (index >= num_bits)
          std::out_of_range("bitset:set(size_type, bit_state) -> index is out of range");

        set_bit(index, value);

        return *this;
      }

      constexpr bitset& reset() noexcept
      {
        std::memset(m_storage,
                    static_cast<size_type>(BMASK::RESET),
                    calculate_capacity(num_bits));

        return *this;
      }

      constexpr bitset& reset(size_type index)
      {
        if (index >= num_bits)
          std::out_of_range("bitset:reset(size_type) -> index is out of range");

        set_bit(index, BIT_UNSET);

        return *this;
      }

      constexpr bitset& flip() noexcept
      {
        pointer end{m_storage + calculate_capacity(num_bits)};

        for (pointer begin{m_storage}; begin != end; ++begin)
          *begin ^= static_cast<size_type>(BMASK::SET);

        return *this;
      }

      constexpr bitset& flip(size_type index)
      {
        if (index >= num_bits)
          std::out_of_range("bitset:flip(size_type) -> index is out of range");

        m_storage[byte_division(index)] ^= static_cast<size_type>(BMASK::BIT) >> byte_module(index);
        return *this;
      }

      [[nodiscard]] bin_string to_string() const
      {
        bin_string storage_representation;

        try
        {
          storage_representation.reserve(num_bits);
        }
        catch (const std::exception& error)
        {
          throw;
        }

        for (size_type bit{}; bit != num_bits; ++bit)
          storage_representation.push_back(
          static_cast<bool>(m_storage[bit >> 3] &
          static_cast<size_type>(BMASK::BIT) >> (bit & 0b00000111)) + '0');

        return storage_representation;
      }

      [[nodiscard]] constexpr bitset operator~() const noexcept
      {
        bitset<num_bits> tmp(*this);
        pointer end{tmp.m_storage + calculate_capacity(num_bits)};

        for (pointer begin{tmp.m_storage}; begin != end; ++begin)
          *begin = ~(*begin);

        return tmp;
      }

      constexpr bitset& operator>>=(size_type bit_offset)
      {
        if (bit_offset >= num_bits)
          (void)std::memset(m_storage,
                            BMASK::RESET,
                            calculate_capacity(num_bits));
        else if (bit_offset != ZERO_VALUE)
        {
          size_type total_shifts{bit_offset - 1};

          for (size_type bit{num_bits - 1}; bit != total_shifts; --bit)
          {
            size_type bit_shift{bit - bit_offset};
            size_type byte{m_storage[byte_division(bit_shift)] &
                           static_cast<size_type>(BMASK::BIT) >> byte_module(bit_shift)};

            set_bit(bit, byte);
            set_bit(bit - bit_offset, BIT_UNSET);
          }
        }

        return *this;
      }

      constexpr bitset& operator<<=(size_type bit_offset)
      {
        if (bit_offset >= num_bits)
          (void)std::memset(m_storage,
                            static_cast<size_type>(BMASK::RESET),
                            calculate_capacity(num_bits));
        else if (bit_offset != ZERO_VALUE)
        {
          size_type total_shifts{num_bits - bit_offset};

          for (size_type bit{}; bit != total_shifts; ++bit)
          {
            size_type bit_shift{bit + bit_offset};
            size_type state{m_storage[byte_division(bit_shift)] &
                           static_cast<size_type>(BMASK::BIT) >> byte_module(bit_shift)};
            
            set_bit(bit, state);
            set_bit(bit + bit_shift, BIT_UNSET);
          }
        }

        return *this;
      }

      constexpr bitset& operator&=(const bitset& other)
      {
        pointer begin{m_storage};
        pointer begin_other{other.m_storage};
        pointer end{m_storage + calculate_capacity(num_bits)};

        while (begin != end)
        {
          *begin &= *begin_other;
          ++begin;
          ++begin_other;
        }

        return *this;
      }

      constexpr bitset& operator|=(const bitset& other)
      {
        pointer begin{m_storage};
        pointer begin_other{other.m_storage};
        pointer end{m_storage + calculate_capacity(num_bits)};

        while (begin != end)
        {
          *begin |= *begin_other;
          ++begin;
          ++begin_other;
        }

        return *this;
      }

      constexpr bitset& operator^=(const bitset& other)
      {
        pointer begin{m_storage};
        pointer begin_other{other.m_storage};
        pointer end{m_storage + calculate_capacity(num_bits)};

        while (begin != end)
        {
          *begin ^= *begin_other;
          ++begin;
          ++begin_other;
        }

        return *this;
      }

      constexpr bitset& operator=(const bitset& other)
      {
        (void)std::memcpy(m_storage,
                          other.m_storage,
                          calculate_capacity(num_bits));

        return *this;
      }

      constexpr bitset& operator=(bitset&& other)
      {
        (void)std::memcpy(m_storage,
                          other.m_storage,
                          calculate_capacity(num_bits));
        (void)std::memset(other.m_storage,
                          static_cast<size_type>(BMASK::RESET),
                          calculate_capacity(num_bits));

        return *this;
      }

      template <size_t other_bits>
      [[nodiscard]] bool operator==(const bitset<other_bits>& other) const noexcept
      {
        if (num_bits != other_bits)
          return false;
        else
          return std::memcmp(m_storage,
                             other.data(),
                             calculate_capacity(num_bits)) == ZERO_VALUE;
      }

      template <size_t other_bits>
      [[nodiscard]] bool operator!=(const bitset<other_bits>& other) const noexcept
      {
        return !(*this == other);
      }

    };

  }

}

template <nop::size_t lhs_bits, nop::size_t rhs_bits>
[[nodiscard]] nop::bit::bitset<lhs_bits> operator&(const nop::bit::bitset<lhs_bits>& lhs,
                                                   const nop::bit::bitset<rhs_bits>& rhs)
{
  if (lhs_bits != rhs_bits)
    throw std::invalid_argument("bitwise operator(&) -> invalid bitset sizes");

  nop::bit::bitset<lhs_bits> tmp_bitset(lhs);
  return tmp_bitset &= rhs;
}

template <nop::size_t lhs_bits, nop::size_t rhs_bits>
[[nodiscard]] nop::bit::bitset<lhs_bits> operator|(const nop::bit::bitset<lhs_bits>& lhs,
                                                   const nop::bit::bitset<rhs_bits>& rhs)
{
  if (lhs_bits != rhs_bits)
    throw std::invalid_argument("bitwise operator(|) -> invalid bitset sizes");

  nop::bit::bitset<lhs_bits> tmp_bitset(lhs);
  return tmp_bitset |= rhs;
}

template <nop::size_t lhs_bits, nop::size_t rhs_bits>
[[nodiscard]] nop::bit::bitset<lhs_bits> operator^(const nop::bit::bitset<lhs_bits>& lhs,
                                                   const nop::bit::bitset<rhs_bits>& rhs)
{
  if (lhs_bits != rhs_bits)
   throw std::invalid_argument("bitwise operator(^) -> invalid bitset sizes");

  nop::bit::bitset<lhs_bits> tmp_bitset(lhs);
  return tmp_bitset ^= rhs;
}

template <nop::size_t num_bits>
[[nodiscard]] nop::bit::bitset<num_bits> operator>>(const nop::bit::bitset<num_bits>& bitset_obj,
                                                    const nop::size_t bit_offset)
{
  nop::bit::bitset<num_bits> tmp_bitset(bitset_obj);
  return tmp_bitset >>= bit_offset;
}

template <nop::size_t num_bits>
[[nodiscard]] nop::bit::bitset<num_bits> operator<<(const nop::bit::bitset<num_bits>& bitset_obj,
                                                    const nop::size_t bit_offset)
{
  nop::bit::bitset<num_bits> tmp_bitset(bitset_obj);
  return tmp_bitset <<= bit_offset;
}

template <nop::size_t num_bits>
std::ostream& operator<<(std::ostream& out, const nop::bit::bitset<num_bits>& bitset_obj)
{
  return out << bitset_obj.to_string();
}

#undef BIT_SET
#undef BIT_UNSET
#undef calculate_capacity
#undef ZERO_VALUE

#endif
