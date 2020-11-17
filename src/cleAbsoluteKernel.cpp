/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleAbsoluteKernel.h"

namespace cle
{

void AbsoluteKernel::DefineDimensionality()
{
    std::string dim = "_2d";
    Buffer* bufferObject = dynamic_cast<Buffer*>(parameterList.at("src"));
    if(bufferObject->GetDimensions()[2] > 1)
    {
        dim = "_3d";
    }
    kernelName = kernelName + dim;
}

void AbsoluteKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void AbsoluteKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

    
void AbsoluteKernel::Execute()
{
    std::cout << "start absolute kernel exe ... "<< std::endl;

        std::cout << "number of parameters : " << this->parameterList.size() << "/" << this->tagList.size() << std::endl;
    for (auto itr = parameterList.begin(); itr != parameterList.end(); ++itr)
    {
        std::cout << itr->first;
        std::cout << "(" << itr->second->GetObjectType() << ")";
        std::cout <<" / ";
    }
    std::cout << std::endl;

    std::cout << "DefineDimensionality ... "<< std::endl;
    DefineDimensionality();
    
    std::cout << "CompileKernel ... "<< std::endl;
    CompileKernel();

    std::cout << "AddArgumentsToKernel ... "<< std::endl;
    AddArgumentsToKernel();

    std::cout << "DefineRangeKernel ... "<< std::endl;
    DefineRangeKernel();

    std::cout << "Done ..." << std::endl;
}

} // namespace cle
