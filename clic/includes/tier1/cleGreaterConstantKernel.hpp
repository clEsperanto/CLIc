
#ifndef __cleGreaterConstantKernel_hpp
#define __cleGreaterConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterConstantKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_greater_constant.h" 
        };

public:
    GreaterConstantKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterConstantKernel_hpp
