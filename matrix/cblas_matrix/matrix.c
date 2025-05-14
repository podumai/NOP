#include "blis.h"

static void create_B(obj_t* A, obj_t* B)
{
  bli_obj_create_conf_to(A, B);
  bli_obj_set_struc(BLIS_GENERAL, B);
  bli_obj_set_uplo(BLIS_DENSE, B);
  bli_copym(A, B);
  obj_t norm1;
  obj_t normi;
  bli_obj_create_1x1(BLIS_FLOAT, &norm1);
  bli_obj_create_1x1(BLIS_FLOAT, &normi);
  bli_norm1m(B, &norm1);
  bli_normim(B, &normi);
  bli_obj_set_onlytrans(BLIS_TRANSPOSE, B);
  bli_obj_set_conj(BLIS_NO_CONJUGATE, B);
  bli_invscalm(&norm1, B);
  bli_invscalm(&normi, B);
  bli_obj_free(&norm1);
  bli_obj_free(&normi);
}

static void create_R(obj_t* B, obj_t* A, obj_t* R)
{
  bli_obj_create_conf_to(A, R);
  bli_obj_set_struc(BLIS_GENERAL, R);
  bli_obj_set_uplo(BLIS_DENSE, R);
  bli_obj_set_onlytrans(BLIS_NO_TRANSPOSE, R);
  bli_obj_set_conj(BLIS_NO_CONJUGATE, R);
  bli_gemm(&BLIS_MINUS_ONE, B, A, &BLIS_ZERO, R);
  bli_shiftd(&BLIS_ONE, R);
}

void inverse(obj_t* A, obj_t* inv_A, size_t M)
{
  if (M == 0)
    return;

  dim_t size= bli_obj_length(A);
  obj_t B;
  create_B(A, &B);

  obj_t R;
  create_R(A, &B, &R);

  obj_t R_curr;
  bli_obj_create(BLIS_FLOAT, size, size, 0, 0, &R_curr);
  bli_obj_set_struc(BLIS_GENERAL, &R_curr);
  bli_obj_set_uplo(BLIS_DENSE, &R_curr);
  bli_obj_set_onlytrans(BLIS_NO_TRANSPOSE, &R_curr);
  bli_obj_set_conj(BLIS_NO_CONJUGATE, &R_curr);
  bli_copym(&R, &R_curr);

  obj_t R_prev;
  bli_obj_create(BLIS_FLOAT, size, size, 0, 0, &R_prev);
  bli_obj_set_struc(BLIS_GENERAL, &R_prev);
  bli_obj_set_uplo(BLIS_DENSE, &R_prev);
  bli_obj_set_onlytrans(BLIS_NO_TRANSPOSE, &R_prev);
  bli_obj_set_conj(BLIS_NO_CONJUGATE, &R_prev);
  bli_setd(&BLIS_ONE, &R_prev);

  obj_t accum;
  bli_obj_create(BLIS_FLOAT, size, size, 0, 0, &accum);
  bli_obj_set_struc(BLIS_GENERAL, &accum);
  bli_obj_set_uplo(BLIS_DENSE, &accum);
  bli_obj_set_onlytrans(BLIS_NO_TRANSPOSE, &accum);
  bli_obj_set_conj(BLIS_NO_CONJUGATE, &accum);
  bli_setd(&BLIS_ONE, &accum);

  for (size_t i = 1; i != M; ++i)
  {
    bli_addm(&R_curr, &accum);
    bli_copym(&R_curr, &R_prev);
    bli_gemm(&BLIS_ONE, &R_prev, &R, &BLIS_ZERO, &R_curr);
  }

  bli_obj_free(&R);
  bli_obj_free(&R_curr);
  bli_obj_free(&R_prev);
  bli_gemm(&BLIS_ONE, &accum, &B, &BLIS_ZERO, inv_A);
  bli_obj_free(&B);
  bli_obj_free(&accum);
}
