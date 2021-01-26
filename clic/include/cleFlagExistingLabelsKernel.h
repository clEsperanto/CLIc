
#ifndef __cleFlagExistingLabelsKernel_h
#define __cleFlagExistingLabelsKernel_h

#include "cleKernel.h"

namespace cle
{
    
class FlagExistingLabelsKernel : public Kernel
{

public:
    FlagExistingLabelsKernel(GPU& gpu) : 
        Kernel( gpu,
                "flag_existing_labels",
                {"dst" , "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleFlagExistingLabelsKernel_h
