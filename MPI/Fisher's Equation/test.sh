for p in 1 2 4
do
    echo =${p} MPI ranks
    for dim in 128 256 512 1024
    do
        echo ======= ${dim}x${dim}
	for t in {1..10}
        do
            printf "%3d threads : "  $t
            OMP_NUM_THREADS=$t mpirun -np $p ./main $dim $dim 100 0.01 | grep took;
        done
    done
done
