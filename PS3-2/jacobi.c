
//#define DEBUG

void printMatrix(size_t rows, size_t columns, __global float *matrix) {

    for (size_t i = 0; i < rows; ++i) {

        for (size_t j = 0; j < columns; ++j) {
            printf("[%f]\t", matrix[i * columns + j]);
        }
        printf("\n");
    }
}


/**
 * solves Ax = b approximately
 * @param A Matrix A
 * @param b Column b
 * @param x1 start column for x
 * @param x2 goal column for x
 * @param matrix_size size of A
 */
__kernel void jacobi(const __global float *f,
                     __global float *u1,
                     __global float *u2,
                     const float factor,
                     const unsigned int matrix_size
) {
    int i = get_global_id(0);
    int j = get_global_id(1);

        if (i == 0 || j == 0 || i == matrix_size - 1 || j == matrix_size - 1) {
            //
        } else {
            u2[i * matrix_size + j] = (float) 1 / 4 *
                                        (u1[matrix_size * (i - 1) + j]
                                         + u1[matrix_size * i + j + 1]
                                         + u1[matrix_size * i + j - 1]
                                         + u1[matrix_size * (i + 1) + j]
                                         - factor * f[i * matrix_size + j]);
        }
}

