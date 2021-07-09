

#ifndef __cleMinimumYProjectionKernel_h
#define __cleMinimumYProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumYProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_minimum_y_projection.h" 
        ;

public:
    MinimumYProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "minimum_y_projection",
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

#endif // __cleMinimumYProjectionKernel_h
