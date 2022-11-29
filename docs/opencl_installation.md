# OpenCL installation

clEsperanto is relying on [OpenCL](https://www.khronos.org/opencl/) for computation. This means that if you do not have OpenCL available on your system, it will not run (nor compile).

OpenCL is not hardware specific, it is an open standard for parallel programming. Hence, it can run on GPUs and CPUs. However, in order to access a specific hardware to run on, you need to provide its corresponding Installable Client Driver (ICD). Those ICD are usually proprietary implementation and they are provided by vendors with their drivers package. 

To avoid building OpenCL project against a specific vendor ICD, we are building it against an *ICD-loader*. It is a generic ICD interface library. It does not contain any OpenCL implementation like ICDs, but work as a dispatcher and redirect OpenCL operation towards selected ICDs associated to the hardware you are trying run on. These comes with more generic names, for example `libOpenCL.so` or `OpenCL.dll`, respectively for Linux and Windows.

For compilation, CLIc will requires an ICD-loader installed on your system. For running, CLIc will required both ICD-loader and an ICD. If no ICD are foundable by the ICD-loader, CLIc will return an error at runtime with the following message:
```
"CL_DEVICE_NOT_FOUND, No OpenCL devices that matched given device type were found"
```
Or
```
"CL_DEVICE_NOT_AVAILABLE, No OpenCL compatible device was found"
```

## Installation

You first need to install your hardware specific drivers or Software Development Kit (SDK). The most common vendors are Intel, AMD, and Nvidia. 
SDK are heavier but will contain all that is required for development, more classic drivers should suffice for running the library.

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


### __Windows__

All SDK are available for windows and can be found on the vendors website. Once installed, make sure to have their folder [added to your system **PATH**](https://learn.microsoft.com/en-us/previous-versions/office/developer/sharepoint-2010/ee537574(v=office.14)).

**Warning:** AMD discontinued their SDK, though an open-source version still available. AMD user can still have a work around by installing the Intel SDK and their AMD drivers.

### __Linux__

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
   - **Intel**: run the command `sudo apt install beignet-dev` in a terminal.

### __MacOS__

Contrary to Windows and Linux, MacOS is bit simpler. Indeed, Apple computer cannot hold any type of devices and their configuration depend on the year the computer was made. If your system is from 2016 or earlier, you should be equipped with Intel and Nvidia devices. From 2016 to 2021, with Intel and AMD devices. And from 2021 and later with new processor (M1, M2, *etc.*) produce by Apple.

This is not so much an issue as in all cases OpenCL is already provided by MacOS system. No particular installation is needed for both compiling and running the library.
