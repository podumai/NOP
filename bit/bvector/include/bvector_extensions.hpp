#ifndef __BITS_BVECTOR_EXTENSIONS__
#define __BITS_BVECTOR_EXTENSIONS__ 1

#define byte_division(bits) ((bits) >> 3)
#define byte_module(bits) ((bits) & static_cast<size_type>(0b00000111))

#define __EXCEPTION_HANDLER(statement) \
try { statement; } catch (const std::exception& error) { throw; }

#define EMPTY_STORAGE (nullptr)
#define ZERO_VALUE (static_cast<size_type>(0))
#define BIT_SET (true)
#define BIT_UNSET (false)
#define ANY_SET (true)
#define NONE_SET (true)

#endif