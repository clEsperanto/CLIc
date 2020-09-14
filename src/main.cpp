/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <utility>

#include "tiffreader.h"
#include "tiffwriter.h"
#include "image.h"

#include "cleAddImageAndScalar.h"
#include "cleMaximumZProjection.h"


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
    cle::GPU gpu;
    gpu.Initialisation();

    // Push / Create buffer
    Image<float> raw_image (raw_data, width, height, depth, "float");
    cle::Buffer gpuRawImage = gpu.Push<float>(raw_image);

    std::array<unsigned int, 3> dimensions = {width, height, depth};
    cle::Buffer gpuAddImage = gpu.Create<float>(dimensions.data(), "float");
    dimensions.back() = 1;    
    cle::Buffer gpuProjImage = gpu.Create<float>(dimensions.data(), "float");

    // Apply pipeline of kernels
    cle::AddImageAndScalar addImageAndScalar3d(gpu); 
    addImageAndScalar3d.Execute(gpuRawImage, gpuAddImage, 127.0);  

    cle::MaximumZProjection maximumzprojection(gpu); 
    maximumzprojection.Execute(gpuAddImage, gpuProjImage);  

    // Pull output into container
    Image<float> add_image = gpu.Pull<float>(gpuAddImage);    
    Image<float> proj_image = gpu.Pull<float>(gpuProjImage);

    // Write add image and scalar test result in tiff
    TiffWriter imageWriter_add (ouFilename_add.c_str());
    imageWriter_add.write(add_image.GetData(), add_image.GetDimensions()[0], add_image.GetDimensions()[1], add_image.GetDimensions()[2]);

    // Write maximum z projection result in tiff
    TiffWriter imageWriter_proj (ouFilename_proj.c_str());
    imageWriter_proj.write(proj_image.GetData(), proj_image.GetDimensions()[0], proj_image.GetDimensions()[1], proj_image.GetDimensions()[2]);

    return EXIT_SUCCESS;
}