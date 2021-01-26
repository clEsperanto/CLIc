
#ifndef __cleAddImagesWeightedKernel_h
#define __cleAddImagesWeightedKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AddImagesWeightedKernel : public Kernel
{
private:

    void DefineDimensionality();

public:
    AddImagesWeightedKernel(GPU& gpu) : 
    Kernel( gpu,  
            "add_images_weighted",
            {"src", "src1", "dst", "factor", "factor1"}
        )
    {}

    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);
    void SetFactor1(float);
    void SetFactor2(float);
    void Execute();

};

} // namespace cle

#endif // __cleAddImagesWeightedKernel_h
