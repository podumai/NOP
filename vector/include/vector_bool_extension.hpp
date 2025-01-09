#ifndef NOP_VECTOR_BOOL_EXTENSIONS_HPP /* Begin vector bool extensions header file */
#define NOP_VECTOR_BOOL_EXTENSIONS_HPP 1

#define byteDivision(bits, suff) ((bits) >> (6 ## suff))
#define byteModule(bits, suff) ((bits) & (63 ## suff))
#define ZERO_VALUE (0UL)
#define BIT_SET (true)
#define BIT_UNSET (false)
#define ANY_SET (true)
#define NONE_SET (true)

#endif /* End vector bool extensions header file */
