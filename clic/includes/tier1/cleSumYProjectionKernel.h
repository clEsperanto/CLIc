

#ifndef __cleSumYProjectionKernel_h
#define __cleSumYProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumYProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_sum_y_projection.h" 
        ;

public:
    SumYProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_y_projection",
                {"dst", "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "", source});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleSumYProjectionKernel_h
