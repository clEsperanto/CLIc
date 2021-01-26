
#ifndef __cleMaximumZProjectionKernel_h
#define __cleMaximumZProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumZProjectionKernel : public Kernel
{

public:

    MaximumZProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "maximum_z_projection",
                {"dst_max", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumZProjectionKernel_h
