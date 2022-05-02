
#ifndef __cleDivideImagesKernel_hpp
#define __cleDivideImagesKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DivideImagesKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_divide_images.h" 
        };

public:
    DivideImagesKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleDivideImagesKernel_hpp
