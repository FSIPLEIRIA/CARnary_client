FROM ubuntu:22.04

# update the system
RUN apt update
RUN apt upgrade -y

# install dependencies
RUN apt install build-essential wget libssl-dev -y

# build cmake from source
WORKDIR /temp
RUN wget https://github.com/Kitware/CMake/releases/download/v3.27.8/cmake-3.27.8.tar.gz
RUN tar -xvf cmake-3.27.8.tar.gz
WORKDIR /temp/cmake-3.27.8
RUN ./bootstrap
RUN make -j8
RUN make install
WORKDIR /home/fslart
RUN rm -rf /temp

# copy the source code
COPY . /home/fslart/carnary_lib
WORKDIR /home/fslart/carnary_lib

# build the library
RUN mkdir build
WORKDIR /home/fslart/carnary_lib/build
RUN cmake ..
RUN make -j8

# install
RUN make install
