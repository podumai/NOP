#define __AVX_ALIGN__ (64UL)
#define __AVX_STEP__ (16UL)

#define __ERROR__ "\033[1;35m[ERROR]\033[0m"
#define __RESULT__ "\033[1;35m[RESULT]\033[0m"

#define compute_aligned_size(size) ((size) + (__AVX_ALIGN__ - ((size) & 63)))

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <float.h>
#include <math.h>
#include <immintrin.h>
#include "matrix.h"

[[nodiscard]] f32* create_matrix(size_t size)
{
  size_t aligned_size = compute_aligned_size(size);
  aligned_size *= aligned_size;
  f32* matrix = aligned_alloc(__AVX_ALIGN__, aligned_size << 2);

  if (matrix == NULL)
  {
    fprintf(stderr,
            "%s Cannot allocate memory\n"
            "%s Terminating process...\n",
            __ERROR__,
            __RESULT__);
    exit(EXIT_FAILURE);
  }

  f32* end = matrix + aligned_size;

  for (register f32* begin = matrix; begin < end; begin += __AVX_STEP__)
    _mm512_store_ps(begin, _mm512_setzero_ps());

  return matrix;
}

[[nodiscard]] f32* create_identity(size_t size)
{
  f32* matrix = __builtin_assume_aligned(create_matrix(size), __AVX_ALIGN__);
  size_t aligned_size = compute_aligned_size(size);

  for (size_t i = 0; i != size; ++i)
    matrix[i * aligned_size + i] = 1.0f;

  return matrix;
}

[[nodiscard]] f32* transpose(f32* matrix, size_t size)
{
  f32* t_matrix = __builtin_assume_aligned(create_matrix(size), __AVX_ALIGN__);
  size_t aligned_size = compute_aligned_size(size);

  for (size_t i = 0; i != size; ++i)
    for (size_t j = 0; j != size; ++j)
      t_matrix[j * aligned_size + i] = matrix[i * aligned_size + j];

  return t_matrix;
}

[[nodiscard]] static f32* create_B(f32* matrix, size_t size)
{
  f32* B = __builtin_assume_aligned(transpose(matrix, size), __AVX_ALIGN__);
  f32 scalar = 1 / (compute_norm(ROW, matrix, size) *
                    compute_norm(ROW, B, size));
  muls_matrix(B, size, scalar);

  return B;
}

[[nodiscard]] static f32* create_R(f32* B, f32* A, size_t size)
{
  f32* R = __builtin_assume_aligned(create_matrix(size), __AVX_ALIGN__);
  size_t aligned_size = compute_aligned_size(size);

  mul_matrix(B, A, R, size);
  muls_matrix(R, size, -1.0f);
  
  for (size_t i = 0; i != size; ++i)
    R[i * aligned_size + i] += 1.0f;

  return R;
}

[[nodiscard]] f32* inverse(f32* A, size_t N, size_t M)
{
  if (M == 0)
    return create_matrix(N);

  f32* B = __builtin_assume_aligned(create_B(A, N), __AVX_ALIGN__);
  f32* R = __builtin_assume_aligned(create_R(B, A, N), __AVX_ALIGN__);
  f32* R_curr = __builtin_assume_aligned(create_matrix(N), __AVX_ALIGN__);
  copy_matrix(R, R_curr, N);
  f32* R_prev = __builtin_assume_aligned(create_identity(N), __AVX_ALIGN__);
  f32* accum = __builtin_assume_aligned(create_identity(N), __AVX_ALIGN__);

  for (size_t i = 1; i != M; ++i)
  {
    sum_matrix(accum, R_curr, accum, N);
    copy_matrix(R_curr, R_prev, N);
    mul_matrix(R_prev, R, R_curr, N);
  }

  free(R);
  free(R_curr);
  free(R_prev);

  f32* inv_A = __builtin_assume_aligned(create_matrix(N), __AVX_ALIGN__);
  mul_matrix(accum, B, inv_A, N);

  free(B);
  free(accum);

  return inv_A;
}

void fill_matrix(f32* matrix, size_t size)
{
  matrix = __builtin_assume_aligned(matrix, __AVX_ALIGN__);
  size_t aligned_size = compute_aligned_size(size);
  f32* end = matrix + aligned_size * aligned_size;

  for (; matrix < end; matrix += __AVX_STEP__)
    _mm512_store_ps(matrix, _mm512_set_ps(rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15,
                                          rand() & 15));
}


