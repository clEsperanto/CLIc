# OpenCL installation

clEsperanto is relying on [OpenCL](https://www.khronos.org/opencl/) for computation. This means that if you do not have a valid OpenCL installation, it will not run (nor compile).

OpenCL is not hardware specific, it is an open standard for parallel programming. Hence, it can run on GPUs and CPUs. However, in order to access a specific hardware to run on, a hardware corresponding Installable Client Driver (ICD) must be provided. Those ICD are usually proprietary implementation. 
They come as runtime library with various naming, for example `amdocl.so/dll`, `intelOpenCL.so/dll`, `nvidia-libopencl.so/dll`, and are normally provided by the hardware vendor along with their drivers.

To avoid building clEsperanto against a specific vendor ICD, we are building it against an *ICD-loader*. It is a generic ICD interface which will then load the correct ICD corresponding to the hardware you are trying to access. These comes has more generic named libraries installed in your system, for example `libOpenCL.so`, `OpenCL.lib`, or `OpenCL.dll`.

## Installation

For now, we would suggest to install your hardware specific drivers and Software Development Kit (SDK). The most common one are Intel, AMD, and Nvidia. 

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
sudo apt install opencl-headers ocl-icd-dev
```
2. Then, 
   - **Nvidia**:  install drivers through `software-properties-gtk`, in the "Additional Drivers" menu. Select the most recent and tested version.
   - **AMD**: install your card drivers available on the [AMD website](https://www.amd.com/en/support).
   - **Intel**: run the command `sudo apt install beignet beignet-dev` in a terminal.

### MacOS

<center><b>WIP</b</center>
