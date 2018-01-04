for dim in 64 128 256 512 1024
do
    echo ======= ${dim}x${dim}
    for t in {1..20}
    do
        printf "%3d threads : "  $t
        OMP_NUM_THREADS=$t ./main $dim $dim 100 0.01 | grep took;
    done
done
