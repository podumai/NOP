#include <print>
#include <cinttypes>
#include "utility.hpp"
#include "types.hpp"

npl::i32 main()
{
  npl::i32 a[10]{};
  npl::i32 b[5]{};
  auto test{npl::utility::pair<npl::i32*, npl::i32*>(a, b)};
  std::println("{}, {}", *test.first, *test.second);

  return 0;
}