
#ifndef __cleDilateSphereKernel_h
#define __cleDilateSphereKernel_h

#include "cleKernel.h"

namespace cle
{
    
class DilateSphereKernel : public Kernel
{

public:
    DilateSphereKernel(GPU& gpu) : 
        Kernel( gpu,
                "dilate_sphere",
                {"src" , "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
    
};

} // namespace cle

#endif // __cleDilateSphereKernel_h
