build Windows
=============

Requirements
------------

#. A C/C++ compiler, [Visual Studio](https://visualstudio.microsoft.com/), e.g. Community edition.
#. [CMake](https://cmake.org/download/), e.g. version 3.20 or higher.
#. [OpenCL](../opencl_installation.md).

Setting up the environment
--------------------------

First, install the C/C++ compiler. On Windows, this can be done with the following steps:

Download [Visual Studio, e.g. Community edition](https://visualstudio.microsoft.com/)

![img.png](visual_studio_download.png)

While installing: Activate Desktop Development C++ build tools

![img.png](visual_studio_installation.png)

Then we need to install CMake. Download the latest version from [here](https://cmake.org/download/) and run the installer.

.. warning::
    
    Make sure to add CMake to the PATH variable during installation.

Source compilation
------------------

Start CMake and configure the **source directory (1)**. Create a "build" directory as sub-folder and select it as **build directory (2)**:

![img.png](configure_cmake.png)

When clicking on **Configure (3)**, choose the Visual Studio version you installed and **select "x64" as target platform**:
![img.png](configure_compiler.png)

Click on **Generate (4)** and afterwards on Open Project. Visual studio should open. Select "Release" in this pulldown:
![img.png](release_build.png)

Use the menu Build > Build solution to compile the whole project.