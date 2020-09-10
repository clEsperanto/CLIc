/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <utility>

#include "utils.h"
#include "tiffreader.h"
#include "tiffwriter.h"
#include "image.h"
#include "clbuffer.h"

#include "clgpu.h"


/**
 * Push local image into buffer
 * @return clBuffer.
 */
template<class T>
clBuffer push(Image<T>& img, std::string type, cl_context context, cl_command_queue command_queue)
{
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, img.GetDataSize(), img.GetData(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to create buffer in push() : " << getOpenCLErrorString(clError) << std::endl;
    }
    clError = clEnqueueWriteBuffer(command_queue, mem_obj, CL_TRUE, 0, img.GetDataSize(), img.GetData(), 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to write buffer in push() " << getOpenCLErrorString(clError) << std::endl;
    }
    return clBuffer (mem_obj, img.GetDimensions().data(), img.GetType());
}


/**
 * Create an empty buffer, use local image for initialisation.
 * @return clBuffer.
 */
template<class T>
clBuffer create(Image<T>& img, std::string type, cl_context context, cl_command_queue command_queue)
{
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, img.GetDataSize(), img.GetData(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to create buffer in create() : " << getOpenCLErrorString(clError) << std::endl;
    }
    return clBuffer (mem_obj, img.GetDimensions().data(), img.GetType());
}


/**
 * Create an empty buffer, use clBuffer for initialisation.
 * @return clBuffer.
 */
template<class T>
clBuffer create(clBuffer& gpu_obj, std::string type, cl_context context, cl_command_queue command_queue)
{
    size_t nbElement = sizeof(T) * gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, nbElement, nullptr, &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to create buffer in create() : " << getOpenCLErrorString(clError) << std::endl;
    }
    return clBuffer (mem_obj, gpu_obj.GetDimensions().data(), type);
}


/**
 * Create an empty buffer, use dimensions and type for initialisation.
 * @return clBuffer.
 */
template<class T>
clBuffer create(std::array<unsigned int,3> arr, std::string type, cl_context context, cl_command_queue command_queue)
{
    size_t nbElement = sizeof(T) * arr[0] * arr[1] * arr[2];
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, nbElement, nullptr, &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to create buffer in create() : " << getOpenCLErrorString(clError) << std::endl;
    }
    return clBuffer (mem_obj, arr.data(), type);
}



/**
 * Pull clBuffer into local image.
 * @return image
 */
template<class T>
Image<T> pull(clBuffer gpu_obj, cl_context context, cl_command_queue command_queue)
{
    unsigned int nbElements = gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
    size_t size = sizeof(T) * nbElements;
    T* array = new T[nbElements];
    cl_int clError = clEnqueueReadBuffer(command_queue, gpu_obj.GetPointer(), CL_TRUE, 0, size, array, 0, NULL, NULL);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to read buffer in pull() : " << getOpenCLErrorString(clError) << std::endl;
    }
    Image<T> img (array, gpu_obj.GetDimensions()[0], gpu_obj.GetDimensions()[1], gpu_obj.GetDimensions()[2], gpu_obj.GetType());
    return img;
}


/**
 * Get preamble source code
 */
std::string LoadPreamble()
{
    std::string preamble;
    const std::string preamble_path = CLP_PATH;
    std::string filename = preamble_path + "/preamble.cl";
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    if (file)
    {
        file.seekg(0, std::ios::end);
        preamble.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&preamble[0], preamble.size());
        file.close();
    }
    else
    {
        std::cerr << "Error reading file! Cannot open " << filename << std::endl;
    }
    return preamble;
}


/**
 * Get kernel source code
 */
std::string LoadSources(std::string kernelFilename)
{
    std::string sources;
    const std::string kernels_path = CLI_PATH;
    std::string filename = kernels_path + "/" + kernelFilename + "_x.cl";
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    if (file)
    {
        file.seekg(0, std::ios::end);
        sources.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&sources[0], sources.size());
        file.close();
    }
    else
    {
        std::cerr << "Error reading file! Cannot open " << filename << std::endl;
    }
    return sources;
}


