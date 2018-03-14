FROM ubuntu:16.04
MAINTAINER Marc <spehlmann@wisc.edu>

ENV LLVM_VERSION=3.8
ENV CONTAINER_USER="cxxdev"

# num_cores is an argument that can be passed in during building via --build-arg
ARG num_cores=40
# build_type is an argument that can be passed in during building via --build-arg
ARG build_type=Debug

RUN apt-get clean && apt-get update && apt-get install -y locales
RUN locale-gen en_GB.UTF-8
ENV LANG=en_GB.UTF-8 \
    LANGUAGE=en_GB:en \
    LC_ALL=en_GB.UTF-8

# Install necessary dependencies
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install \
  autoconf \
  bison \
  build-essential \
  clang++-${LLVM_VERSION} \
  clang-${LLVM_VERSION} \
  cmake \
  curl \
  flex \
  git-core \
  libtool \
  software-properties-common \
  wget

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100

# Add non-root user for container.
# Password is the same as the username
RUN useradd -m ${CONTAINER_USER} && \
    echo ${CONTAINER_USER}:${CONTAINER_USER} | chpasswd

# Make bash the default shell (useful for when using tmux in the container)
RUN chsh --shell /bin/bash ${CONTAINER_USER}

# Compile and install GRPC/Protobuf3
RUN cd /home/${CONTAINER_USER} && \
  git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc && \
  cd grpc && git submodule update --init && \
  make -j$num_cores && make install && \
  cd third_party/protobuf && make install
RUN rm -r /home/${CONTAINER_USER}/grpc

# Compile and install quickstep distributed
USER ${CONTAINER_USER}
RUN cd /home/${CONTAINER_USER} && \
  git clone https://github.com/apache/incubator-quickstep quickstep && \
  cd quickstep && \
  git submodule update --init && \
  cd third_party && ./download_and_patch_prerequisites.sh && cd .. && \
  cd build && cmake -DENABLE_DISTRIBUTED=ON -DCMAKE_BUILD_TYPE=$build_type -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .. && \
  make quickstep_distributed_cli_shell tmb_net_server -j$num_cores
