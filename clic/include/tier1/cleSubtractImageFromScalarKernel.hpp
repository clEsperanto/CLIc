
#ifndef __cleSubtractImageFromScalarKernel_hpp
#define __cleSubtractImageFromScalarKernel_hpp

#include "cleKernel.hpp"

namespace cle
{

class SubtractImageFromScalarKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_subtract_image_from_scalar.h" 
        };

public:
    SubtractImageFromScalarKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);    
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleSubtractImageFromScalarKernel_hpp
