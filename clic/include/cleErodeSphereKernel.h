
#ifndef __cleErodeSphereKernel_h
#define __cleErodeSphereKernel_h

#include "cleKernel.h"

namespace cle
{
    
class ErodeSphereKernel : public Kernel
{

public:
    ErodeSphereKernel(GPU& gpu) : 
        Kernel( gpu,
                "erode_sphere",
                {"src" , "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleErodeSphereKernel_h
