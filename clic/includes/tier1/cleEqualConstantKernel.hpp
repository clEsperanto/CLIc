
#ifndef __cleEqualConstantKernel_hpp
#define __cleEqualConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class EqualConstantKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_equal_constant.h" 
        };

public:
    EqualConstantKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleEqualConstantKernel_hpp
