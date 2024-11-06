#include <gtest/gtest.h>
#include "bvector.hpp"

class BVECTOR_TEST_FIXTURE : public ::testing::Test
{
protected:
  nop::bit::bvector<std::allocator<nop::u8>> empty_bvector;
  nop::bit::bvector<std::allocator<nop::u8>> filled_bvector{16, 0xffff};
};

TEST_F(BVECTOR_TEST_FIXTURE, CONSTRUCTOR_TEST)
{
  EXPECT_EQ(0, empty_bvector.size())
  << "Empty object must be initialized with zero size";
  EXPECT_EQ(0, empty_bvector.capacity())
  << "Empty object must be initialized with zero capacity";
  EXPECT_EQ(nullptr, empty_bvector.data())
  << "Empty object must be initialized with empty storage";

  EXPECT_EQ(16, filled_bvector.size());
  EXPECT_EQ(2, filled_bvector.capacity());
  ASSERT_NE(nullptr, filled_bvector.data());
  EXPECT_EQ("1111111111111111", filled_bvector.to_string())
  << "BVECTOR was initialized with 0xffff which is 0b1111111111111111";
}

TEST_F(BVECTOR_TEST_FIXTURE, COPY_CONSTRUCTOR_TEST)
{
  nop::bit::bvector test_bvector(filled_bvector);

  EXPECT_EQ(test_bvector.size(), filled_bvector.size());
  EXPECT_EQ(test_bvector.capacity(), filled_bvector.capacity());
  EXPECT_EQ(test_bvector.to_string(), filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, MOVE_CONSTRUCTOR_TEST)
{
  nop::bit::bvector test_bvector(std::move(filled_bvector));

  EXPECT_EQ(16, test_bvector.size());
  EXPECT_EQ(2, test_bvector.capacity());
  ASSERT_NE(nullptr, test_bvector.data());
  EXPECT_EQ("1111111111111111", test_bvector.to_string());

  EXPECT_EQ(empty_bvector.size(), filled_bvector.size())
  << "Moved object must be empty with zero size";
  EXPECT_EQ(empty_bvector.capacity(), filled_bvector.capacity())
  << "Moved object must be empty with zero capacity";
  EXPECT_EQ(empty_bvector.data(), filled_bvector.data())
  << "Moved object must be empty with nullptr storage";
}

TEST_F(BVECTOR_TEST_FIXTURE, SIZE_METHOD_TEST)
{
  EXPECT_EQ(0, empty_bvector.size());
  EXPECT_EQ(16, filled_bvector.size());
}

TEST_F(BVECTOR_TEST_FIXTURE, CAPACITY_METHOD_TEST)
{
  EXPECT_EQ(0, empty_bvector.capacity());
  EXPECT_EQ(2, filled_bvector.capacity());
}

TEST_F(BVECTOR_TEST_FIXTURE, DATA_METHOD_TEST)
{
  EXPECT_EQ(nullptr, empty_bvector.data());
  EXPECT_NE(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, MAX_SIZE_METHOD_TEST)
{
  EXPECT_EQ(nop::bit::bvector_limits::MAX_SIZE, empty_bvector.max_size());
}

TEST_F(BVECTOR_TEST_FIXTURE, COUNT_METHOD_TEST)
{
  EXPECT_EQ(0, empty_bvector.count())
  << "Count on empty object must return zero";
  EXPECT_EQ(16, filled_bvector.count());
}

TEST_F(BVECTOR_TEST_FIXTURE, RESERVE_METHOD)
{
  EXPECT_THROW(empty_bvector.reserve(nop::bit::bvector_limits::MAX_SIZE),
               std::length_error);
  empty_bvector.reserve(10);

  EXPECT_EQ(0, empty_bvector.size());
  ASSERT_EQ(10, empty_bvector.capacity())
  << "Invalid reservation of space which must be: previous + new <=> 0 + 10 = 10";
  ASSERT_NE(nullptr, empty_bvector.data());

  filled_bvector.reserve(10);

  EXPECT_EQ(16, filled_bvector.size());
  ASSERT_EQ(12, filled_bvector.capacity())
  << "Invalid reservation of space which must be: previous + new <=> 2 + 10 = 12";
  ASSERT_NE(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, SHRINK_TO_FIT_METHOD)
{
  empty_bvector.reserve(10);
  empty_bvector.shrink_to_fit();

  EXPECT_EQ(0, empty_bvector.size())
  << "Shrink to fit must not modify the number of bits";
  EXPECT_EQ(0, empty_bvector.capacity())
  << "Zero number of bits means empty object and in this case it must clear the object";
  EXPECT_EQ(nullptr, empty_bvector.data())
  << "Nullptr must be set after cleaning the object";

  filled_bvector.shrink_to_fit();

  EXPECT_EQ(16, filled_bvector.size())
  << "Shrink to fit must not modify the number of bits";
  EXPECT_EQ(2, filled_bvector.capacity())
  << "If object does not contain extra space it must not change object state";
  EXPECT_NE(nullptr, filled_bvector.data())
  << "Nullptr must be set if object is empty but 'filled_bvector' is not";
}

TEST_F(BVECTOR_TEST_FIXTURE, ANY_METHOD)
{
  EXPECT_EQ(false, empty_bvector.any())
  << "Empty object can not contain any bits";
  EXPECT_EQ(true, filled_bvector.any());
}

TEST_F(BVECTOR_TEST_FIXTURE, NONE_METHOD)
{
  EXPECT_EQ(true, empty_bvector.none())
  << "Empty object can not contain any bits";
  EXPECT_EQ(false, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, EMPTY_METHOD)
{
  EXPECT_EQ(true, empty_bvector.empty())
  << "Empty object can not contain any bits";
  EXPECT_EQ(false, filled_bvector.empty());
}

TEST_F(BVECTOR_TEST_FIXTURE, CLEAR_METHOD)
{
  empty_bvector.clear();
  filled_bvector.clear();

  EXPECT_EQ(empty_bvector.size(), filled_bvector.size())
  << "Every object must be empty after clear";
  EXPECT_EQ(empty_bvector.capacity(), filled_bvector.capacity())
  << "Every object must be empty after clear";
  EXPECT_EQ(empty_bvector.data(), filled_bvector.data())
  << "Every object must be empty after clear";
}

TEST_F(BVECTOR_TEST_FIXTURE, RESIZE_METHOD)
{
  empty_bvector.resize(10, false);
  filled_bvector.resize(20, true);

  EXPECT_EQ(10, empty_bvector.size())
  << "Resize must modify the number of bits";
  EXPECT_EQ(2, empty_bvector.capacity())
  << "Resize must modify the number of bytes";
  ASSERT_NE(nullptr, empty_bvector.data());
  EXPECT_EQ(true, empty_bvector.none())
  << "Bits must be unset due to second parameter -> false";

  EXPECT_EQ(20, filled_bvector.size());
  EXPECT_EQ(3, filled_bvector.capacity());
  ASSERT_NE(nullptr, filled_bvector.data());
  EXPECT_EQ(20, filled_bvector.count())
  << "New bits must be set due to second parameter -> true";
}

TEST_F(BVECTOR_TEST_FIXTURE, PUSH_BACK_METHOD)
{
  for (nop::size_t i{}; i != 10; ++i)
  {
    empty_bvector.push_back(true);
    filled_bvector.push_back(true);
  }

  EXPECT_EQ(10, empty_bvector.count());
  EXPECT_EQ(26, filled_bvector.count());
}

TEST_F(BVECTOR_TEST_FIXTURE, POP_BACK_METHOD)
{
  EXPECT_THROW((void)empty_bvector.pop_back(),
                std::out_of_range);

  for (nop::size_t i {}; i != 16; ++i)
    ASSERT_EQ(true, filled_bvector.pop_back());
}

TEST_F(BVECTOR_TEST_FIXTURE, SET_INDEX_METHOD)
{
  EXPECT_THROW(empty_bvector.set(10),
               std::out_of_range);
  
  for (nop::size_t i{}; i != 4; ++i)
    filled_bvector.set(i, false);
  
  EXPECT_EQ("0000111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, SET_METHOD)
{
  EXPECT_THROW(empty_bvector.set(),
               std::out_of_range);

  nop::bit::bvector test_bvector(16);
  test_bvector.set();

  EXPECT_EQ(false, test_bvector.none())
  << "Set method must set all bits to true";
}

TEST_F(BVECTOR_TEST_FIXTURE, RESET_INDEX_METHOD)
{
  EXPECT_THROW(empty_bvector.reset(10),
               std::out_of_range);

  for (nop::size_t i{}; i != 4; ++i)
    filled_bvector.reset(i);
  
  EXPECT_EQ("0000111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, RESET_METHOD)
{
  EXPECT_THROW(empty_bvector.reset(),
               std::out_of_range);

  filled_bvector.reset();

  EXPECT_EQ(true, filled_bvector.none())
  << "Reset method must set all bits to false";
}

TEST_F(BVECTOR_TEST_FIXTURE, FLIP_INDEX_METHOD)
{
  EXPECT_THROW(empty_bvector.flip(10),
               std::out_of_range);

  for (nop::size_t i{}; i != 4; ++i)
    filled_bvector.flip(i);
  
  EXPECT_EQ("0000111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, FLIP_METHOD)
{
  EXPECT_THROW(empty_bvector.flip(),
               std::out_of_range);

  filled_bvector.flip();

  EXPECT_EQ(true, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, SWAP_METHOD)
{
  empty_bvector.swap(filled_bvector);

  EXPECT_EQ(16, empty_bvector.size());
  EXPECT_EQ(2, empty_bvector.capacity());
  EXPECT_EQ(16, empty_bvector.count());
  EXPECT_EQ(0, filled_bvector.size());
  EXPECT_EQ(0, filled_bvector.capacity());
  EXPECT_EQ(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, SUBSCRIPT_OPERATOR_TEST)
{
  for (nop::size_t i{}; i != 10; ++i)
  {
    ASSERT_EQ(true, filled_bvector[i]);
    filled_bvector[i] = false;
    ASSERT_EQ(false, filled_bvector[i]);
    filled_bvector[i] = true;
    ASSERT_EQ(true, filled_bvector[i]);
  }
}

TEST_F(BVECTOR_TEST_FIXTURE, AT_METHOD)
{
  EXPECT_THROW((void)empty_bvector.at(10),
                std::out_of_range);
  EXPECT_THROW((void)filled_bvector.at(20),
               std::out_of_range);

  EXPECT_EQ(true, filled_bvector.at(0));
  filled_bvector.reset(0);
  EXPECT_EQ(false, filled_bvector.at(0));
}

TEST_F(BVECTOR_TEST_FIXTURE, FRONT_METHOD)
{
  EXPECT_THROW((void)empty_bvector.front(),
               std::out_of_range);

  EXPECT_EQ(true, filled_bvector.front());
  filled_bvector.reset(0);
  EXPECT_EQ(false, filled_bvector.front());
}

TEST_F(BVECTOR_TEST_FIXTURE, BACK_METHOD)
{
  EXPECT_THROW((void)empty_bvector.back(),
                std::out_of_range);

  EXPECT_EQ(true, filled_bvector.back());
  filled_bvector.reset(15);
  EXPECT_EQ(false, filled_bvector.back());
}

TEST_F(BVECTOR_TEST_FIXTURE, COPY_ASSIGNMENT_OPERATOR)
{
  empty_bvector = filled_bvector;

  EXPECT_EQ(empty_bvector.size(), filled_bvector.size());
  EXPECT_EQ(empty_bvector.capacity(), filled_bvector.capacity());
  ASSERT_NE(nullptr, empty_bvector.data());
  EXPECT_EQ(empty_bvector.to_string(), filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, MOVE_ASSIGNMENT_OPERATOR)
{
  empty_bvector = std::move(filled_bvector);

  EXPECT_EQ(16, empty_bvector.size());
  EXPECT_EQ(2, empty_bvector.capacity());
  EXPECT_EQ(16, empty_bvector.count());
  EXPECT_EQ(0, filled_bvector.size());
  EXPECT_EQ(0, filled_bvector.capacity());
  EXPECT_EQ(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, BITWISE_AND_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(empty_bvector &= filled_bvector,
               std::invalid_argument);

  empty_bvector.resize(16, true);
  filled_bvector &= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(16, filled_bvector.count());

  empty_bvector.reset();
  filled_bvector &= empty_bvector;

  EXPECT_EQ(16, filled_bvector.size());
  EXPECT_EQ(2, filled_bvector.capacity());
  EXPECT_EQ(true, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, BITWISE_OR_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(empty_bvector |= filled_bvector,
               std::invalid_argument);

  empty_bvector.resize(16, true);
  filled_bvector |= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(16, filled_bvector.count());

  empty_bvector.reset();
  filled_bvector |= empty_bvector;

  EXPECT_EQ(16, filled_bvector.size());
  EXPECT_EQ(2, filled_bvector.capacity());
  EXPECT_EQ(false, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, BITWISE_XOR_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(empty_bvector ^= filled_bvector,
               std::invalid_argument);

  empty_bvector.resize(16, true);
  filled_bvector ^= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(true, filled_bvector.none());

  filled_bvector ^= empty_bvector;

  EXPECT_EQ(16, filled_bvector.size());
  EXPECT_EQ(2, filled_bvector.capacity());
  EXPECT_EQ(16, filled_bvector.count());
}

TEST_F(BVECTOR_TEST_FIXTURE, BITWISE_INVERSE_OPERATOR_TEST)
{
  EXPECT_THROW((void)~empty_bvector,
                std::out_of_range);

  empty_bvector = ~filled_bvector;

  ASSERT_EQ(16, empty_bvector.size());
  ASSERT_EQ(2, empty_bvector.capacity());
  ASSERT_EQ(true, empty_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, TO_STRING_METHOD_TEST)
{
  EXPECT_EQ("", empty_bvector.to_string());
  EXPECT_EQ("1111111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, LHS_ASSIGNMENT_TEST)
{
  EXPECT_THROW(empty_bvector >>= 10,
               std::out_of_range);

  filled_bvector >>= 0;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(16, filled_bvector.count());

  filled_bvector >>= 8;

  ASSERT_EQ("0000000011111111", filled_bvector.to_string());

  filled_bvector >>= 8;

  ASSERT_EQ(true, filled_bvector.none());

  filled_bvector.set();
  filled_bvector >>= 20;

  ASSERT_EQ(true, filled_bvector.none());

  filled_bvector.set();
  filled_bvector >>= 5;

  EXPECT_EQ("0000011111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, RHS_ASSIGNMENT_TEST)
{
  EXPECT_THROW(empty_bvector <<= 10,
               std::out_of_range);

  filled_bvector <<= 0;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(16, filled_bvector.count());

  filled_bvector <<= 8;

  ASSERT_EQ("1111111100000000", filled_bvector.to_string());

  filled_bvector <<= 8;

  ASSERT_EQ(true, filled_bvector.none());

  filled_bvector.set();
  filled_bvector <<= 20;

  ASSERT_EQ(true, filled_bvector.none());
  filled_bvector.set();
  filled_bvector <<= 5;

  EXPECT_EQ("1111111111100000", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, COPY_LHS_TEST)
{
  empty_bvector = filled_bvector << 8;

  ASSERT_EQ("1111111100000000", empty_bvector.to_string());

  empty_bvector = filled_bvector << 1;

  EXPECT_EQ("1111111111111110", empty_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, COPY_RHS_TEST)
{
  empty_bvector = filled_bvector >> 8;

  ASSERT_EQ("0000000011111111", empty_bvector.to_string());

  empty_bvector = filled_bvector >> 1;

  EXPECT_EQ("0111111111111111", empty_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, ADVANCED_LHS_RHS_TEST)
{
  for (nop::size_t i{}; i != 16; ++i)
  {
    filled_bvector >>= 1;
    ASSERT_EQ(false, filled_bvector[i]);
  }

  filled_bvector.set();

  for (nop::size_t i{}; i != 16; ++i)
  {
    filled_bvector <<= 1;
    ASSERT_EQ(false, filled_bvector[15 - i]);
  }

  filled_bvector.set();
  filled_bvector >>= 8;

  ASSERT_EQ("0000000011111111", filled_bvector.to_string());

  filled_bvector >>= 3;

  ASSERT_EQ("0000000000011111", filled_bvector.to_string());

  filled_bvector >>= 5;

  ASSERT_EQ(true, filled_bvector.none());

  filled_bvector.set();
  filled_bvector <<= 8;

  ASSERT_EQ("1111111100000000", filled_bvector.to_string());

  filled_bvector <<= 3;

  ASSERT_EQ("1111100000000000", filled_bvector.to_string());

  filled_bvector <<= 5;

  EXPECT_EQ(true, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, ITERATOR_TEST)
{
  for (auto& iter : filled_bvector)
  {
    ASSERT_EQ(true, iter);
    iter = false;
    ASSERT_EQ(false, iter);
  }

  for (const auto& iter : filled_bvector)
    ASSERT_EQ(false, iter);

  for ([[maybe_unused]] auto& iter : empty_bvector)
    ASSERT_EQ(false, true)
    << "Error in for range loop traversing empty object";
  
  for ([[maybe_unused]] const auto& iter : empty_bvector)
    ASSERT_EQ(false, true)
    << "Error in for range loop (const) traversing empty object";
}

TEST(BVECTOR_TEST_ADVANCED, STRESS_TEST)
{
  nop::bit::bvector test_bvector;

  constexpr nop::size_t SIZE{nop::bit::bvector_limits::MAX_SIZE >> 2};

  for (nop::size_t i{}; i != SIZE; ++i)
    test_bvector.push_back(true);
  
  ASSERT_EQ(SIZE, test_bvector.count());

  for (nop::size_t i{}; i != SIZE; ++i)
    (void)test_bvector.pop_back();
  
  ASSERT_EQ(true, test_bvector.none());

  test_bvector.shrink_to_fit();

  constexpr nop::size_t MID_SIZE{SIZE >> 1};

  test_bvector.resize(SIZE, true);

  test_bvector >>= MID_SIZE;

  ASSERT_EQ(MID_SIZE, test_bvector.count());

  test_bvector <<= MID_SIZE;

  ASSERT_EQ(MID_SIZE, test_bvector.count());

  test_bvector.clear();
}

nop::i32 main(nop::i32 argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
