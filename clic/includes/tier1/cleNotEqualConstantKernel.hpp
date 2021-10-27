

#ifndef __cleNotEqualConstantKernel_hpp
#define __cleNotEqualConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class NotEqualConstantKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_not_equal_constant_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_not_equal_constant_3d.h" 
        ;

public:
    NotEqualConstantKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleNotEqualConstantKernel_hpp
