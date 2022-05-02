
#ifndef __cleMultiplyImagesKernel_hpp
#define __cleMultiplyImagesKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MultiplyImagesKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_multiply_images.h" 
        };

public:
    MultiplyImagesKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleMultiplyImagesKernel_hpp
