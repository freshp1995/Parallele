
void printMatrix(size_t rows, size_t columns, __global float *matrix) {

  for (size_t i = 0; i < rows; ++i) {

    for (size_t j = 0; j < columns; ++j) {
      printf("[%f]\t", matrix[i * columns + j]);
    }
    printf("\n");
  }
}

__kernel void matrix(const int l, const int m, const int n,
                     const __global float *A,
                     const __global float *B,
                     __global float *C) {

  const int globalRow = get_global_id(0);
  const int globalCol = get_global_id(1);
  float acc = 0.0f;

  for (int k = 0; k < m; k++) {
    acc += A[globalRow * m + k] * B[k * n + globalCol];
  }

  C[globalRow * n + globalCol] = acc;
}
