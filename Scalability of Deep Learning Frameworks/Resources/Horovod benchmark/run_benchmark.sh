#!/bin/bash

# load modules
module load daint-gpu
module load shifter-ng

# benchmark Tensorflow using  Horovod
NUM_NODE_LIST="1 2 4 8 16 32 64 128 256 512"

# batch size 64 upto 512 nodes
for NUM_NODE in ${NUM_NODE_LIST}
do
	echo Benchmarking for ${NUM_NODE} nodes
	srun --switch 1 -N ${NUM_NODE} -n ${NUM_NODE} -t 00:20:00 -C gpu \
	shifter run --mpi load/library/horovod_container \
	python scripts/tf_cnn_benchmarks/tf_cnn_benchmarks.py \
	--model inception3 \
	--batch_size 64 \
	--variable_update horovod >> horovod_result_nodes_${NUM_NODE}.txt &
        sleep 2
done

# batch size 32 at 256 nodes
#echo Benchmarking for 256 nodes
#srun --switch 1 -N 256 -n 256 -t 00:07:00  -C gpu \
#shifter run --mpi load/library/horovod_container \
#python scripts/tf_cnn_benchmarks/tf_cnn_benchmarks.py \
#--model inception3 \
#--batch_size 64 \
#--variable_update horovod >> horovod_result_nodes_256_batchsize_64.txt &
#sleep 2
#
## batch size 16 at 512 nodes
#echo Benchmarking for 512 nodes
#srun --switch 1 -N 512 -n 512 -t 00:07:00  -C gpu \
#shifter run --mpi load/library/horovod_container \
#python scripts/tf_cnn_benchmarks/tf_cnn_benchmarks.py \
#--model inception3 \
#--batch_size 64 \
#--variable_update horovod >> horovod_result_nodes_512_batchsize_64.txt &
#sleep 2
#
#echo Benchmarking for 256 nodes
#srun --switch 1 -N 256 -n 256 -t 00:05:00  -C gpu \
#shifter run --mpi load/library/horovod_container \
#python scripts/tf_cnn_benchmarks/tf_cnn_benchmarks.py \
#--model inception3 \
#--batch_size 128 \
#--variable_update horovod >> horovod_result_nodes_256_batchsize_128.txt &
#sleep 2

