# Build from source - MacOS

<center><b>WARNING:</b> new Mac M1 chips have not been properly tested yet.</center>

## Requirements

1. [XCode](https://developer.apple.com/xcode/).
2. [CMake](https://cmake.org/download/), e.g. version 3.20 or higher.
3. [OpenCL](../opencl_installation.md).

## Source compilation

In a terminal, install XCode command-line tools by running the following command
```
xcode-select --install
```

Download, install [CMake](https://cmake.org/download/) and make it usable by command line by adding it to your system **PATH**. To do so, run CMake and follow the “How to Install For Command Line Use” menu item for instructions.

In the source directory, you can then use CMake to configure and build the sources from a terminal by running the following commands.
```
cmake -S . -B ./build
cmake --build ./build
```

##### CMake GUI (Alternative) 

Alternatively you can use CMake GUI for a more user-friendly project configuration.

Start CMake and configure the **source directory (1)**. Create a "build" directory as sub-folder and select it as **build directory (2)**:
![img.png](configure_cmake.png)

In the CMAKE_BUILD_TYPE line, wither "Release" to configure a release build type.

Then run the configuration by clicking on **Configure (3)**, and choose "Unix Makefile":
![img.png](configure_compiler.png)

Click on **Generate (4)** and afterwards open the build directory and run the command `make` in a terminal to compile the whole project.

You can also choose XCode when running the configuration though it is much more heavier approach.

## Installation

**WIP**