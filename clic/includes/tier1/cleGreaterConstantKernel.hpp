
#ifndef __cleGreaterConstantKernel_hpp
#define __cleGreaterConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterConstantKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_greater_constant_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_greater_constant_3d.h" 
        ;

public:
    GreaterConstantKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterConstantKernel_hpp
