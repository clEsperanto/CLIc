/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleCloseIndexGapsInLabelMapKernel.h"

#include "cleMaximumOfAllPixelsKernel.h"
#include "cleFlagExistingIntensitiesKernel.h"
#include "cleSetKernel.h"
#include "cleSetColumnKernel.h"


namespace cle
{
    
void CloseIndexGapsInLabelMapKernel::DefineDimensionality()
{
    std::string dim = "_2d";
    Buffer* bufferObject = dynamic_cast<Buffer*>(parameterList.at("src"));
    if(bufferObject->GetDimensions()[2] > 1)
    {
        dim = "_3d";
    }
    kernelName = kernelName + dim;
}

void CloseIndexGapsInLabelMapKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void CloseIndexGapsInLabelMapKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void CloseIndexGapsInLabelMapKernel::SetBlockSize(int x)
{
    this->blocksize = x;
}

void CloseIndexGapsInLabelMapKernel::Execute()
{

    std::cout << "close gaps kernel ... " << std::endl;

    Buffer* src = dynamic_cast<Buffer*>(parameterList.at("src"));
    Buffer* dst = dynamic_cast<Buffer*>(parameterList.at("dst"));

    size_t arr_size = src->GetDimensions()[0] * src->GetDimensions()[1] * src->GetDimensions()[2];
    unsigned int one_dim[3] = {1,1,1};

    cl_mem max_mem = CreateBuffer<float>(sizeof(float), this->gpu);
    Buffer* max  = new Buffer(max_mem, one_dim, src->GetDataType());



    float* src_arr = ReadBuffer<float>(src->GetData(), src->GetBitSize() * arr_size, this->gpu);
    for (size_t i = 0; i < arr_size; i++)
    {
        if (i % 10 == 0) {
            std::cout << std::endl;
        }
        std::cout << src_arr[i] << " ";
    }
    std::cout << std::endl;



    // max_label = maximum_of_all_pixels(input)
    MaximumOfAllPixelsKernel maximumOfPixels(this->gpu);
    maximumOfPixels.SetInput(*src);
    maximumOfPixels.SetOutput(*max);
    maximumOfPixels.Execute();

    float* max_value = ReadBuffer<float>(max->GetData(), max->GetBitSize() * 1, this->gpu);
    unsigned int max_indices = int(max_value[0]) + 1;
    std::cout << "maximum indices + 1 : " << max_indices << std::endl;

    unsigned int indices_dim[3] = {max_indices,1,1};
    cl_mem flaggedIndices_mem = CreateBuffer<float>(src->GetBitSize() * max_indices, this->gpu);
    Buffer* flaggedIndices  = new Buffer(flaggedIndices_mem, indices_dim, src->GetDataType());

    SetKernel set(this->gpu);
    set.SetInput(*flaggedIndices);
    set.SetValue(0);
    set.Execute();


    std::cout << "intensity list array : " << std::endl; 

    FlagExistingIntensitiesKernel flagIntensity(this->gpu);
    flagIntensity.SetInput(*src);
    flagIntensity.SetOutput(*flaggedIndices);
    flagIntensity.Execute();

    for (size_t i = 0; i < max_indices; i++)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    float* flaggedIndices_arr = ReadBuffer<float>(flaggedIndices->GetData(), flaggedIndices->GetBitSize() * max_indices, this->gpu);
    for (size_t i = 0; i < max_indices; i++)
    {
        std::cout << flaggedIndices_arr[i] << " ";
    }
    std::cout << std::endl;

    SetColumnKernel setColumn(this->gpu);
    setColumn.SetInput(*flaggedIndices);
    setColumn.SetColumn(0);
    setColumn.SetValue(0);
    setColumn.Execute();
    
    float* flaggedIndices_arr2 = ReadBuffer<float>(flaggedIndices->GetData(), flaggedIndices->GetBitSize() * max_indices, this->gpu);
    
    for (size_t i = 0; i < max_indices; i++)
    {
        std::cout << flaggedIndices_arr2[i] << " ";
    }
    std::cout << std::endl;

    // # sum existing labels per blocks


    // unsigned int indices_dim[3] = {max_indices,1,1};
    // cl_mem flaggedIndices_mem = CreateBuffer<float>(src->GetBitSize() * max_indices, this->gpu);
    // Buffer* flaggedIndices  = new Buffer(flaggedIndices_mem, indices_dim, src->GetDataType());



    // block_sums = create([1, int((int(max_label) + 1) / blocksize) + 1])
    // sum_reduction_x(flagged_indices, block_sums, blocksize)

    // # distribute new numbers
    // new_indices = create([1, int(max_label) + 1])
    // block_enumerate(flagged_indices, block_sums, new_indices, blocksize)

    // replace_intensities(input, new_indices, output)

}

} // namespace cle