/**
 * Set OpenCL Defines variable from clBuffer.
 */
std::string LoadDefines(std::map<std::string, clBuffer>& parameters)
{
    std::string defines;
    defines = defines + "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";
    defines = defines + "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT";
    defines = defines + "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";
    defines = defines + "\n";   

    for (auto itr = parameters.begin(); itr != parameters.end(); ++itr)
    {
        // image type handling
        defines = defines + "\n#define CONVERT_" + itr->first + "_PIXEL_TYPE clij_convert_" + itr->second.GetType() + "_sat";
        defines = defines + "\n#define IMAGE_" + itr->first + "_TYPE __global " + itr->second.GetType() + "*";
        defines = defines + "\n#define IMAGE_" + itr->first + "_PIXEL_TYPE " + itr->second.GetType();

        // image size handling
        if (itr->second.GetDimensions()[2] > 1)
        {
            defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_WIDTH " + std::to_string(itr->second.GetDimensions()[0]);
            defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_HEIGHT " + std::to_string(itr->second.GetDimensions()[1]);
            defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_DEPTH " + std::to_string(itr->second.GetDimensions()[2]);
        }
        else
        {
            if (itr->second.GetDimensions()[1] > 1)
            {
                defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_WIDTH " + std::to_string(itr->second.GetDimensions()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_HEIGHT " + std::to_string(itr->second.GetDimensions()[1]);
            }
            else
            {
                defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_WIDTH " + std::to_string(itr->second.GetDimensions()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_HEIGHT 1";
            }
            defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_DEPTH 1";
        }

        // position (dimensionality) handling
        if (itr->second.GetDimensions()[2] == 1)
        {
            defines = defines + "\n#define POS_" + itr->first + "_TYPE int2";
            if (itr->second.GetDimensions()[1] == 1) // 1D
            {
                defines = defines + "\n#define POS_" + itr->first + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, 0)";
            }
            else // 2D
            {
                defines = defines + "\n#define POS_" + itr->first + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, pos1)";
            }
        }
        else // 3/4D
        {
            defines = defines + "\n#define POS_" + itr->first + "_TYPE int4";
            defines =
                defines + "\n#define POS_" + itr->first + "_INSTANCE(pos0,pos1,pos2,pos3) (int4)(pos0, pos1, pos2, 0)";
        }

        // read/write images
        std::string sdim = (itr->second.GetDimensions()[2] == 1) ? "2" : "3";
        defines = defines + "\n#define READ_" + itr->first + "_IMAGE(a,b,c) read_buffer" + sdim + "d" + itr->second.GetTypeId() +
                  "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
        defines = defines + "\n#define WRITE_" + itr->first + "_IMAGE(a,b,c) write_buffer" + sdim + "d" + itr->second.GetTypeId() +
                  "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
        defines = defines + "\n";
    }
    return defines;
}


/**
 * Set maximum z projection kernel.
 *
 * Basic function that load and prepare the maximum_z_projection code.
 * Build the program, kernel, and setup the parameters before enqueuing the kernel.
 *
 */
