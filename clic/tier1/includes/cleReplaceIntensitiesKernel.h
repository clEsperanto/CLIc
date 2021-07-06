

#ifndef __cleReplaceIntensitiesKernel_h
#define __cleReplaceIntensitiesKernel_h

#include "cleKernel.h"

namespace cle
{
    
class ReplaceIntensitiesKernel : public Kernel
{

public:
    ReplaceIntensitiesKernel(GPU& gpu) : 
        Kernel( gpu,
                "replace_intensities",
                {"dst", "src", "map"}
        )
    {}
    
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetMap(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleReplaceIntensitiesKernel_h
