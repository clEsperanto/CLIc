
#ifndef __cleSmallerConstantKernel_hpp
#define __cleSmallerConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerConstantKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_smaller_constant.h" 
        };

public:
    SmallerConstantKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetConstant(float);
    void Execute();
};

} // namespace cle

#endif // __cleSmallerConstantKernel_hpp
