#include <iostream>
#include "bitset.hpp"

npl::i32 main()
{
  npl::bit::bitset<16> test(0xff);
  
  for (auto& iterator : test)
    std::cout << iterator;
  std::cout << '\n';
  
  return 0;
}