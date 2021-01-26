
#ifndef __cleNotEqualKernel_h
#define __cleNotEqualKernel_h

#include "cleKernel.h"

namespace cle
{
    
class NotEqualKernel : public Kernel
{

public:
    NotEqualKernel(GPU& gpu) : 
        Kernel( gpu,
                "not_equal",
                {"src1", "src2", "dst"}
        )
    {}
    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);

    void Execute();

};

} // namespace cle

#endif // __cleNotEqualKernel_h
