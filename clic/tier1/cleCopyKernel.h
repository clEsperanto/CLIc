
#ifndef __cleCopyKernel_h
#define __cleCopyKernel_h

#include "cleKernel.h"

namespace cle
{
    
class CopyKernel : public Kernel
{
public:
    CopyKernel(GPU& gpu) : 
        Kernel( gpu, 
                "copy",
                {"dst" , "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleCopyKernel_h