void mul_matrix(const f32* lhs,
                const f32* rhs,
                f32* __restrict__ res, size_t size)
{
  lhs = __builtin_assume_aligned(lhs, __AVX_ALIGN__);
  rhs = __builtin_assume_aligned(rhs, __AVX_ALIGN__);
  res = __builtin_assume_aligned(res, __AVX_ALIGN__);
  size_t aligned_size = compute_aligned_size(size);

  for (size_t i = 0; i != size; ++i)
  {
    register f32* tmp_res = res + i * aligned_size;

    for (size_t j = 0; j < size; j += __AVX_STEP__)
      _mm512_store_ps(tmp_res + j, _mm512_setzero_ps());

    for (size_t j = 0; j != size; ++j)
    {
      __m512 tmp_lhs = _mm512_set1_ps(lhs[i * aligned_size + j]);
      register const f32* tmp_rhs = rhs + j * aligned_size;

      for (register size_t k = 0; k < size; k += __AVX_STEP__)
        _mm512_store_ps(tmp_res + k,
                        _mm512_fmadd_ps(tmp_lhs,
                                        _mm512_load_ps(tmp_rhs + k),
                                        _mm512_load_ps(tmp_res + k)));
    }
  }
}

void muls_matrix(f32* matrix, size_t size, f32 value)
{
  matrix = __builtin_assume_aligned(matrix, __AVX_ALIGN__);
  size_t aligned_size = compute_aligned_size(size);
  f32* end = matrix + aligned_size * aligned_size;

  while (matrix < end)
  {
    _mm512_store_ps(matrix,
                    _mm512_mul_ps(_mm512_load_ps(matrix),
                                  _mm512_set1_ps(value)));
    matrix += __AVX_STEP__;
  }
}

void sum_matrix(f32* lhs, f32* rhs, f32* res, size_t size)
{
  lhs = __builtin_assume_aligned(lhs, __AVX_ALIGN__);
  rhs = __builtin_assume_aligned(rhs, __AVX_ALIGN__);
  res = __builtin_assume_aligned(res, __AVX_ALIGN__);
  size_t aligned_size = compute_aligned_size(size);
  f32* end = lhs + aligned_size * aligned_size;

  while (lhs < end)
  {
    _mm512_store_ps(res,
                    _mm512_add_ps(_mm512_load_ps(lhs),
                                  _mm512_load_ps(rhs)));
    lhs += __AVX_STEP__;
    rhs += __AVX_STEP__;
    res += __AVX_STEP__;
  }
}

void copy_matrix(const f32* __restrict__ src, f32* __restrict__ dst, size_t size)
{
  src = __builtin_assume_aligned(src, __AVX_ALIGN__);
  dst = __builtin_assume_aligned(dst, __AVX_ALIGN__);
  size_t aligned_size = compute_aligned_size(size);
  const f32* end = src + aligned_size * aligned_size;

  
  while (src < end)
  {
    _mm512_store_ps(dst, _mm512_load_ps(src));
    src += __AVX_STEP__;
    dst += __AVX_STEP__;
  }
}

[[nodiscard]] f32 compute_norm(NORM_DIRECTION df, const f32* matrix, size_t size)
{
  f32 max_norm = DBL_MIN;
  size_t aligned_size = compute_aligned_size(size);

  if (df == ROW)
  {
    for (size_t i = 0; i != size; ++i)
    {
      f32 current_norm = 0.0f;
      const f32* curr_matrix = matrix + i * aligned_size;

      for (size_t j = 0; j != size; ++j)
        current_norm += fabs(curr_matrix[j]);

      if (current_norm > max_norm)
        max_norm = current_norm;
    }
  }
  else if (df == COLUMN)
  {
    for (size_t i = 0; i != size; ++i)
    {
      f32 current_norm = 0.0f;
      const f32* curr_matrix = matrix + i;

      for (size_t j = 0; j != size; ++j)
        current_norm += fabs(curr_matrix[j * aligned_size]);

      if (current_norm > max_norm)
        max_norm = current_norm;
    }
  }

  return max_norm;
}
