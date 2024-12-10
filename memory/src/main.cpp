#include <iostream>
#include "smart_ptr.hpp"

nop::i32 main()
{
  auto ptr{nop::make_sptr<nop::i32>(120)};
  std::cout << *ptr << '\n';
  return 0;
}
