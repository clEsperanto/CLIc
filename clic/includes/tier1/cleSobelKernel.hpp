
#ifndef __cleSobelKernel_hpp
#define __cleSobelKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SobelKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_sobel_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_sobel_3d.h" 
        ;

public:
    SobelKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleSobelKernel_hpp
