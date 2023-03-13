
#ifndef __TIER1_CLECROPKERNEL_HPP        
#define __TIER1_CLECROPKERNEL_HPP        

#include "cleOperation.hpp"                 

namespace cle                                        
{

class CropKernel : public Operation     
{                                                     
    public:
        explicit CropKernel(const ProcessorPointer & device);

        auto 
        SetInput(const Image & object) -> void;

        auto 
        SetOutput(const Image & object) -> void;

        auto
        SetIndex1(const int & index) -> void;

        auto
        SetIndex2(const int & index) -> void;

        auto 
        SetIndex3(const int & index) -> void;
    
};

CropKernel_Call(const std::sharedptr<cle::Processor> & device, 
                const Image & src,
                const Image & dst, 
                const Int & index0,
                const Int & index1,
                const Int & index2
                ) -> void
{
    CropKernel kernel(device);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetIndex1(index0);
    kernel.SetIndex2(index1);
    kernel.SetIndex3(index2);
    kernel.Execute();
}

}                                  

#endif 