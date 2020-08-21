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

#include "utils.h"
#include "tiffreader.h"
#include "tiffwriter.h"
#include "image.h"


/**
 * Push local array into buffer.
 */
template<class T>
cl_mem push(Image<T>& img, cl_context context, cl_command_queue command_queue)
{
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, img.GetDataSize(), img.GetData(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to create buffer in push() : " << getOpenCLErrorString(clError) << std::endl;
    }
    clError = clEnqueueWriteBuffer(command_queue, mem_obj, CL_TRUE, 0, img.GetDataSize(), img.GetData(), 0, NULL, NULL);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to write buffer in push() " << getOpenCLErrorString(clError) << std::endl;
    }
    return mem_obj;
}


/**
 * Create an empty array into buffer.
 */
template<class T>
cl_mem create(Image<T>& img, cl_context context, cl_command_queue command_queue)
{
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, img.GetDataSize(), img.GetData(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to create buffer in create() : " << getOpenCLErrorString(clError) << std::endl;
    }
    return mem_obj;
}


/**
 * Read array from buffer.
 */
template<class T>
void pull(Image<T>& img, cl_mem mem_obj, cl_context context, cl_command_queue command_queue)
{
    size_t size = sizeof(T) * img.GetNbPixels();
    T* array = new T[img.GetNbPixels()];
    cl_int clError = clEnqueueReadBuffer(command_queue, mem_obj, CL_TRUE, 0, size, array, 0, NULL, NULL);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to read buffer in pull() : " << getOpenCLErrorString(clError) << std::endl;
    }
    img.SetData(array);
}


/**
 * Get preamble source code
 */
