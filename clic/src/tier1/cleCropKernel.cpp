
#include "cleCropKernel.hpp"    
namespace cle                                
{
    CropKernel::CropKernel(const ProcessorPointer & device) : 
        Operation(device, 5)
    {
        std::string cl_header = {
            #include "cle_crop.h"
        };
        this->SetSource("crop", cl_header);
    }

    auto 
    CropKernel::SetInput(const Image & object) -> void
    {
        this->AddParameter("src", object);
    }

    auto 
    CropKernel::SetOutput(const Image & object) -> void
    {
        this->AddParameter("dst", object);
    }

    auto 
    CropKernel::SetIndex1(const int & index) -> void
    {
        this->AddParameter("index0", index);
    }

    auto 
    CropKernel::SetIndex2(const int & index) -> void
    {
        this->AddParameter("index1", index);
    }

    auto 
    CropKernel::SetIndex3(const int & index) -> void
    {
        this->AddParameter("index2", index);
    }
} 
