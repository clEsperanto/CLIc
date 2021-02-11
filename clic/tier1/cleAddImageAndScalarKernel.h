
#ifndef __cleAddImageAndScalarKernel_h
#define __cleAddImageAndScalarKernel_h

#include "cleKernel.h"

namespace cle
{

class AddImageAndScalarKernel : public Kernel
{
    
public:

    AddImageAndScalarKernel(GPU gpu) : 
        Kernel( gpu, 
                "add_image_and_scalar", 
                {"src", "dst", "scalar"}
            ) 
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();

};

} // namespace cle

#endif // __cleAddImageAndScalarKernel_h
