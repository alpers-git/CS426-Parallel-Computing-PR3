for k in 1 2 5 7 10
do
    ./lbp_seq $k >> alper_şahıstan.output
    #echo "FOR k=$k" >> prof_sequential.txt
    #gprof lbp_seq gmon.out >> prof_sequential.txt
    for num_of_threads in 1 2 4 6 8 16
    do
        OMP_NUM_THREADS=$num_of_threads ./lbp_omp $k >> alper_şahıstan.output
        #echo "FOR k=$k and NUM OF THREADS=$num_of_threads" >> prof_omp.txt
        #gprof lbp_omp gmon.out >> prof_omp.txt
    done
done