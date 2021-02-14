

#ifndef __cleSetColumnKernel_h
#define __cleSetColumnKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SetColumnKernel : public Kernel
{

public:
    SetColumnKernel(GPU& gpu) : 
        Kernel( gpu,
                "set_column",
                {"dst" , "column", "value"}
        )
    {}

    void SetInput(Buffer&);
    void SetColumn(int);
    void SetValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleSetColumnKernel_h
