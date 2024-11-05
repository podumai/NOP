#include <iostream>
#include "bitset.hpp"

npl::i32 main()
{
  npl::bit::bitset<8> test(0xff);
  
  for (npl::size_t i{}; i != test.size(); ++i)
  {
    std::cout << test[i] << ' ';
    test[i] = false;
    std::cout << test[i] << '\n';
  }

  return 0;
}