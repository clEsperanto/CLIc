
#ifndef __cleGreaterConstantKernel_hpp
#define __cleGreaterConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterConstantKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_greater_constant_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_greater_constant_3d.h" 
        ;

public:
    GreaterConstantKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();

};

} // namespace cle

#endif // __cleGreaterConstantKernel_hpp
