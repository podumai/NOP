#include <gtest/gtest.h>
#include "bvector.hpp"

class BVECTOR_TEST_FIXTURE : public ::testing::Test
{
protected:
  npl::bit::bvector<std::allocator<npl::u8>> empty_bvector;
  npl::bit::bvector<std::allocator<npl::u8>> filled_bvector{16, 0xffff};
};

TEST_F(BVECTOR_TEST_FIXTURE, CONSTRUCTOR_TEST)
{
  ASSERT_EQ(0, empty_bvector.size());
  ASSERT_EQ(0, empty_bvector.capacity());
  ASSERT_EQ(nullptr, empty_bvector.data());

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_NE(nullptr, filled_bvector.data());
  ASSERT_EQ("1111111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, COPY_CONSTRUCTOR_TEST)
{
  npl::bit::bvector test_bvector(filled_bvector);

  ASSERT_EQ(test_bvector.size(), filled_bvector.size());
  ASSERT_EQ(test_bvector.capacity(), filled_bvector.capacity());
  ASSERT_EQ(test_bvector.to_string(), filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, MOVE_CONSTRUCTOR_TEST)
{
  npl::bit::bvector test_bvector(std::move(filled_bvector));

  ASSERT_EQ(16, test_bvector.size());
  ASSERT_EQ(2, test_bvector.capacity());
  ASSERT_EQ("1111111111111111", test_bvector.to_string());

  ASSERT_EQ(empty_bvector.size(), filled_bvector.size());
  ASSERT_EQ(empty_bvector.capacity(), filled_bvector.capacity());
  ASSERT_EQ(empty_bvector.data(), filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, SIZE_METHOD_TEST)
{
  ASSERT_EQ(0, empty_bvector.size());
  ASSERT_EQ(16, filled_bvector.size());
}

TEST_F(BVECTOR_TEST_FIXTURE, CAPACITY_METHOD_TEST)
{
  ASSERT_EQ(0, empty_bvector.capacity());
  ASSERT_EQ(2, filled_bvector.capacity());
}

TEST_F(BVECTOR_TEST_FIXTURE, DATA_METHOD_TEST)
{
  ASSERT_EQ(nullptr, empty_bvector.data());
  ASSERT_NE(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, MAX_SIZE_METHOD_TEST)
{
  ASSERT_EQ(npl::bit::bvector_limits::MAX_SIZE, empty_bvector.max_size());
}

TEST_F(BVECTOR_TEST_FIXTURE, COUNT_METHOD_TEST)
{
  ASSERT_EQ(0, empty_bvector.count());
  ASSERT_EQ(16, filled_bvector.count());
}

TEST_F(BVECTOR_TEST_FIXTURE, RESERVE_METHOD)
{
  EXPECT_ANY_THROW(empty_bvector.reserve(npl::bit::bvector_limits::MAX_SIZE));
  empty_bvector.reserve(10);

  ASSERT_EQ(0, empty_bvector.size());
  ASSERT_EQ(10, empty_bvector.capacity());
  ASSERT_NE(nullptr, empty_bvector.data());

  filled_bvector.reserve(10);

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(12, filled_bvector.capacity());
  ASSERT_NE(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, SHRINK_TO_FIT_METHOD)
{
  empty_bvector.reserve(10);
  empty_bvector.shrink_to_fit();

  ASSERT_EQ(0, empty_bvector.size());
  ASSERT_EQ(0, empty_bvector.capacity());
  ASSERT_EQ(nullptr, empty_bvector.data());

  filled_bvector.shrink_to_fit();

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_NE(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, ANY_METHOD)
{
  ASSERT_EQ(false, empty_bvector.any());
  ASSERT_EQ(true, filled_bvector.any());
}

TEST_F(BVECTOR_TEST_FIXTURE, NONE_METHOD)
{
  ASSERT_EQ(true, empty_bvector.none());
  ASSERT_EQ(false, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, EMPTY_METHOD)
{
  ASSERT_EQ(true, empty_bvector.empty());
  ASSERT_EQ(false, filled_bvector.empty());
}

TEST_F(BVECTOR_TEST_FIXTURE, CLEAR_METHOD)
{
  empty_bvector.clear();
  filled_bvector.clear();

  ASSERT_EQ(empty_bvector.size(), filled_bvector.size());
  ASSERT_EQ(empty_bvector.capacity(), filled_bvector.capacity());
  ASSERT_EQ(empty_bvector.data(), filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, RESIZE_METHOD)
{
  empty_bvector.resize(10, false);
  filled_bvector.resize(20, true);

  ASSERT_EQ(10, empty_bvector.size());
  ASSERT_EQ(2, empty_bvector.capacity());
  ASSERT_NE(nullptr, empty_bvector.data());

  ASSERT_EQ(20, filled_bvector.size());
  ASSERT_EQ(3, filled_bvector.capacity());
  ASSERT_NE(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, PUSH_BACK_METHOD)
{
  for (npl::size_t i{}; i != 10; ++i)
  {
    empty_bvector.push_back(true);
    filled_bvector.push_back(true);
  }

  ASSERT_EQ(10, empty_bvector.count());
  ASSERT_EQ(26, filled_bvector.count());
}

TEST_F(BVECTOR_TEST_FIXTURE, POP_BACK_METHOD)
{
  EXPECT_ANY_THROW((void)empty_bvector.pop_back());

  for (npl::size_t i {}; i != 16; ++i)
    ASSERT_EQ(true, filled_bvector.pop_back());
}

TEST_F(BVECTOR_TEST_FIXTURE, SET_INDEX_METHOD)
{
  EXPECT_ANY_THROW(empty_bvector.set(10));
  
  for (npl::size_t i{}; i != 4; ++i)
    filled_bvector.set(i, false);
  
  ASSERT_EQ("0000111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, SET_METHOD)
{
  EXPECT_ANY_THROW(empty_bvector.set());

  npl::bit::bvector test_bvector(16);
  test_bvector.set();

  ASSERT_EQ(16, test_bvector.count());
}

TEST_F(BVECTOR_TEST_FIXTURE, RESET_INDEX_METHOD)
{
  EXPECT_ANY_THROW(empty_bvector.reset(10));

  for (npl::size_t i{}; i != 4; ++i)
    filled_bvector.reset(i);
  
  ASSERT_EQ("0000111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, RESET_METHOD)
{
  EXPECT_ANY_THROW(empty_bvector.reset());

  filled_bvector.reset();

  ASSERT_EQ(true, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, FLIP_INDEX_METHOD)
{
  EXPECT_ANY_THROW(empty_bvector.flip(10));

  for (npl::size_t i{}; i != 4; ++i)
    filled_bvector.flip(i);
  
  ASSERT_EQ("0000111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, FLIP_METHOD)
{
  EXPECT_ANY_THROW(empty_bvector.flip());

  filled_bvector.flip();

  ASSERT_EQ(true, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, SWAP_METHOD)
{
  empty_bvector.swap(filled_bvector);

  ASSERT_EQ(16, empty_bvector.size());
  ASSERT_EQ(2, empty_bvector.capacity());
  ASSERT_EQ(16, empty_bvector.count());
  ASSERT_EQ(0, filled_bvector.size());
  ASSERT_EQ(0, filled_bvector.capacity());
  ASSERT_EQ(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, SUBSCRIPT_OPERATOR_TEST)
{
  for (npl::size_t i{}; i != 10; ++i)
  {
    ASSERT_EQ(true, filled_bvector[i]);
    filled_bvector.reset(i);
    ASSERT_EQ(false, filled_bvector[i]);
  }
}

TEST_F(BVECTOR_TEST_FIXTURE, AT_METHOD)
{
  EXPECT_ANY_THROW((void)empty_bvector.at(10));
  EXPECT_ANY_THROW((void)filled_bvector.at(20));

  ASSERT_EQ(true, filled_bvector.at(0));
  filled_bvector.reset(0);
  ASSERT_EQ(false, filled_bvector[0]);
}

TEST_F(BVECTOR_TEST_FIXTURE, FRONT_METHOD)
{
  EXPECT_ANY_THROW((void)empty_bvector.front());

  ASSERT_EQ(true, filled_bvector.front());
  filled_bvector.reset(0);
  ASSERT_EQ(false, filled_bvector.front());
}

TEST_F(BVECTOR_TEST_FIXTURE, BACK_METHOD)
{
  EXPECT_ANY_THROW((void)empty_bvector.back());

  ASSERT_EQ(true, filled_bvector.back());
  filled_bvector.reset(15);
  ASSERT_EQ(false, filled_bvector.back());
}

TEST_F(BVECTOR_TEST_FIXTURE, COPY_ASSIGNMENT_OPERATOR)
{
  empty_bvector = filled_bvector;

  ASSERT_EQ(empty_bvector.size(), filled_bvector.size());
  ASSERT_EQ(empty_bvector.capacity(), filled_bvector.capacity());
  ASSERT_EQ(empty_bvector.to_string(), filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, MOVE_ASSIGNMENT_OPERATOR)
{
  empty_bvector = std::move(filled_bvector);

  ASSERT_EQ(16, empty_bvector.size());
  ASSERT_EQ(2, empty_bvector.capacity());
  ASSERT_EQ(16, empty_bvector.count());
  ASSERT_EQ(0, filled_bvector.size());
  ASSERT_EQ(0, filled_bvector.capacity());
  ASSERT_EQ(nullptr, filled_bvector.data());
}

TEST_F(BVECTOR_TEST_FIXTURE, BITWISE_AND_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_ANY_THROW(empty_bvector &= filled_bvector);

  empty_bvector.resize(16, true);
  filled_bvector &= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(16, filled_bvector.count());

  empty_bvector.reset();
  filled_bvector &= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(true, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, BITWISE_OR_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_ANY_THROW(empty_bvector |= filled_bvector);

  empty_bvector.resize(16, true);
  filled_bvector |= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(16, filled_bvector.count());

  empty_bvector.reset();
  filled_bvector |= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(false, filled_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, BITWISE_XOR_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_ANY_THROW(empty_bvector ^= filled_bvector);

  empty_bvector.resize(16, true);
  filled_bvector ^= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(true, filled_bvector.none());

  filled_bvector ^= empty_bvector;

  ASSERT_EQ(16, filled_bvector.size());
  ASSERT_EQ(2, filled_bvector.capacity());
  ASSERT_EQ(16, filled_bvector.count());
}

TEST_F(BVECTOR_TEST_FIXTURE, BITWISE_INVERSE_OPERATOR_TEST)
{
  EXPECT_ANY_THROW((void)~empty_bvector);

  empty_bvector = ~filled_bvector;

  ASSERT_EQ(16, empty_bvector.size());
  ASSERT_EQ(2, empty_bvector.capacity());
  ASSERT_EQ(true, empty_bvector.none());
}

TEST_F(BVECTOR_TEST_FIXTURE, TO_STRING_METHOD_TEST)
{
  ASSERT_EQ("", empty_bvector.to_string());
  ASSERT_EQ("1111111111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, LHS_ASSIGNMENT_TEST)
{
  EXPECT_ANY_THROW(empty_bvector >>= 10);

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

  ASSERT_EQ("0000011111111111", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, RHS_ASSIGNMENT_TEST)
{
  EXPECT_ANY_THROW(empty_bvector <<= 10);

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

  ASSERT_EQ("1111111111100000", filled_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, COPY_LHS_TEST)
{
  empty_bvector = filled_bvector << 8;

  ASSERT_EQ("1111111100000000", empty_bvector.to_string());

  empty_bvector = filled_bvector << 1;

  ASSERT_EQ("1111111111111110", empty_bvector.to_string());
}

TEST_F(BVECTOR_TEST_FIXTURE, COPY_RHS_TEST)
{
  empty_bvector = filled_bvector >> 8;

  ASSERT_EQ("0000000011111111", empty_bvector.to_string());

  empty_bvector = filled_bvector >> 1;

  ASSERT_EQ("0111111111111111", empty_bvector.to_string());
}

TEST(BVECTOR_TEST_ADVANCED, STRESS_TEST)
{
  npl::bit::bvector test_bvector;

  constexpr npl::size_t SIZE{npl::bit::bvector_limits::MAX_SIZE >> 2};

  for (npl::size_t i{}; i != SIZE; ++i)
    test_bvector.push_back(true);
  
  ASSERT_EQ(SIZE, test_bvector.count());

  for (npl::size_t i{}; i != SIZE; ++i)
    (void)test_bvector.pop_back();
  
  ASSERT_EQ(true, test_bvector.none());

  test_bvector.shrink_to_fit();

  constexpr npl::size_t MID_SIZE{SIZE >> 1};

  test_bvector.resize(SIZE, true);

  test_bvector >>= MID_SIZE;

  ASSERT_EQ(MID_SIZE, test_bvector.count());

  test_bvector <<= MID_SIZE;

  ASSERT_EQ(MID_SIZE, test_bvector.count());

  test_bvector.clear();
}

npl::i32 main(npl::i32 argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}