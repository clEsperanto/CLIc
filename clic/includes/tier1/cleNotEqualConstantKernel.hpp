

#ifndef __cleNotEqualConstantKernel_hpp
#define __cleNotEqualConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class NotEqualConstantKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_not_equal_constant.h" 
        };

public:
    NotEqualConstantKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleNotEqualConstantKernel_hpp
