
#ifndef __cleSumXProjectionKernel_h
#define __cleSumXProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumXProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_sum_x_projection.h" 
        ;

public:
    SumXProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_x_projection",
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

#endif // __cleSumXProjectionKernel_h
