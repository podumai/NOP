#ifndef __NOP_MATRIX_H__
#define __NOP_MATRIX_H__ 1

typedef float f32;

typedef enum NORM_DIRECTION
{
  ROW = 1,
  COLUMN = 2
} NORM_DIRECTION;

f32* create_matrix(size_t);
f32* create_identity(size_t);
void delete_matrix(f32*);

f32* transpose(f32*, size_t);
f32* inverse(f32*, size_t, size_t);
void fill_matrix(f32*, size_t);
void mul_matrix(const f32*, const f32*, f32* __restrict__, size_t);
void muls_matrix(f32*, size_t, f32);
void sum_matrix(f32*, f32*, f32*, size_t);
void copy_matrix(const f32* __restrict__, f32* __restrict__, size_t);

f32 compute_norm(NORM_DIRECTION, const f32*, size_t);

#endif
