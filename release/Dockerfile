FROM ubuntu:16.04
MAINTAINER Marc <spehlmann@wisc.edu>

ENV LLVM_VERSION=3.8
ENV CONTAINER_USER="cxxdev"

RUN locale-gen en_GB.UTF-8
ENV LANG=en_GB.UTF-8 \
    LANGUAGE=en_GB:en \
    LC_ALL=en_GB.UTF-8

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get -y upgrade && apt-get -y install sudo wget apt-utils
RUN wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -
RUN echo "deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-${LLVM_VERSION} main" >> /etc/apt/sources.list.d/llvm.list
RUN apt-get update && apt-get -y upgrade

RUN apt-get -y install \
  aptitude \
  bash-completion \
  bison \
  build-essential \
  clang++-${LLVM_VERSION} \
  clang-${LLVM_VERSION} \
  cmake \
  coreutils \
  curl \
  flex \
  g++ \
  gcc \
  gdb \
  git-core \
  htop \
  iwyu \
  libprotobuf-dev \
  lldb-${LLVM_VERSION} \
  ninja-build \
  protobuf-compiler \
  python \
  python-dev \
  python-pip \
  python-software-properties \
  software-properties-common \
  subversion \
  tmux \
  tree \
  unzip \
  vim

# creates gpg directories
RUN gpg --version

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100
RUN update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-${LLVM_VERSION} 100

# Add non-root user for container but give it sudo access.
# Password is the same as the username
RUN useradd -m ${CONTAINER_USER} && \
    echo ${CONTAINER_USER}:${CONTAINER_USER} | chpasswd && \
    cp /etc/sudoers /etc/sudoers.bak && \
    echo "${CONTAINER_USER}  ALL=(root) ALL" >> /etc/sudoers
# Make bash the default shell (useful for when using tmux in the container)
RUN chsh --shell /bin/bash ${CONTAINER_USER}
USER ${CONTAINER_USER}