std::string LoadPreamble()
{
    std::string preamble;
    const std::string preamble_path = CLP_PATH;
    std::string filename = preamble_path + filesep + "preamble.cl";
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
    std::string filename = kernels_path + filesep + kernelFilename + "_x.cl";
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
 * Set OpenCL Defines variable.
 */
template<class T>
std::string LoadDefines(Image<T>& src, Image<T>& dst)
{
    Image<T> imageList[2] = {src, dst};
    // imageList[0] = src;
    // imageList[1] = dst;

    std::string defines;
    defines = defines + "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";
    defines = defines + "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT";
    defines = defines + "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";
    defines = defines + "\n";   

    for (size_t i = 0; i < 2; i++)
    {
        // image type handling
        defines = defines + "\n#define CONVERT_" + imageList[i].GetKey() + "_PIXEL_TYPE clij_convert_" + imageList[i].GetType() + "_sat";
        defines = defines + "\n#define IMAGE_" + imageList[i].GetKey() + "_TYPE __global " + imageList[i].GetType() + "*";
        defines = defines + "\n#define IMAGE_" + imageList[i].GetKey() + "_PIXEL_TYPE " + imageList[i].GetType();

        // image size handling
        if (imageList[i].GetDimension()[2] > 1)
        {
            defines = defines + "\n#define IMAGE_SIZE_" + imageList[i].GetKey() + "_WIDTH " + std::to_string(imageList[i].GetDimension()[0]);
            defines = defines + "\n#define IMAGE_SIZE_" + imageList[i].GetKey() + "_HEIGHT " + std::to_string(imageList[i].GetDimension()[1]);
            defines = defines + "\n#define IMAGE_SIZE_" + imageList[i].GetKey() + "_DEPTH " + std::to_string(imageList[i].GetDimension()[2]);
        }
        else
        {
            if (imageList[i].GetDimension()[1] > 1)
            {
                defines = defines + "\n#define IMAGE_SIZE_" + imageList[i].GetKey() + "_WIDTH " + std::to_string(imageList[i].GetDimension()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + imageList[i].GetKey() + "_HEIGHT " + std::to_string(imageList[i].GetDimension()[1]);
            }
            else
            {
                defines = defines + "\n#define IMAGE_SIZE_" + imageList[i].GetKey() + "_WIDTH " + std::to_string(imageList[i].GetDimension()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + imageList[i].GetKey() + "_HEIGHT 1";
            }
            defines = defines + "\n#define IMAGE_SIZE_" + imageList[i].GetKey() + "_DEPTH 1";
        }

        // position (dimensionality) handling
        if (imageList[i].GetDimension()[2] == 1)
        {
            defines = defines + "\n#define POS_" + imageList[i].GetKey() + "_TYPE int2";
            if (imageList[i].GetDimension()[1] == 1) // 1D
            {
                defines = defines + "\n#define POS_" + imageList[i].GetKey() + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, 0)";
            }
            else // 2D
            {
                defines = defines + "\n#define POS_" + imageList[i].GetKey() + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, pos1)";
            }
        }
        else // 3/4D
        {
            defines = defines + "\n#define POS_" + imageList[i].GetKey() + "_TYPE int4";
            defines =
                defines + "\n#define POS_" + imageList[i].GetKey() + "_INSTANCE(pos0,pos1,pos2,pos3) (int4)(pos0, pos1, pos2, 0)";
        }

        // read/write images
        std::string sdim = (imageList[i].GetDimension()[2] == 1) ? "2" : "3";
        defines = defines + "\n#define READ_" + imageList[i].GetKey() + "_IMAGE(a,b,c) read_buffer" + sdim + "d" + TypeId(imageList[i].GetType()) +
                  "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
        defines = defines + "\n#define WRITE_" + imageList[i].GetKey() + "_IMAGE(a,b,c) write_buffer" + sdim + "d" + TypeId(imageList[i].GetType()) +
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
template<class T>
int maximumzprojection(cl_mem src_mem_obj, Image<T>& src, cl_mem dst_mem_obj, Image<T>& dst,
                       cl_context context, cl_device_id device_id, cl_command_queue command_queue)
{
    // initialise information on kernel and data to process
    cl_int clError;
    std::string kernel_name = "maximum_z_projection";

    // read kernel, defines, and preamble
    std::string kernel_src = LoadSources(kernel_name);
    std::string defines_src = LoadDefines<T>(src, dst);
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
    clError = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
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
    clError = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&dst_mem_obj);
    clError = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&src_mem_obj);

    // execute the opencl kernel
    size_t global_item_size[3] = {std::max(src.GetDimension()[0], dst.GetDimension()[0]), std::max(src.GetDimension()[1], dst.GetDimension()[1]), std::max(src.GetDimension()[2], dst.GetDimension()[2])};
    size_t local_item_size[3] = {1, 1, std::max(src.GetDimension()[2], dst.GetDimension()[2])};
    size_t work_dim = 3;
    clError = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_item_size, local_item_size, 0, NULL, NULL);
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
template<class T>
int addImageAndScalar3d(cl_mem src_mem_obj, Image<T>& src, cl_mem dst_mem_obj, Image<T>& dst, float scalar,
                       cl_context context, cl_device_id device_id, cl_command_queue command_queue)
{
    // initialise information on kernel and data to process
    cl_int clError;
    std::string kernel_name = "add_image_and_scalar_3d";

    // read kernel, defines, and preamble
    std::string kernel_src = LoadSources(kernel_name);
    std::string defines_src = LoadDefines<T>(src, dst);
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
    clError = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
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
    clError = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&src_mem_obj);
    clError = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&dst_mem_obj);
    clError = clSetKernelArg(kernel, 2, sizeof(float), (void *)&scalar);

    // execute the opencl kernel
    size_t global_item_size[3] = {std::max(src.GetDimension()[0], dst.GetDimension()[0]), std::max(src.GetDimension()[1], dst.GetDimension()[1]), std::max(src.GetDimension()[2], dst.GetDimension()[2])};
    size_t local_item_size[3] = {1, 1, 1};
    size_t work_dim = 3;
    clError = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_item_size, local_item_size, 0, NULL, NULL);
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
    command_queue = clCreateCommandQueue(context, device_id, NULL, &clError);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "OpenCL Error! Could not create commande queue : " << getOpenCLErrorString(clError) << std::endl;
        return EXIT_FAILURE;
    }
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

    // I/O filename
    std::string ouFilename_add = "./images/output_add_image_and_scalar.tif";
    std::string ouFilename_proj = "./images/output_maximum_z_projection.tif";
    std::string inFilename = "./images/input.tif";

    // Read input tiff file
    unsigned int width, height, depth;
    TiffReader imageReader(inFilename.c_str());
    float* raw_data = imageReader.read(&width, &height, &depth);

    // Initialise image container for host with information
    Image<float> raw_image (raw_data, width, height, depth, "src", "float");
    Image<float> add_image (width, height, depth, "dst", "float");
    Image<float> proj_image (width, height, (unsigned int) 1, "dst_max", "float");


    // Initialise device, context, and CQ.
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_context context;
    cl_command_queue command_queue;
    initialise_gpu(platform_id, device_id, context, command_queue);

    // Push / Create buffer
    cl_mem raw_data_mem = push<float>(raw_image, context, command_queue);
    cl_mem add_data_mem = create<float>(add_image, context, command_queue);
    cl_mem proj_data_mem = create<float>(proj_image, context, command_queue);

    // Apply first kernel
    addImageAndScalar3d<float>(raw_data_mem, raw_image, add_data_mem, add_image, 127.0, context, device_id, command_queue);
    
    // Pull output into container, update key for next usage  
    pull<float>(add_image, add_data_mem, context, command_queue);
    add_image.SetKey("src");
    
    // Apply second kernel
    maximumzprojection<float>(add_data_mem, add_image, proj_data_mem, proj_image, context, device_id, command_queue);   

    // Pull output into container
    pull<float>(proj_image, proj_data_mem, context, command_queue);

    // Write add image and scalar test result in tiff
    TiffWriter imageWriter_add (ouFilename_add.c_str());
    imageWriter_add.write(add_image.GetData(), add_image.GetDimension()[0], add_image.GetDimension()[1], add_image.GetDimension()[2]);

    // Write maximum z projection result in tiff
    TiffWriter imageWriter_proj (ouFilename_proj.c_str());
    imageWriter_proj.write(proj_image.GetData(), proj_image.GetDimension()[0], proj_image.GetDimension()[1], proj_image.GetDimension()[2]);

    return EXIT_SUCCESS;
}