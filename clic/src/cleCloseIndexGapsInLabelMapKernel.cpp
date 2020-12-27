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
#include "cleSumReductionKernel.h"
#include "cleSetColumnKernel.h"
#include "cleBlockEnumerateKernel.h"
#include "cleReplaceIntensitiesKernel.h"


namespace cle
{

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

    float max_value = ReadBuffer<float>(max->GetData(), max->GetBitSize() * 1, this->gpu)[0];
    unsigned int nb_indices =  int(max_value) + 1;
    std::cout << "nb indices = " << nb_indices << std::endl;

    unsigned int indices_dim[3] = {nb_indices, 1, 1};
    cl_mem flaggedIndices_mem = CreateBuffer<float>(src->GetBitSize() * indices_dim[0], this->gpu);
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

    for (size_t i = 0; i < indices_dim[0]; i++)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    float* flaggedIndices_arr = ReadBuffer<float>(flaggedIndices->GetData(), flaggedIndices->GetBitSize() * indices_dim[0], this->gpu);
    for (size_t i = 0; i < indices_dim[0]; i++)
    {
        std::cout << flaggedIndices_arr[i] << " ";
    }
    std::cout << std::endl;

    SetColumnKernel setColumn(this->gpu);
    setColumn.SetInput(*flaggedIndices);
    setColumn.SetColumn(0);
    setColumn.SetValue(0);
    setColumn.Execute();
    
    float* flaggedIndices_arr2 = ReadBuffer<float>(flaggedIndices->GetData(), flaggedIndices->GetBitSize() * indices_dim[0], this->gpu);
    for (size_t i = 0; i < indices_dim[0]; i++)
    {
        std::cout << flaggedIndices_arr2[i] << " ";
    }
    std::cout << std::endl;

    // # sum existing labels per blocks

    unsigned int nb_sums = int(nb_indices / this->blocksize) + 1;
    unsigned int sums_dim[3] = {nb_sums, 1, 1};
    cl_mem sums_mem = CreateBuffer<float>(flaggedIndices->GetBitSize() * sums_dim[0], this->gpu);
    Buffer* blockSums  = new Buffer(sums_mem, sums_dim, flaggedIndices->GetDataType());


    SumReductionKernel sumReduction(this->gpu);
    sumReduction.SetInput(*flaggedIndices);
    sumReduction.SetOutput(*blockSums);
    sumReduction.SetBlocksize(this->blocksize);
    sumReduction.Execute();



    unsigned int newIndices_dim[3] = {nb_indices, 1, 1};
    cl_mem newIndices_mem = CreateBuffer<float>(flaggedIndices->GetBitSize() * newIndices_dim[0], this->gpu);
    Buffer* newIndices = new Buffer(newIndices_mem, newIndices_dim, flaggedIndices->GetDataType());

    BlockEnumerateKernel blockEnumerate(this->gpu);
    blockEnumerate.SetInput(*flaggedIndices);
    blockEnumerate.SetInputSums(*blockSums);
    blockEnumerate.SetOutput(*newIndices);
    blockEnumerate.SetBlocksize(this->blocksize);
    blockEnumerate.Execute();


    float* newIndices_arr = ReadBuffer<float>(newIndices->GetData(), newIndices->GetBitSize() * newIndices_dim[0], this->gpu);
    for (size_t i = 0; i < newIndices_dim[0]; i++)
    {
        std::cout << newIndices_arr[i] << " ";
    }
    std::cout << std::endl;

    // replace_intensities(input, new_indices, output)
    ReplaceIntensitiesKernel replaceIntensities(this->gpu);
    replaceIntensities.SetInput(*src);
    replaceIntensities.SetOutput(*dst);
    replaceIntensities.SetMap(*newIndices);
    replaceIntensities.Execute();

    float* arr = ReadBuffer<float>(dst->GetData(), dst->GetBitSize() * arr_size, this->gpu);
    for (size_t i = 0; i < arr_size; i++)
    {
        if (i % dst->GetDimensions()[0] == 0)
        {
            std::cout << std::endl;
        }
        
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

}

} // namespace cle
