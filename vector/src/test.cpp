#include <gtest/gtest.h>
#include "vector.hpp"

class VECTOR_TEST_FIXTURE : public ::testing::Test
{
protected:
  nop::Vector<bool> emptyVector;
  nop::Vector<bool> filledVector{16UL, 0xffffUL};
};

TEST_F(VECTOR_TEST_FIXTURE, CONSTRUCTOR_TEST)
{
  EXPECT_EQ(0UL, emptyVector.size())
  << "Empty object must be initialized with zero size";
  EXPECT_EQ(0UL, emptyVector.capacity())
  << "Empty object must be initialized with zero capacity";
  EXPECT_EQ(nullptr, emptyVector.data())
  << "Empty object must be initialized with empty storage";

  EXPECT_EQ(16UL, filledVector.size());
  EXPECT_EQ(2UL, filledVector.capacity());
  ASSERT_NE(nullptr, filledVector.data());
  EXPECT_EQ("1111111111111111", filledVector.toString())
  << "Vector was initialized with 0xffff which is 0b1111111111111111";
}

TEST_F(VECTOR_TEST_FIXTURE, COPY_CONSTRUCTOR_TEST)
{
  auto testVector{filledVector};

  EXPECT_EQ(testVector.size(), filledVector.size());
  EXPECT_EQ(testVector.capacity(), filledVector.capacity());
  EXPECT_EQ(testVector.toString(), filledVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, MOVE_CONSTRUCTOR_TEST)
{
  auto testVector(std::move(filledVector));

  EXPECT_EQ(16UL, testVector.size());
  EXPECT_EQ(2UL, testVector.capacity());
  ASSERT_NE(nullptr, testVector.data());
  EXPECT_EQ("1111111111111111", testVector.toString());

  EXPECT_EQ(emptyVector.size(), filledVector.size())
  << "Moved object must be empty with zero size";
  EXPECT_EQ(emptyVector.capacity(), filledVector.capacity())
  << "Moved object must be empty with zero capacity";
  EXPECT_EQ(emptyVector.data(), filledVector.data())
  << "Moved object must be empty with nullptr storage";
}

TEST_F(VECTOR_TEST_FIXTURE, SIZE_METHOD_TEST)
{
  EXPECT_EQ(0UL, emptyVector.size());
  EXPECT_EQ(16UL, filledVector.size());
}

TEST_F(VECTOR_TEST_FIXTURE, CAPACITY_METHOD_TEST)
{
  EXPECT_EQ(0UL, emptyVector.capacity());
  EXPECT_EQ(2UL, filledVector.capacity());
}

TEST_F(VECTOR_TEST_FIXTURE, DATA_METHOD_TEST)
{
  EXPECT_EQ(nullptr, emptyVector.data());
  EXPECT_NE(nullptr, filledVector.data());
}

TEST_F(VECTOR_TEST_FIXTURE, MAX_SIZE_METHOD_TEST)
{
  EXPECT_EQ(nop::vectorLimits::MAX_SIZE, emptyVector.maxSize());
}

TEST_F(VECTOR_TEST_FIXTURE, COUNT_METHOD_TEST)
{
  EXPECT_EQ(0UL, emptyVector.count())
  << "Count on empty object must return zero";
  EXPECT_EQ(16UL, filledVector.count());
}

TEST_F(VECTOR_TEST_FIXTURE, RESERVE_METHOD)
{
  EXPECT_THROW(emptyVector.reserve(nop::vectorLimits::MAX_SIZE),
               std::length_error);
  emptyVector.reserve(10UL);

  EXPECT_EQ(0UL, emptyVector.size());
  ASSERT_EQ(10UL, emptyVector.capacity())
  << "Invalid reservation of space which must be: previous + new <=> 0 + 10 = 10";
  ASSERT_NE(nullptr, emptyVector.data());

  filledVector.reserve(10UL);

  EXPECT_EQ(16UL, filledVector.size());
  ASSERT_EQ(12UL, filledVector.capacity())
  << "Invalid reservation of space which must be: previous + new <=> 2 + 10 = 12";
  ASSERT_NE(nullptr, filledVector.data());
}

TEST_F(VECTOR_TEST_FIXTURE, SHRINK_TO_FIT_METHOD)
{
  emptyVector.reserve(10UL);
  emptyVector.shrinkToFit();

  EXPECT_EQ(0UL, emptyVector.size())
  << "Shrink to fit must not modify the number of bits";
  EXPECT_EQ(0UL, emptyVector.capacity())
  << "Zero number of bits means empty object and in this case it must clear the object";
  EXPECT_EQ(nullptr, emptyVector.data())
  << "Nullptr must be set after cleaning the object";

  filledVector.shrinkToFit();

  EXPECT_EQ(16UL, filledVector.size())
  << "Shrink to fit must not modify the number of bits";
  EXPECT_EQ(2UL, filledVector.capacity())
  << "If object does not contain extra space it must not change object state";
  EXPECT_NE(nullptr, filledVector.data())
  << "Nullptr must be set if object is empty but 'filledVector' is not";
}

TEST_F(VECTOR_TEST_FIXTURE, ANY_METHOD)
{
  EXPECT_EQ(false, emptyVector.any())
  << "Empty object can not contain any bits";
  EXPECT_EQ(true, filledVector.any());
}

TEST_F(VECTOR_TEST_FIXTURE, NONE_METHOD)
{
  EXPECT_EQ(true, emptyVector.none())
  << "Empty object can not contain any bits";
  EXPECT_EQ(false, filledVector.none());
}

TEST_F(VECTOR_TEST_FIXTURE, EMPTY_METHOD)
{
  EXPECT_EQ(true, emptyVector.empty())
  << "Empty object can not contain any bits";
  EXPECT_EQ(false, filledVector.empty());
}

TEST_F(VECTOR_TEST_FIXTURE, CLEAR_METHOD)
{
  emptyVector.clear();
  filledVector.clear();

  EXPECT_EQ(emptyVector.size(), filledVector.size())
  << "Every object must be empty after clear";
  EXPECT_EQ(emptyVector.capacity(), filledVector.capacity())
  << "Every object must be empty after clear";
  EXPECT_EQ(emptyVector.data(), filledVector.data())
  << "Every object must be empty after clear";
}

TEST_F(VECTOR_TEST_FIXTURE, RESIZE_METHOD)
{
  emptyVector.resize(10UL, false);
  filledVector.resize(20UL, true);

  EXPECT_EQ(10UL, emptyVector.size())
  << "Resize must modify the number of bits";
  EXPECT_EQ(2UL, emptyVector.capacity())
  << "Resize must modify the number of bytes";
  ASSERT_NE(nullptr, emptyVector.data());
  EXPECT_EQ(true, emptyVector.none())
  << "Bits must be unset due to second parameter -> false";

  EXPECT_EQ(20UL, filledVector.size());
  EXPECT_EQ(3UL, filledVector.capacity());
  ASSERT_NE(nullptr, filledVector.data());
  EXPECT_EQ(20UL, filledVector.count())
  << "New bits must be set due to second parameter -> true";
}

TEST_F(VECTOR_TEST_FIXTURE, pushBack_METHOD)
{
  for (nop::size_t i{}; i != 10UL; ++i)
  {
    emptyVector.pushBack(true);
    filledVector.pushBack(true);
  }

  EXPECT_EQ(10UL, emptyVector.count());
  EXPECT_EQ(26UL, filledVector.count());
}

TEST_F(VECTOR_TEST_FIXTURE, popBack_METHOD)
{
  EXPECT_THROW(emptyVector.popBack(), std::out_of_range);

  for (nop::size_t i {}; i != 16UL; ++i)
  {
    filledVector.popBack();
  }
  EXPECT_EQ(0UL, filledVector.size());
}

TEST_F(VECTOR_TEST_FIXTURE, SET_INDEX_METHOD)
{
  EXPECT_THROW(emptyVector.set(10UL), std::out_of_range);
  
  for (nop::size_t i{}; i != 4UL; ++i)
  {
    filledVector.set(i, false);
  }
  
  EXPECT_EQ("0000111111111111", filledVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, SET_METHOD)
{
  EXPECT_THROW(emptyVector.set(), std::out_of_range);

  nop::Vector<bool> testVector{16UL};
  testVector.set();

  EXPECT_EQ(false, testVector.none())
  << "Set method must set all bits to true";
}

TEST_F(VECTOR_TEST_FIXTURE, RESET_INDEX_METHOD)
{
  EXPECT_THROW(emptyVector.reset(10UL), std::out_of_range);

  for (nop::size_t i{}; i != 4UL; ++i)
  {
    filledVector.reset(i);
  }
  
  EXPECT_EQ("0000111111111111", filledVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, RESET_METHOD)
{
  EXPECT_THROW(emptyVector.reset(), std::out_of_range);

  filledVector.reset();

  EXPECT_EQ(true, filledVector.none())
  << "Reset method must set all bits to false";
}

TEST_F(VECTOR_TEST_FIXTURE, FLIP_INDEX_METHOD)
{
  EXPECT_THROW(emptyVector.flip(10UL), std::out_of_range);

  for (nop::size_t i{}; i != 4UL; ++i)
  {
    filledVector.flip(i);
  }
  
  EXPECT_EQ("0000111111111111", filledVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, FLIP_METHOD)
{
  EXPECT_THROW(emptyVector.flip(), std::out_of_range);

  filledVector.flip();

  EXPECT_EQ(true, filledVector.none());
}

TEST_F(VECTOR_TEST_FIXTURE, SWAP_METHOD)
{
  emptyVector.swap(filledVector);

  EXPECT_EQ(16UL, emptyVector.size());
  EXPECT_EQ(2UL, emptyVector.capacity());
  EXPECT_EQ(16UL, emptyVector.count());
  EXPECT_EQ(0UL, filledVector.size());
  EXPECT_EQ(0UL, filledVector.capacity());
  EXPECT_EQ(nullptr, filledVector.data());
}

TEST_F(VECTOR_TEST_FIXTURE, SUBSCRIPT_OPERATOR_TEST)
{
  for (nop::size_t i{}; i != 10UL; ++i)
  {
    ASSERT_EQ(true, filledVector[i]);
    filledVector[i] = false;
    ASSERT_EQ(false, filledVector[i]);
    filledVector[i] = true;
    ASSERT_EQ(true, filledVector[i]);
  }
}

TEST_F(VECTOR_TEST_FIXTURE, AT_METHOD)
{
  EXPECT_THROW((void)emptyVector.at(10UL), std::out_of_range);
  EXPECT_THROW((void)filledVector.at(20UL), std::out_of_range);

  EXPECT_EQ(true, filledVector.at(0UL));
  filledVector.reset(0UL);
  EXPECT_EQ(false, filledVector.at(0UL));
}

TEST_F(VECTOR_TEST_FIXTURE, FRONT_METHOD)
{
  EXPECT_THROW((void)emptyVector.front(), std::out_of_range);

  EXPECT_EQ(true, filledVector.front());
  filledVector.reset(0UL);
  EXPECT_EQ(false, filledVector.front());
}

TEST_F(VECTOR_TEST_FIXTURE, BACK_METHOD)
{
  EXPECT_THROW((void)emptyVector.back(), std::out_of_range);

  EXPECT_EQ(true, filledVector.back());
  filledVector.reset(15UL);
  EXPECT_EQ(false, filledVector.back());
}

TEST_F(VECTOR_TEST_FIXTURE, COPY_ASSIGNMENT_OPERATOR)
{
  emptyVector = filledVector;

  EXPECT_EQ(emptyVector.size(), filledVector.size());
  EXPECT_EQ(emptyVector.capacity(), filledVector.capacity());
  ASSERT_NE(nullptr, emptyVector.data());
  EXPECT_EQ(emptyVector.toString(), filledVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, MOVE_ASSIGNMENT_OPERATOR)
{
  emptyVector = std::move(filledVector);

  EXPECT_EQ(16UL, emptyVector.size());
  EXPECT_EQ(2UL, emptyVector.capacity());
  EXPECT_EQ(16UL, emptyVector.count());
  EXPECT_EQ(0UL, filledVector.size());
  EXPECT_EQ(0UL, filledVector.capacity());
  EXPECT_EQ(nullptr, filledVector.data());
}

TEST_F(VECTOR_TEST_FIXTURE, BITWISE_AND_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(emptyVector &= filledVector, std::invalid_argument);

  emptyVector.resize(16UL, true);
  filledVector &= emptyVector;

  ASSERT_EQ(16UL, filledVector.size());
  ASSERT_EQ(2UL, filledVector.capacity());
  ASSERT_EQ(16UL, filledVector.count());

  emptyVector.reset();
  filledVector &= emptyVector;

  EXPECT_EQ(16UL, filledVector.size());
  EXPECT_EQ(2UL, filledVector.capacity());
  EXPECT_EQ(true, filledVector.none());
}

TEST_F(VECTOR_TEST_FIXTURE, BITWISE_OR_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(emptyVector |= filledVector, std::invalid_argument);

  emptyVector.resize(16UL, true);
  filledVector |= emptyVector;

  ASSERT_EQ(16UL, filledVector.size());
  ASSERT_EQ(2UL, filledVector.capacity());
  ASSERT_EQ(16UL, filledVector.count());

  emptyVector.reset();
  filledVector |= emptyVector;

  EXPECT_EQ(16UL, filledVector.size());
  EXPECT_EQ(2UL, filledVector.capacity());
  EXPECT_EQ(false, filledVector.none());
}

TEST_F(VECTOR_TEST_FIXTURE, BITWISE_XOR_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(emptyVector ^= filledVector, std::invalid_argument);

  emptyVector.resize(16UL, true);
  filledVector ^= emptyVector;

  ASSERT_EQ(16UL, filledVector.size());
  ASSERT_EQ(2UL, filledVector.capacity());
  ASSERT_EQ(true, filledVector.none());

  filledVector ^= emptyVector;

  EXPECT_EQ(16UL, filledVector.size());
  EXPECT_EQ(2UL, filledVector.capacity());
  EXPECT_EQ(16UL, filledVector.count());
}

TEST_F(VECTOR_TEST_FIXTURE, BITWISE_INVERSE_OPERATOR_TEST)
{
  EXPECT_THROW((void)~emptyVector, std::out_of_range);

  emptyVector = ~filledVector;

  ASSERT_EQ(16UL, emptyVector.size());
  ASSERT_EQ(2UL, emptyVector.capacity());
  ASSERT_EQ(true, emptyVector.none());
}

TEST_F(VECTOR_TEST_FIXTURE, toString_METHOD_TEST)
{
  EXPECT_EQ("", emptyVector.toString());
  EXPECT_EQ("1111111111111111", filledVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, LHS_ASSIGNMENT_TEST)
{
  EXPECT_THROW(emptyVector >>= 10UL,
               std::out_of_range);

  filledVector >>= 0UL;

  ASSERT_EQ(16UL, filledVector.size());
  ASSERT_EQ(2UL, filledVector.capacity());
  ASSERT_EQ(16UL, filledVector.count());

  filledVector >>= 8UL;

  ASSERT_EQ("0000000011111111", filledVector.toString());

  filledVector >>= 8UL;

  ASSERT_EQ(true, filledVector.none());

  filledVector.set();
  filledVector >>= 20UL;

  ASSERT_EQ(true, filledVector.none());

  filledVector.set();
  filledVector >>= 5UL;

  EXPECT_EQ("0000011111111111", filledVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, RHS_ASSIGNMENT_TEST)
{
  EXPECT_THROW(emptyVector <<= 10UL, std::out_of_range);

  filledVector <<= 0UL;

  ASSERT_EQ(16, filledVector.size());
  ASSERT_EQ(2, filledVector.capacity());
  ASSERT_EQ(16, filledVector.count());

  filledVector <<= 8UL;

  ASSERT_EQ("1111111100000000", filledVector.toString());

  filledVector <<= 8UL;

  ASSERT_EQ(true, filledVector.none());

  filledVector.set();
  filledVector <<= 20UL;

  ASSERT_EQ(true, filledVector.none());
  filledVector.set();
  filledVector <<= 5UL;

  EXPECT_EQ("1111111111100000", filledVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, COPY_LHS_TEST)
{
  emptyVector = filledVector << 8UL;

  ASSERT_EQ("1111111100000000", emptyVector.toString());

  emptyVector = filledVector << 1UL;

  EXPECT_EQ("1111111111111110", emptyVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, COPY_RHS_TEST)
{
  emptyVector = filledVector >> 8UL;

  ASSERT_EQ("0000000011111111", emptyVector.toString());

  emptyVector = filledVector >> 1UL;

  EXPECT_EQ("0111111111111111", emptyVector.toString());
}

TEST_F(VECTOR_TEST_FIXTURE, ADVANCED_LHS_RHS_TEST)
{
  for (nop::size_t i{}; i != 16UL; ++i)
  {
    filledVector >>= 1UL;
    ASSERT_EQ(false, filledVector[i]);
  }

  filledVector.set();

  for (nop::size_t i{}; i != 16UL; ++i)
  {
    filledVector <<= 1UL;
    ASSERT_EQ(false, filledVector[15UL - i]);
  }

  filledVector.set();
  filledVector >>= 8UL;

  ASSERT_EQ("0000000011111111", filledVector.toString());

  filledVector >>= 3UL;

  ASSERT_EQ("0000000000011111", filledVector.toString());

  filledVector >>= 5UL;

  ASSERT_EQ(true, filledVector.none());

  filledVector.set();
  filledVector <<= 8UL;

  ASSERT_EQ("1111111100000000", filledVector.toString());

  filledVector <<= 3UL;

  ASSERT_EQ("1111100000000000", filledVector.toString());

  filledVector <<= 5UL;

  EXPECT_EQ(true, filledVector.none());
}

TEST_F(VECTOR_TEST_FIXTURE, ITERATOR_TEST)
{
  for (auto& iter : filledVector)
  {
    ASSERT_EQ(true, iter);
    iter = false;
    ASSERT_EQ(false, iter);
  }

  for (const auto& iter : filledVector)
    ASSERT_EQ(false, iter);

  for ([[maybe_unused]] auto& iter : emptyVector)
    ASSERT_EQ(false, true)
    << "Error in for range loop traversing empty object";
  
  for ([[maybe_unused]] const auto& iter : emptyVector)
    ASSERT_EQ(false, true)
    << "Error in for range loop (const) traversing empty object";
}

TEST_F(VECTOR_TEST_FIXTURE, STRESS_TEST)
{
  nop::Vector<bool> testVector;

  constexpr nop::size_t SIZE{nop::vectorLimits::MAX_SIZE >> 6UL};

  for (nop::size_t i{}; i != SIZE; ++i)
  {
    testVector.pushBack(true);
  }
  
  ASSERT_EQ(SIZE, testVector.count());

  for (nop::size_t i{}; i != SIZE; ++i)
    (void)testVector.popBack();
  
  ASSERT_EQ(true, testVector.none());

  testVector.shrinkToFit();

  constexpr nop::size_t MID_SIZE{SIZE >> 1UL};

  testVector.resize(SIZE, true);

  testVector >>= MID_SIZE;

  ASSERT_EQ(MID_SIZE, testVector.count());

  testVector <<= MID_SIZE;

  ASSERT_EQ(MID_SIZE, testVector.count());

  testVector.clear();
}
