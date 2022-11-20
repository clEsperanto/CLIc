# OpenCL installation

clEsperanto is relying on [OpenCL](https://www.khronos.org/opencl/) for computation. This means that if you do not have OpenCL available on your system, it will not run (nor compile).

OpenCL is not hardware specific, it is an open standard for parallel programming. Hence, it can run on GPUs and CPUs. However, in order to access a specific hardware to run on, you need to provide its corresponding Installable Client Driver (ICD). Those ICD are usually proprietary implementation and they are provided by vendors with their drivers package. 

To avoid building OpenCL project against a specific vendor ICD, we are building it against an *ICD-loader*. It is a generic ICD interface library. It does not contain any OpenCL implementation like ICDs, but work as a dispatcher and redirect OpenCL operation towards selected ICDs associated to the hardware you are trying run on. These comes with more generic names, for example `libOpenCL.so` or `OpenCL.dll`, respectively for Linux and Windows.

## Installation

You first need to install your hardware specific drivers or Software Development Kit (SDK). The most common vendors are Intel, AMD, and Nvidia. 

<center>
<table>
<thead>
  <tr>
    <th>Nvidia</th>
    <th>Intel</th>
    <th>AMD</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>
<a href="https://developer.nvidia.com/cuda-downloads">Nvidia SDK</a></td>
    <td><a href="https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk/choose-download.html">Intel SDK</a></td>
    <td><a href="https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases">AMD SDK</a></td>
  </tr>
</tbody>
</table>
</center>


### Windows

All SDK are available for windows and can be found on the vendors website. Once installed, make sure to have their folder added to your system **PATH**.

**Warning:** AMD discontinued their SDK, though an open-source version still available. AMD user can still have a work around by installing the Intel SDK and their AMD drivers.

### Linux

IT is possible to directly use Nvidia or Intel SDK which are also compatible with linux system.

Alternatively, minimal installation can be done as followed:
1. First by running the following commands in a terminal
```
sudo apt update
sudo apt install opencl-headers ocl-icd-dev ocl-icd-opencl-dev
```
2. Then, 
   - **Nvidia**:  install drivers through `software-properties-gtk`, in the "Additional Drivers" menu. Select the most recent and tested version.
   - **AMD**: install your card drivers available on the [AMD website](https://www.amd.com/en/support).
   - **Intel**: run the command `sudo apt install beignet beignet-dev` in a terminal.

### MacOS

Contrary to Windows and Linux, MacOS is bit simpler. Indeed, Apple computer cannot hold any type of devices and their configuration depend on the year the computer was made. If your system is from 2016 or earlier, you should be equipped with Intel and Nvidia devices. From 2016 to 2021, with Intel and AMD. And from 2021 and later with new processor M1 produce by Apple.

This is not so much an issue as, in any cases you are, OpenCL is already provided by MacOS system. Not particular need is then required for our project to run.

**WARNING:** Latest M1 chips have not yet been fully tested, though no particular issue is to be expected but
