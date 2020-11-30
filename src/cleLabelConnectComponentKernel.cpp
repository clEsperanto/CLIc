/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleLabelConnectComponentKernel.h"
#include "cleSetNonzeroPixelsToPixelindexKernel.h"
#include "cleSetKernel.h"
#include "cleNonzeroMinimumBoxKernel.h"
#include "cleCopyKernel.h"

namespace cle
{

void LabelConnectComponentKernel::DefineDimensionality()
{
    std::string dim = "_2d";
    Buffer* bufferObject = dynamic_cast<Buffer*>(parameterList.at("src"));
    if(bufferObject->GetDimensions()[2] > 1)
    {
        dim = "_3d";
    }
    kernelName = kernelName + dim;
}

void LabelConnectComponentKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void LabelConnectComponentKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void LabelConnectComponentKernel::Execute()
{
    std::cout << "start label exe" << std::endl;
    Buffer* src = dynamic_cast<Buffer*>(parameterList.at("src"));
    Buffer* dst = dynamic_cast<Buffer*>(parameterList.at("dst"));
    
    // create temp buffer
    size_t size = src->GetDimensions()[0] * src->GetDimensions()[1] * src->GetDimensions()[2];
    cl_mem tmp1_mem = CreateBuffer<int>(size, this->gpu);
    Buffer temp1 (tmp1_mem, src->GetDimensions(), "int");
    cl_mem tmp2_mem = CreateBuffer<int>(size, this->gpu);
    Buffer temp2 (tmp2_mem, src->GetDimensions(), "int");



    std::cout << "\tSetNonzeroPixelsToPixelindexKernel" << std::endl;

    SetNonzeroPixelsToPixelindexKernel setNonzeroKernel(this->gpu);
    setNonzeroKernel.SetInput(*src);
    setNonzeroKernel.SetOutput(temp1);
    setNonzeroKernel.SetOffset(1);
    setNonzeroKernel.Execute();

        std::cout << "\tSetKernel" << std::endl;


    SetKernel setInit(this->gpu);
    setInit.SetInput(temp2);
    setInit.SetValue(0);
    setInit.Execute();

    int flag_value = 1;
    int iteration_count = 0;

    unsigned int flag_dim[3] = {1,1,1};
    cl_mem flag_mem = CreateBuffer<int>(1, this->gpu);
    Buffer flag (flag_mem, flag_dim, "int");

            std::cout << "\tStartLoop" << std::endl;


    while (flag_value > 0)
    {

                    std::cout << "\t\tNonzeroMinimumBoxKernel" << std::endl;

        NonzeroMinimumBoxKernel nonzeroMinBox(this->gpu);
        nonzeroMinBox.SetOutputFlag(flag);
        if (iteration_count % 2 == 0)
        {
            nonzeroMinBox.SetInput(temp1);
            nonzeroMinBox.SetOutput(temp2);
        }
        else
        {
            nonzeroMinBox.SetInput(temp2);
            nonzeroMinBox.SetOutput(temp1);
        }
        nonzeroMinBox.Execute();

                    std::cout << "\t\tReadFlag" << std::endl;

        flag_value = ReadBuffer<int>(flag_mem, 1, this->gpu)[0];
        
                            std::cout << "\t\tSet" << std::endl;

        SetKernel set(this->gpu);
        set.SetInput(temp2);
        set.SetValue(flag_value);
        set.Execute();

        iteration_count++;
    }
    
    CopyKernel copy(this->gpu);
    if (iteration_count % 2 == 0)
    {    
        copy.SetInput(temp1);
    }
    else
    {
        copy.SetInput(temp2);
    }
    copy.SetOutput(*dst);
    copy.Execute();
}

} // namespace cle
