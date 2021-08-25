
#ifndef __cleAddImagesWeightedKernel_hpp
#define __cleAddImagesWeightedKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class AddImagesWeightedKernel : public Kernel
{


private:
    std::string source_2d = 
        #include "cle_add_images_weighted_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_add_images_weighted_3d.h" 
        ;

public:
    AddImagesWeightedKernel (std::shared_ptr<GPU> gpu);
    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);
    void SetFactor1(float);
    void SetFactor2(float);
    void Execute();

};

} // namespace cle

#endif // __cleAddImagesWeightedKernel_hpp