int maximumzprojection(clBuffer src_gpu_obj, clBuffer dst_gpu_obj,
                       cl_context context, cl_device_id device_id, cl_command_queue command_queue)
{
    // initialise information on kernel and data to process
    cl_int clError;
    std::string kernel_name = "maximum_z_projection";

    std::map<std::string, clBuffer> dataList;
    std::pair<std::string, clBuffer> p1 = std::make_pair("src", src_gpu_obj);
    std::pair<std::string, clBuffer> p2 = std::make_pair("dst_max", dst_gpu_obj);
    dataList.insert(p1);
    dataList.insert(p2);

    // read kernel, defines, and preamble
    std::string kernel_src = LoadSources(kernel_name);
    std::string defines_src = LoadDefines(dataList);
    std::string preambule_src = LoadPreamble();

    // construct final source code
    std::string ocl_src = defines_src + "\n" + preambule_src + "\n" + kernel_src;
    const char *source_str = (ocl_src).c_str();
    size_t source_size = (ocl_src).size();

    // OpenCL kernel code debuging. Uncomment for kernel code information
    // write_kernel_source(ocl_src);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, &source_str, &source_size, &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OpenCL Error! Fail to create program in maximumzprojection() : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    // build the program
    clError = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OpenCL Error! Fail to build program in maximumzprojection() : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    // create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, kernel_name.c_str(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OpenCL Error! Fail to create kernel in maximumzprojection() : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }

    // Set the arguments of the kernel
    cl_mem src_mem = src_gpu_obj.GetPointer();
    cl_mem dst_mem = dst_gpu_obj.GetPointer();
    clError = clSetKernelArg(kernel, 0, sizeof(cl_mem), &dst_mem);
    clError = clSetKernelArg(kernel, 1, sizeof(cl_mem), &src_mem);

    // execute the opencl kernel
    size_t global_item_size[3];
    for (size_t i = 0; i < 3; i++)
    {
        global_item_size[i] = std::max(src_gpu_obj.GetDimensions()[i], dst_gpu_obj.GetDimensions()[i]);
    }
    size_t work_dim = 3;
    clError = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, nullptr, global_item_size, nullptr, 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OpenCL Error! Could not enqueue the ND-Range in maximumzprojection() : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/**
 * Set add image and scalar kernel.
 *
 * Basic function that load and prepare the add_image_and_scalar source code.
 * Build the program, kernel, and setup the parameters before enqueuing the kernel.
 *
 */
int addImageAndScalar3d(clBuffer src_gpu_obj, clBuffer dst_gpu_obj, float scalar,
                       cl_context context, cl_device_id device_id, cl_command_queue command_queue)
{
    // initialise information on kernel and data to process
    cl_int clError;
    std::string kernel_name = "add_image_and_scalar_3d";
    
    std::map<std::string, clBuffer> dataList;
    std::pair<std::string, clBuffer> p1 = std::make_pair("src", src_gpu_obj);
    std::pair<std::string, clBuffer> p2 = std::make_pair("dst", dst_gpu_obj);
    dataList.insert(p1);
    dataList.insert(p2);

    // read kernel, defines, and preamble
    std::string kernel_src = LoadSources(kernel_name);
    std::string defines_src = LoadDefines(dataList);
    std::string preambule_src = LoadPreamble();

    // construct final source code
    std::string ocl_src = defines_src + "\n" + preambule_src + "\n" + kernel_src;
    const char *source_str = (ocl_src).c_str();
    size_t source_size = (ocl_src).size();

    // OpenCL kernel code debuging. Uncomment for kernel code information
    // write_kernel_source(ocl_src);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, &source_str, &source_size, &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OpenCL Error! Fail to create program in addImageAndScalar3d() " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    // build the program
    clError = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OpenCL Error! Fail to build program in addImageAndScalar3d() : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    // create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, kernel_name.c_str(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OpenCL Error! Fail to create kernel in addImageAndScalar3d() : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }

    // Set the arguments of the kernel
    cl_mem src_mem = src_gpu_obj.GetPointer();
    cl_mem dst_mem = dst_gpu_obj.GetPointer();
    clError = clSetKernelArg(kernel, 0, sizeof(cl_mem), &src_mem);
    clError = clSetKernelArg(kernel, 1, sizeof(cl_mem), &dst_mem);
    clError = clSetKernelArg(kernel, 2, sizeof(float), (void *)&scalar);

    // execute the opencl kernel
    size_t global_item_size[3];
    for (size_t i = 0; i < 3; i++)
    {
        global_item_size[i] = std::max(src_gpu_obj.GetDimensions()[i], dst_gpu_obj.GetDimensions()[i]);
    }
    size_t work_dim = 3;
    clError = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, nullptr, global_item_size, nullptr, 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OpenCL Error! Could not enqueue ND-Range in addImageAndScalar3d() : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * Naive initialisation of device, context, and command queue.
 */ 
int initialise_gpu(cl_platform_id &platform_id, cl_device_id &device_id, cl_context &context, cl_command_queue &command_queue)
{
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int clError = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if (clError != CL_SUCCESS)  
    {
        std::cerr << "OpenCL Error! Could not get platform : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    clError = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "OpenCL Error! Could not get device : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &clError);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "OpenCL Error! Could not create context : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }

#if OCL_MAJOR_VERSION == 2  // 2.0 or higher
    command_queue = clCreateCommandQueueWithProperties(context, device_id, nullptr, &clError);
#elif OCL_MAJOR_VERSION == 1  // 1.1 and 1.2
    command_queue = clCreateCommandQueue(context, device_id, 0, &clError);
#else
    command_queue = clCreateCommandQueue(context, device_id, 0, &clError);
#endif

    if (clError != CL_SUCCESS) 
    {
        std::cerr << "OpenCL Error! Could not create commande queue : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/**
 * Main function
 *
 * Example of minipipeline operation.
 * Two kernel execution in a row, with push, create, and pull.
 *
 */
int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "CLIc Prototype, Compiled : " << __DATE__ << " at " << __TIME__ << std::endl;
        std::cerr << "Usage: " << argv[0] << std::endl;
        std::cerr << "\tstring - path to input tiff 3d image." << std::endl;
        std::cerr << "\tstring - path to output tiff image 1." << std::endl;
        std::cerr << "\tstring - path to output tiff image 2." << std::endl;
        return EXIT_FAILURE;
    }

    // I/O filename
    std::string ouFilename_add = argv[2];
    std::string ouFilename_proj = argv[3];
    std::string inFilename = argv[1];

    // Read input tiff file
    unsigned int width, height, depth;
    TiffReader imageReader(inFilename.c_str());
    float* raw_data = imageReader.read(&width, &height, &depth);

    // Initialise device, context, and CQ.
    cl_platform_id platform_id = nullptr;
    cl_device_id device_id = nullptr;
    cl_context context;
    cl_command_queue command_queue;
    initialise_gpu(platform_id, device_id, context, command_queue);

    // Push / Create buffer
    // cl_mem raw_data_mem = push<float>(raw_image, context, command_queue);
    // cl_mem add_data_mem = create<float>(add_image, context, command_queue);
    // cl_mem proj_data_mem = create<float>(proj_image, context, command_queue);
    Image<float> raw_image (raw_data, width, height, depth, "float");
    clBuffer gpuRawImage = push(raw_image, "float", context, command_queue);

    std::array<unsigned int, 3> dimensions = {width, height, depth};
    clBuffer gpuAddImage = create<float>(dimensions, "float", context, command_queue);
    dimensions.back() = 1;    
    clBuffer gpuProjImage = create<float>(dimensions, "float", context, command_queue);

    // Apply pipeline of kernels
    addImageAndScalar3d(gpuRawImage, gpuAddImage, 127.0, context, device_id, command_queue);   
    maximumzprojection(gpuAddImage, gpuProjImage, context, device_id, command_queue);   

    // Pull output into container
    Image<float> add_image = pull<float>(gpuAddImage, context, command_queue);    
    Image<float> proj_image = pull<float>(gpuProjImage, context, command_queue);

    // Write add image and scalar test result in tiff
    TiffWriter imageWriter_add (ouFilename_add.c_str());
    imageWriter_add.write(add_image.GetData(), add_image.GetDimensions()[0], add_image.GetDimensions()[1], add_image.GetDimensions()[2]);

    // Write maximum z projection result in tiff
    TiffWriter imageWriter_proj (ouFilename_proj.c_str());
    imageWriter_proj.write(proj_image.GetData(), proj_image.GetDimensions()[0], proj_image.GetDimensions()[1], proj_image.GetDimensions()[2]);

    return EXIT_SUCCESS;
}