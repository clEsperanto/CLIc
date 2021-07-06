
#ifndef __cleAbsoluteKernel_h
#define __cleAbsoluteKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AbsoluteKernel : public Kernel
{
public:
    AbsoluteKernel(GPU& gpu) :    
        Kernel( gpu, 
                "absolute",     // kernel name
                {"src" , "dst"} // parameter tags
        )
    {}

    void SetInput(Buffer&);   // Set<Parameter>(<Type>)
    void SetOutput(Buffer&);
    void Execute();         
};

} // namespace cle

#endif // __cleAbsoluteKernel_h
