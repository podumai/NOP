#define __BASE__ (10)
#define __ERROR__ "\033[1;34m[ERROR]\033[0m"
#define __MESSAGE__ "\033[1;37m[MESSAGE]\033[0m"
#define __RESULT__ "\033[1;35m[RESULT]\033[0m"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include "blis.h"

static void check_args(int32_t, char**);
extern void inverse(obj_t*, obj_t*, size_t);

[[maybe_unused]] static void print_matrix(const float* matrix, size_t size)
{
  size_t aligned_size = size + (64 - (size & 63));

  if (size <= 16)
  {
    for (size_t i = 0; i != size; ++i)
    {
      printf("Row[%zu] ", i);
      for (size_t j = 0; j != size; ++j)
        printf("%7.5f ", matrix[i * aligned_size + j]);
      putchar('\n');
    }
  }
  else
  {
    for (size_t i = 0; i != 4; ++i)
    {
      printf("Row[%zu] ", i);
      for (size_t j = 0; j != 8; ++j)
        printf("%7.5f ", matrix[i * aligned_size + j]);
      putchar('\n');
    }
    for (size_t i = size - 4; i != size; ++i)
    {
      printf("Row[%zu] ", i);
      for (size_t j = size - 8; j != size; ++j)
        printf("%7.5f ", matrix[i * aligned_size + j]);
      putchar('\n');
    }
  }
}

int32_t main(int32_t argc, char* argv[])
{
  srand(time(NULL));
  check_args(argc, argv);

  size_t size = strtoull(argv[1], NULL, __BASE__);
  size_t iterations = strtoull(argv[2], NULL, __BASE__);

  obj_t A;
  bli_obj_create(BLIS_FLOAT, size, size, 0, 0, &A);
  bli_obj_set_struc(BLIS_GENERAL, &A);
  bli_obj_set_uplo(BLIS_DENSE, &A);
  bli_obj_set_onlytrans(BLIS_NO_TRANSPOSE, &A);
  bli_obj_set_conj(BLIS_NO_CONJUGATE, &A);
  bli_randm(&A);

  obj_t B;
  bli_obj_create(BLIS_FLOAT, size, size, 0, 0, &B);
  bli_obj_set_struc(BLIS_GENERAL, &B);
  bli_obj_set_uplo(BLIS_DENSE, &B);
  bli_obj_set_onlytrans(BLIS_NO_TRANSPOSE, &B);
  bli_obj_set_conj(BLIS_NO_CONJUGATE, &B);

  double t_save = bli_clock();
  inverse(&A, &B, iterations);
  t_save = bli_clock_min_diff(DBL_MAX, t_save);
  printf("Inverse matrix computation: %f\n", t_save);
  print_matrix(bli_obj_buffer(&B), bli_obj_length(&B));

  bli_obj_free(&A);
  bli_obj_free(&B);

  return EXIT_SUCCESS;
}

static void check_args(int32_t argc, char** argv)
{
  if (argc != 3)
  {
    fprintf(stderr
            , "%s Invalid number of arguments\n"
              "%s Two parameters was required : <size> <iterations>\n"
              "%s Terminating process...\n"
            , __ERROR__
            , __MESSAGE__
            , __RESULT__);
    exit(EXIT_FAILURE);
  }
  
  bool valid_input = true;
  int32_t check_var = atoi(argv[1]);

  if (check_var <= 0)
  {
    fprintf(stderr
            , "%s Invalid argument\n"
              "%s The size of the matrix cannot be negative or equal to zero\n"
            , __ERROR__
            , __MESSAGE__);
    valid_input = false;
  }

  check_var = atoi(argv[2]);

  if (check_var < 0)
  {
    fprintf(stderr
            , "%s Invalid argument\n"
              "%s The number of iterations cannot be negative\n"
            , __ERROR__
            , __MESSAGE__);
    valid_input = false;
  }

  if (valid_input == false)
  {
    fprintf(stderr
            , "%s Errors occurred\n%s Terminating process\n"
            , __MESSAGE__
            , __RESULT__);
    exit(EXIT_FAILURE);
  }
}
