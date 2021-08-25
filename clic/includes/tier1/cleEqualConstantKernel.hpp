
#ifndef __cleEqualConstantKernel_hpp
#define __cleEqualConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class EqualConstantKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_equal_constant_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_equal_constant_3d.h" 
        ;

public:
    EqualConstantKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleEqualConstantKernel_hpp
