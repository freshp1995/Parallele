//
// Created by Roland Gritzer on 13.10.16.
//

#include <iostream>
#include <malloc.h>
#include "ClLoader.h"

//#define PRINT_MTX


void printMatrix(int rows, int columns, cl_float *matrix) {

    for (int i = 0; i < rows; ++i) {
        std::string line = "";

        for (int j = 0; j < columns; ++j) {
            line.append("[");
            line.append(std::to_string(matrix[i * columns + j]));
            line.append("]\t");
        }
        std::cout << line << "\n";
    }
}

int main() {

    try {

        ClLoader *loader = new ClLoader("../matrix.c", 0);

        loader->Build();

        cl_int l, m, n;


        for (int d = 1024; d < 1024 * 8; d *= 2) {

            cl_int matrix_size = d;

            l = matrix_size;
            m = matrix_size;
            n = matrix_size;

            loader->AddParameter(&l, 0, sizeof(cl_int));
            loader->AddParameter(&m, 1, sizeof(cl_int));
            loader->AddParameter(&n, 2, sizeof(cl_int));

            std::cout << std::endl << "Matrix size: " << (int) matrix_size << "x" << (int) matrix_size << std::endl;

            cl_float *A = (cl_float *) std::malloc(l * m * sizeof(cl_float));
            cl_float *B = (cl_float *) std::malloc(m * n * sizeof(cl_float));
            cl_float *C = (cl_float *) std::malloc(l * n * sizeof(cl_float));


            for (int i = 0; i < l * m; i++) { A[i] = (cl_float) (3.6 * i + i * i + 3.1); }
            for (int i = 0; i < m * n; i++) { B[i] = (cl_float) (1.2 * i + 0.01 * i * i + 13.9); }
            for (int i = 0; i < l * n; i++) { C[i] = 0.0; }

            for (int i = 0; i < l; ++i) {
                for (int j = 0; j < n; ++j) {
                    C[i * n + j] = 0;
                }
            }

#ifdef PRINT_MTX
            printMatrix(l, m, A);
            std::cout << "\n*\n\n";
            printMatrix(m, n, B);
#endif

            cl_mem buffer_a = loader->AddBuffer(CL_MEM_READ_ONLY, 3, l * m * sizeof(cl_float));
            cl_mem buffer_b = loader->AddBuffer(CL_MEM_READ_ONLY, 4, m * n * sizeof(cl_float));
            cl_mem buffer_c = loader->AddBuffer(CL_MEM_READ_WRITE, 5, l * n * sizeof(cl_float));

            loader->WriteBuffer(buffer_a, A, 3, l * m * sizeof(cl_float));
            loader->WriteBuffer(buffer_b, B, 4, m * n * sizeof(cl_float));
            loader->WriteBuffer(buffer_c, C, 5, l * n * sizeof(cl_float));

            const size_t global[2] = {(size_t) l, (size_t) n};

            loader->Run(NULL, global);

            loader->ReadBuffer(buffer_c, l * n * sizeof(cl_float), C);

#ifdef PRINT_MTX
            std::cout << "\n=\n\n";
            printMatrix(l, n, C);
#endif

            loader->PrintProfileInfo();

//      for (int i = 0; i < l; ++i) {
//        for (int j = 0; j < n; ++j) {
//          if (C[i * n + j] != A[i * n + j])
//            correct = false;
//        }
//      }

//      std::cout << "The calculation was " << correct ? "correct" : "incorrect";

            free(A);
            free(B);
            free(C);

        }

        delete loader;

        return 0;

    } catch (const std::exception &e) {

        std::cout << std::flush;
        std::cerr << std::flush << "Exception thrown: " << e.what();

        return -1;
    }
}

