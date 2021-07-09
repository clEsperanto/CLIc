
#ifndef __cleMaximumYProjectionKernel_h
#define __cleMaximumYProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumYProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_maximum_y_projection.h" 
        ;

public:

    MaximumYProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "maximum_y_projection",
                {"dst_max", "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "", source});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumYProjectionKernel_h
