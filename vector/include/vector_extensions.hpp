#ifndef NOP_BITS_VECTOR_EXTENSIONS_HPP
#define NOP_BITS_VECTOR_EXTENSIONS_HPP 1

#define byteDivision(bits) ((bits) >> 3UL)
#define byteModule(bits) ((bits) & 0b00000111UL)
#define EMPTY_STORAGE (nullptr)
#define ZERO_VALUE (0UL)
#define BIT_SET (true)
#define BIT_UNSET (false)
#define ANY_SET (true)
#define NONE_SET (true)

#endif
