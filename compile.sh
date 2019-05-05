gcc -Wall -o3 lbp_omp.c util.h util.c -lm -fopenmp -o lbp_omp
gcc -Wall -o3 lbp_seq.c util.h util.c -lm -o lbp_seq