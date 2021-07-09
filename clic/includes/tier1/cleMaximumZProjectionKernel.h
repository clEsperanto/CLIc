
#ifndef __cleMaximumZProjectionKernel_h
#define __cleMaximumZProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumZProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_maximum_z_projection.h" 
        ;

public:

    MaximumZProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "maximum_z_projection",
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

#endif // __cleMaximumZProjectionKernel_h
