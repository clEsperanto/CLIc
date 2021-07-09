

#ifndef __cleMinimumZProjectionKernel_h
#define __cleMinimumZProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumZProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_minimum_z_projection.h" 
        ;

public:
    MinimumZProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "minimum_z_projection",
                {"dst_min", "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "", source});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleMinimumZProjectionKernel_h
