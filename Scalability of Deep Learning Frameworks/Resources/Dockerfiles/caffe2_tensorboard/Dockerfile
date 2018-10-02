FROM nvidia/cuda:8.0-cudnn7-devel-ubuntu16.04
LABEL maintainer="aaronmarkham@fb.com"

# caffe2 install with gpu support

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    libgflags-dev \
    libgoogle-glog-dev \
    libgtest-dev \
    libiomp-dev \
    libleveldb-dev \
    liblmdb-dev \
    libopencv-dev \
    libopenmpi-dev \
    libprotobuf-dev \
    libsnappy-dev \
    openmpi-bin \
    openmpi-doc \
    python-dev \
    python-numpy \
    python-pip \
    python-pydot \
    python-setuptools \
    python-scipy \
    wget \
    && rm -rf /var/lib/apt/lists/*

RUN pip install --no-cache-dir --upgrade pip==9.0.3 setuptools wheel && \
    pip install --no-cache-dir \
    flask \
    future \
    graphviz \
    hypothesis \
    jupyter \
    matplotlib \
    numpy \
    Pillow \
    protobuf \
    pydot \
    python-nvd3 \
    pyyaml \
    requests \
    scikit-image \
    scipy \
    setuptools \
    six \
    tornado \
    typing

########## INSTALLATION STEPS ###################
RUN git clone --branch master --recursive https://github.com/pytorch/pytorch.git \
    && cd pytorch && git checkout b62b378 \
    && git submodule update --recursive
RUN cd pytorch && mkdir build && cd build \
    && cmake .. \
    -DCUDA_ARCH_NAME=Manual \
    -DCUDA_ARCH_BIN="35 52 60 61" \
    -DCUDA_ARCH_PTX="61" \
    -DUSE_NNPACK=OFF \
    -DUSE_ROCKSDB=OFF \
    && make -j"$(nproc)" install \
    && ldconfig \
    && make clean \
    && cd .. \
    && rm -rf build

##### INSTALLATION FOR C2BOARD ##########
RUN git clone --recursive https://github.com/endernewton/c2board.git
RUN cd c2board && sed -i "s/develop --user/develop/g" Makefile && make

ENV PYTHONPATH /usr/local
