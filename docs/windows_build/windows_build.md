# Build from source - Windows

## Requirements

1. [Visual Studio](https://visualstudio.microsoft.com/), e.g. Community edition.
2. [CMake](https://cmake.org/download/), e.g. version 3.20 or higher.
3. [OpenCL](../opencl_installation.md).

## Source compilation

Download [Visual Studio, e.g. Community edition](https://visualstudio.microsoft.com/)

![img.png](visual_studio_download.png)

While installing: Activate Desktop Development C++ build tools

![img.png](visual_studio_installation.png)

Download and install [CMake, e.g. version 3.20](https://cmake.org/download/)

Start CMake and configure the **source directory (1)**. Create a "build" directory as sub-folder and select it as **build directory (2)**:

![img.png](configure_cmake.png)

When clicking on **Configure (3)**, choose the Visual Studio version you installed and **select "x64" as target platform**:
![img.png](configure_compiler.png)

Click on **Generate (4)** and afterwards on Open Project. Visual studio should open. Select "Release" in this pulldown:
![img.png](release_build.png)

Use the menu Build > Build solution to compile the whole project.

## Installation

**WIP**