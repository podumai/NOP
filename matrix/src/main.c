#define __BASE__ (10)
#define __ERROR__ "\033[1;34m[ERROR]\033[0m"
#define __MESSAGE__ "\033[1;37m[MESSAGE]\033[0m"
#define __RESULT__ "\033[1;35m[RESULT]\033[0m"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <time.h>
#include "matrix.h"

static void check_args(int32_t, char**);
static void print_matrix(const f32*, size_t);

int32_t main(int32_t argc, char* argv[])
{
  srand(time(NULL));
  check_args(argc, argv);

  size_t size = strtoull(argv[1], NULL, __BASE__);
  size_t iterations = strtoull(argv[2], NULL, __BASE__);

  f32* matrix = create_matrix(size);
  fill_matrix(matrix, size);

  clock_t begin = clock();
  f32* inv_matrix = inverse(matrix, size, iterations);

  clock_t end = clock();
  printf("Inverse matrix computation : %f sec\n", (double)(end - begin) / CLOCKS_PER_SEC);
  f32* tmp = create_matrix(size);
  mul_matrix(matrix, inv_matrix, tmp, size);
  print_matrix(tmp, size);

  free(matrix);
  free(inv_matrix);
  free(tmp);

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

static void print_matrix(const f32* matrix, size_t size)
{
  size_t aligned_size = size + (64 - (size & 63));

  if (size <= 16)
  {
    for (size_t i = 0; i != size; ++i)
    {
      for (size_t j = 0; j != size; ++j)
        printf("%4.1f ", matrix[i * aligned_size + j]);
      putchar('\n');
    }
  }
  else
  {
    for (size_t i = 0; i != 4; ++i)
    {
      for (size_t j = 0; j != 8; ++j)
        printf("%4.1f ", matrix[i * aligned_size + j]);
      putchar('\n');
    }
    for (size_t i = size - 4; i != size; ++i)
    {
      for (size_t j = size - 8; j != size; ++j)
        printf("%4.1f ", matrix[i * aligned_size + j]);
      putchar('\n');
    }
  }
}
