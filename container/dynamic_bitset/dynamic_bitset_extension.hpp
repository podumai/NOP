#ifndef NOP_CONTAINER_DYNAMIC_BITSET_EXTENSION_HPP /* Begin nop::container::dynamic_bitset extensions header file */
#define NOP_CONTAINER_DYNAMIC_BITSET_EXTENSION_HPP 1UL

#define byte_division(bits, suff) ((bits) >> (6 ## suff))
#define byte_module(bits, suff) ((bits) & (63 ## suff))
#define ZERO_VALUE (0UL)
#define BIT_SET (true)
#define BIT_UNSET (false)
#define ANY_SET (true)
#define NONE_SET (true)

#endif /* End nop::container::dynamic_bitset extensions header file */
