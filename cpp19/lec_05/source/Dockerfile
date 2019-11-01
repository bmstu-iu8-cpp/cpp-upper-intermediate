FROM ubuntu:18.04
LABEL maintainer="rusdevops@gmail.com"

RUN apt update && apt install -y software-properties-common && \
    apt-add-repository -y "ppa:ubuntu-toolchain-r/test" && apt update && \
    apt install -y wget && wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main" && \
    apt install -y libcurl4-gnutls-dev curl nodejs npm \
    g++-7 lcov gcovr cmake cmake-data python3-pip clang-7 && \
    rm -rf /var/lib/apt/lists/* && \
    pip3 install cpplint && npm install -g jscpd && \
    apt -y autoremove && ln -s /usr/bin/clang-7 /usr/bin/clang && \
    ln -s /usr/bin/clang++-7 /usr/bin/clang++ && \
    ln -s /usr/bin/clang-cpp-7 /usr/bin/clang-cpp
