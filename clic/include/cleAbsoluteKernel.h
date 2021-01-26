
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
                "absolute",
                {"src" , "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleAbsoluteKernel_h
