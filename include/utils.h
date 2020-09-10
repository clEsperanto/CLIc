/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __utils_h
#define __utils_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <string>

/**
 * save string into text file
 */ 
void write_kernel_source(std::string);

/**
 * OpenCL error code into string function
 */ 
const char *getOpenCLErrorString(cl_int);

#endif // __utils_h