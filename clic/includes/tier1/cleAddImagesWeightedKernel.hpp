
#ifndef __cleAddImagesWeightedKernel_hpp
#define __cleAddImagesWeightedKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class AddImagesWeightedKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_add_images_weighted.h" 
        };

public:
    AddImagesWeightedKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void SetFactor1(float);
    void SetFactor2(float);
    void Execute();
};

} // namespace cle

#endif // __cleAddImagesWeightedKernel_hpp
