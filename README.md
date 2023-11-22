# CARnary Library

This library contains headers common to the client and the daemon, as well as the client implementation.

The client is made to be integrated into every critical package of the pipeline.

# Requisites

- C++17 compiler
- CMake

# Installation

- Create a new `build` directory and navigate to it.
- Run the command `cmake ..`.
- Run the command `make -j8` to build the library.
- Run the command `sudo make install` to install system-wise.
