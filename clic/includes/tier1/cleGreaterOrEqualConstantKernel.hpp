
#ifndef __cleGreaterOrEqualConstantKernel_hpp
#define __cleGreaterOrEqualConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterOrEqualConstantKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_greater_or_equal_constant_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_greater_or_equal_constant_3d.h" 
        ;

public:
    GreaterOrEqualConstantKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterOrEqualConstantKernel_hpp
