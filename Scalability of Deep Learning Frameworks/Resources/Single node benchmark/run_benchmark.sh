#!/bin/bash

#SBATCH --job-name=daint_benchmark
#SBATCH --time=03:30:00
#SBATCH --nodes=1
#SBATCH --constraint=gpu
#SBATCH --output=benchmark_daint.%j.log
#SBATCH --error=benchmark_daint.%j.err

# load modules
module load daint-gpu
module load TensorFlow/1.7.0-CrayGNU-17.08-cuda-8.0-python3
module load shifter-ng

# benchmark tensorflow
srun -N 1 -n 1 -C gpu -t \
sh run_tensorflow.sh

# benchmark caffe2
srun -N 1 -n 1 -C gpu -t \
shifter run load/library/caffe2_container \
sh run_caffe2.sh

# benchmark pytorch
srun -N 1 -n 1 -C gpu \
shifter run load/library/horovod_container \
sh run_pytorch.sh
