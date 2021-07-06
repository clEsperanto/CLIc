
#ifndef __cleSobelKernel_h
#define __cleSobelKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SobelKernel : public Kernel
{

public:
    SobelKernel(GPU& gpu) : 
        Kernel( gpu,
                "sobel",
                {"dst" , "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleSobelKernel_h
