#include <gtest/gtest.h>
#include <vector>
#include "parser.hpp"

TEST(TEST_PARSER, VALID_FILE1)
{
  std::ifstream in{"../csv_tests/test1.csv"};
  nop::csv::Parser<nop::csv::DefaultCfg, int32_t, std::string> prs{in, 1};
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
  std::ifstream in{"../csv_tests/test2.csv"};
  nop::csv::Parser<nop::csv::DefaultCfg, std::string, int32_t, std::string> prs{in, 1};
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
  std::ifstream in{"../csv_tests/test8.csv"};

  struct MyCfg
  {
  public:
    enum Symbol : char
    {
      Column = '.',
      Row = '*',
      Escape = '$'
    };
  };

  nop::csv::Parser<MyCfg, int32_t, std::string> prs{in, 1};
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
  std::ifstream in{"../csv_tests/test3.csv"};
  try
  {
    nop::csv::Parser<nop::csv::DefaultCfg, int32_t, std::string> prs{in, 0};
    std::tuple<int32_t, std::string> t;
    EXPECT_EQ(t, *(prs.begin()));
  }
  catch (const nop::err::BaseException& error)
  {
    FAIL();
  }
}

TEST(TEST_PARSER, SKIP_LINES1)
{
  std::ifstream in{"../csv_tests/test1.csv"};
  nop::csv::Parser<nop::csv::DefaultCfg, int32_t, std::string> prs{in, 10};
  std::tuple<int32_t, std::string> t;
  EXPECT_EQ(t, *(prs.begin()));
}

TEST(TEST_PARSER, SKIP_LINES2)
{
  std::ifstream in{"../csv_tests/test5.csv"};
  nop::csv::Parser<nop::csv::DefaultCfg, std::string, int32_t, std::string> prs{in, 144};
  auto t{std::make_tuple<std::string, int32_t, std::string>("ifndef", 1, "0.69444%")};
  EXPECT_EQ(t, *(prs.begin()));
}

TEST(TEST_PARSER, INVALID_FILE)
{
  std::ifstream in{"../csv_tests/test4.csv"};
  try
  {
    nop::csv::Parser<nop::csv::DefaultCfg, int32_t, std::string> prs{in, 1};
  }
  catch (const nop::err::InvalidArgument& error)
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
  std::ifstream in{"../csv_tests/test1.csv"};
  try
  {
    nop::csv::Parser<nop::csv::DefaultCfg, std::string, int32_t, std::string> prs{in, 1};
  }
  catch (const nop::err::FormatError& error)
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
  std::ifstream in{"../csv_tests/test6.csv"};
  nop::csv::Parser<nop::csv::DefaultCfg, int32_t, std::string> prs{in, 1};
  EXPECT_THROW(
      {
        auto&& e{prs.end()};
        for (auto&& b{prs.begin()}; b != e; ++b)
          __asm__ volatile ("");
      }
      , nop::err::FormatError);
}

TEST(TEST_PARSER, INVALID_FILE_FORMAT3)
{
  std::ifstream in{"../csv_tests/test7.csv"};
  nop::csv::Parser<nop::csv::DefaultCfg, int32_t, std::string> prs{in, 1};
  EXPECT_THROW(
      {
        auto&& e{prs.end()};
        for (auto&& b{prs.begin()}; b != e; ++b)
          asm volatile ("");
      }
      , nop::err::FormatError);
}
