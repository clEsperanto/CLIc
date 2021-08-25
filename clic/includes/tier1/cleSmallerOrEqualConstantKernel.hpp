
#ifndef __cleSmallerOrEqualConstantKernel_hpp
#define __cleSmallerOrEqualConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerOrEqualConstantKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_smaller_or_equal_constant_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_smaller_or_equal_constant_3d.h" 
        ;

public:
    SmallerOrEqualConstantKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetConstant(float);
    void Execute();

};

} // namespace cle

#endif // __cleSmallerOrEqualConstantKernel_hpp
