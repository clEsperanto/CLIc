/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleAddImageAndScalarKernel.h"

namespace cle
{
    
void AddImageAndScalarKernel::DefineDimensionality()
{
    std::string dim = "_2d";
    Buffer* bufferObject = dynamic_cast<Buffer*>(parameterList.at("src"));
    if(bufferObject->GetDimensions()[2] > 1)
    {
        dim = "_3d";
    }
    kernelName = kernelName + dim;
    std::cout << "kernel name : " << kernelName << std::endl;

}

void AddImageAndScalarKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void AddImageAndScalarKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void AddImageAndScalarKernel::SetScalar(float& x)
{
    Float val(x);
    LightObject* obj = dynamic_cast<LightObject*>(&val);
    this->AddObject(obj, "scalar");
}

void AddImageAndScalarKernel::Execute()
{
    std::cout << "Execution kernel" << std::endl;

    std::cout << "number of parameters : " << this->parameterList.size() << "/" << this->tagList.size() << std::endl;
    for (auto itr = parameterList.begin(); itr != parameterList.end(); ++itr)
    {
        std::cout << itr->first;
        std::cout << "(" << itr->second->GetObjectType() << ")";
        std::cout <<" / ";
    }
    std::cout << std::endl;

    DefineDimensionality();
    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
