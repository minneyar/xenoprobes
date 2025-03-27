# Build Instructions

This is a C++ project built using [CMake](https://cmake.org/).  Building it should just be a matter of installing
dependencies (Qt 6, spdlog, Boost) and then using CMake to compile it.

Please add more detailed instructions here if you have success building it on your platform!

## Linux

1. Install dependencies.
   - Ubuntu:
    ```
    sudo apt install \
        build-essential \
        cmake \
        git \
        libboost-all-dev \
        libspdlog-dev \
        ninja-build \
        qt6-base-dev \
        qt6-svg-dev
    ```
   - Fedora:
    ```
    sudo dnf install \
        boost-devel \
        boost-static \
        cmake \
        gcc-c++ \
        ninja-build \
        qt6-qtbase-devel \
        qt6-qtsvg-devel \
        spdlog-devel
    ```
   
2. Create a `build` directory inside the repository and run cmake in it:
   ```
   cd xenoprobes/
   cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja
   ```
3. Build it:
   ```
   cmake --build build/
   ```
4. After it's finished building, the command line tool will be in `build/src/xenoprobes/` and the GUI
   will be in `build/src/xenoprobes_gui/`.