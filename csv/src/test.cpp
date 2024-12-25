#include <gtest/gtest.h>
#include <vector>
#include "parser.hpp"

TEST(TEST_PARSER, VALID_FILE1)
{
  std::ifstream in{"csv/test1.csv"};
  csv::parser<int32_t, std::string> prs{in, 1};
  std::vector<std::tuple<int32_t, std::string>> vals{{120, "another1"},
                                                     {52, "another2"}};
  size_t counter{};
  for (auto&& t : prs)
  {
    EXPECT_EQ(vals[counter], t);
    ++counter;
  }
}

TEST(TEST_PARSER, VALID_FILE2)
{
  std::ifstream in{"csv/test2.csv"};
  csv::parser<std::string, int32_t, std::string> prs{in, 1};
  std::vector<std::tuple<std::string, int32_t, std::string>> vals{
                                     {"another1", 120, "another1"},
                                     {"another2", 52, "another2"},
                                     {"another3", 60, "another3"}};
  size_t counter{};
  for (auto&& t : prs)
  {
    EXPECT_EQ(vals[counter], t);
    ++counter;
  }
}

TEST(TEST_PARSER, CONFIGURATION)
{
  std::ifstream in{"csv/test8.csv"};
  csv::parser<int32_t, std::string> prs{in, 1, {'.','*','$'}};
  std::vector<std::tuple<int32_t, std::string>> vals{{120, "another1 120 another2"},
                                                     {52, "another2"}};
  size_t counter{};
  for (auto&& t : prs)
  {
    EXPECT_EQ(vals[counter], t);
    ++counter;
  }
}

TEST(TEST_PARSER, EMPTY_FILE)
{
  std::ifstream in{"csv/test3.csv"};
  try
  {
    csv::parser<int32_t, std::string> prs{in, 0};
    std::tuple<int32_t, std::string> t;
    EXPECT_EQ(t, *(prs.begin()));
  }
  catch (const csv::err::base_exception& error)
  {
    FAIL();
  }
}

TEST(TEST_PARSER, SKIP_LINES1)
{
  std::ifstream in{"csv/test1.csv"};
  csv::parser<int32_t, std::string> prs{in, 10};
  std::tuple<int32_t, std::string> t;
  EXPECT_EQ(t, *(prs.begin()));
}

TEST(TEST_PARSER, SKIP_LINES2)
{
  std::ifstream in{"csv/test5.csv"};
  csv::parser<std::string, int32_t, std::string> prs{in, 144};
  auto t{std::make_tuple<std::string, int32_t, std::string>("ifndef", 1, "0.69444%")};
  EXPECT_EQ(t, *(prs.begin()));
}

TEST(TEST_PARSER, INVALID_FILE)
{
  std::ifstream in{"csv/test4.csv"};
  try
  {
    csv::parser<int32_t, std::string> prs{in, 1};
  }
  catch (const csv::err::invalid_argument& error)
  {
    SUCCEED();
  }
  catch (...)
  {
    FAIL();
  }
}

TEST(TEST_PARSER, INVALID_FILE_FORMAT1)
{
  std::ifstream in{"csv/test1.csv"};
  try
  {
    csv::parser<std::string, int32_t, std::string> prs{in, 1};
  }
  catch (const csv::err::format_error& error)
  {
    SUCCEED();
  }
  catch (...)
  {
    FAIL();
  }
}

TEST(TEST_PARSER, INVALID_FILE_FORMAT2)
{
  std::ifstream in{"csv/test6.csv"};
  csv::parser<int32_t, std::string> prs{in, 1};
  EXPECT_THROW(
      {
        auto&& e{prs.end()};
        for (auto&& b{prs.begin()}; b != e; ++b)
          asm volatile ("");
      }
      , csv::err::format_error);
}

TEST(TEST_PARSER, INVALID_FILE_FORMAT3)
{
  std::ifstream in{"csv/test7.csv"};
  csv::parser<int32_t, std::string> prs{in, 1};
  EXPECT_THROW(
      {
        auto&& e{prs.end()};
        for (auto&& b{prs.begin()}; b != e; ++b)
          asm volatile ("");
      }
      , csv::err::format_error);
}

int32_t main(int32_t argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
