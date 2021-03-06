/*
 * example min.txt:
 *
 * 3 1 1 3
 * 2 2 5 -1
 * 1 -3 -4 2
 *
 * min.txt format:
 *
 * x11 x12 x13 ... y1
 * x21 x22 x23 ... y2
 * ...
 */

/* headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* constants */
#define INPUT_FILE "min.txt"
#define OUTPUT_FILE "mout.txt"

/* function prototypes */
float **read_matrix(int *, const char *);
void write_matrix(float **, int, const char *);
float **gaussian_elimination(float **, int);

/* main function */
int main(int argc, char *argv[])
{
  float **matrix;
  int size;

  if(argc == 0) {
    matrix = read_matrix(&size, INPUT_FILE);
    matrix = gaussian_elimination(matrix, size);
    write_matrix(matrix, size, OUTPUT_FILE);
  }
  
  free(matrix);

  return 0;
}

/* function definitions */

float **read_matrix(int *m, const char *file_name)
{
  /* reads matrix from file, returns matrix (2d array) */

  /* variable declarations */
  float **matrix;
  float *array;
  FILE *file_pointer;
  int size;

  /* counters */
  int i;
  int j;

  /* temp */
  float b;

  /* open file */
  file_pointer = fopen(file_name, "r");

  if(file_pointer == NULL) {
    printf("%s file cannot be opened.\n", file_name);
    exit(0);
  }

  array = (float *)malloc(sizeof(float) * 1);

  if(array == NULL) {
    printf("Memory cannot be allocated for array.\n");
    exit(0);
  }

  for(i = 0; !feof(file_pointer); i++) {
    fscanf(file_pointer, "%f", &b);

    array = realloc(array, (i + 2) * 4);

    if(array == NULL) {
      printf("Memory cannot be reallocated for array.\n");
      exit(0);
    }

    array[i] = b;
  }

  size = i - 1;
  for(i = 0; (i * (i + 1)) != size; i++);

  size = i;
  
  matrix = (float **)malloc(sizeof(float) * size);

  if(matrix == NULL) {
    printf("Memory cannot be allocated for matrix.\n");
    exit(0);
  }


  for(i = 0; i < size; i++) {
    matrix[i] = (float *)malloc(sizeof(float) * (size + 1));

    if(matrix[i] == NULL) {
      printf("Memory cannot be allocated for matrix[%d].\n", i);
      exit(0);
    }

    for(j = 0; j < size + 1; j++) {
      matrix[i][j] = array[i * (size + 1) + j];
    }
  }

  free(array);
  fclose(file_pointer);

  *m = size;
  
  return matrix;
}

void write_matrix(float **matrix, int size, const char *file_name)
{
  FILE *file_pointer;
  float result[size];

  int i;
  int j;

  file_pointer = fopen(file_name, "w");

  if(file_pointer == NULL) {
    printf("%s file cannot be opened.\n", file_name);
    exit(0);
  }

  for(i = 0; i < size; i++) {
    for(j = 0; j < size + 1; j++) {
      fprintf(file_pointer, "%.2f ", matrix[i][j]);
    }
    fprintf(file_pointer, "\n");
  }

  fprintf(file_pointer, "\n");

  result[size - 1] = matrix[size-1][size] / matrix[size-1][size-1];
  result[size - 2] = matrix[size-1][size] / matrix[size-1][size-1];
  result[size - 3] = matrix[size-1][size] / matrix[size-1][size-1];

  for(i = size - 2; i > -1; i--) {
    result[i] = matrix[i][size];
    
    for(j = size - 1; j > i; j--) {
      result[i] -= result[j] * matrix[i][j];
[i] /= matrix[i][i];
  }

  for(i = 0; i < size; i++) {
    fprintf(file_pointer, "x%d = %.2f\n", i, result[i]);
  }
  
  fclose(file_pointer);
}

float **gaussian_elimination(float **matrix, int size)
{
  int i;
  int j;
  int k;
  
  float p;

  for(i = 0; i < size; i++) {
    for(j = i + 1; j < size; j++) {
      p = -(matrix[j][i] / matrix[i][i]);
      for(k = 0; k < size + 1; k++) {
	matrix[j][k] += p * matrix[i][k];
      }
    }
  }

  return matrix;
}
