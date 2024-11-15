#include <gtest/gtest.h>
#include "bitset.hpp"

class TEST_BITSET_FIXTURE : public ::testing::Test
{
protected:
  nop::bit::bitset<8> empty_bitset;
  nop::bit::bitset<20> filled_bitset{0xffffffff};
};

TEST_F(TEST_BITSET_FIXTURE, CONSTRUCTOR_TEST)
{
  EXPECT_EQ(8, empty_bitset.size());
  EXPECT_EQ(false, empty_bitset.all());

  EXPECT_EQ(20, filled_bitset.size());
  EXPECT_EQ(true, filled_bitset.all());
}

TEST_F(TEST_BITSET_FIXTURE, MOVE_CONSTRUCTOR_TEST)
{
  EXPECT_EQ()
}

nop::i32 main()
{
  nop::bit::bitset<8> test(0xff);
  
  for (nop::size_t i{}; i != test.size(); ++i)
  {
    std::cout << test[i] << ' ';
    test[i] = false;
    std::cout << test[i] << '\n';
  }

  return 0;
}
