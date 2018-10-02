#!/bin/bash

export EASYBUILD_PREFIX=$SCRATCH/easybuild/daint
export EB_CUSTOM_REPOSITORY=$SCRATCH/production/easybuild
module load daint-gpu EasyBuild-custom
module load cray-python/3.6.1.1
module load Caffe2-PyExtensions/0.8.1-CrayGNU-17.08-python3
module load cray-hdf5/1.10.0
module load cuDNN/.7.0-cuda-8.0
module load glog/0.3.4-CrayGNU-17.08
module load gflags/2.1.2-CrayGNU-17.08
module load OpenCV/3.2.0-CrayGNU-17.08-cuda-8.0-python3
module load LMDB/0.9.18-CrayGNU-17.08
module load LevelDB/1.18-CrayGNU-17.08
module load snappy/1.1.3-CrayGNU-17.08
module load Boost/1.65.0-CrayGNU-17.08-python3
module load scikit-image/.0.13.0-CrayGNU-17.08-python3
module load protobuf-core/.3.5.1-CrayGNU-17.08
module load protobuf-python/3.5.1-CrayGNU-17.08-python3
module load caffe
export PYTHONPATH=$PYTHONPATH:$HOME/local/lib/python3.6/site-packages
