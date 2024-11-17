#include <iostream>
#include <type_traits>
#include "smart_ptr.hpp"
#include "types.hpp"

nop::i32 main()
{
  nop::smart_ptr<nop::i32> test(new nop::i32(52));

  if (test)
    std::cout << *test << std::endl;

  return 0;
}
