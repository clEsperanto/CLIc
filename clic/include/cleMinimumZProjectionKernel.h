

#ifndef __cleMinimumZProjectionKernel_h
#define __cleMinimumZProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumZProjectionKernel : public Kernel
{

public:
    MinimumZProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "minimum_z_projection",
                {"dst_min", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleMinimumZProjectionKernel_h
