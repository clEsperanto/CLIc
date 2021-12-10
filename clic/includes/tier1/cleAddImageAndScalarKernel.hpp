
#ifndef __cleAddImageAndScalarKernel_hpp
#define __cleAddImageAndScalarKernel_hpp

#include "cleKernel.hpp"

namespace cle
{

class AddImageAndScalarKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_add_image_and_scalar.h" 
        };

public:
    AddImageAndScalarKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);    
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleAddImageAndScalarKernel_hpp
