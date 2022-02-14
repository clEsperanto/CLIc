
#ifndef __cleSmallerOrEqualConstantKernel_hpp
#define __cleSmallerOrEqualConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerOrEqualConstantKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_smaller_or_equal_constant.h" 
        };

public:
    SmallerOrEqualConstantKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetConstant(float);
    void Execute();
};

} // namespace cle

#endif // __cleSmallerOrEqualConstantKernel_hpp
