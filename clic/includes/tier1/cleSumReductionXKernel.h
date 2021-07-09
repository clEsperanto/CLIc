

#ifndef __cleSumReductionXKernel_h
#define __cleSumReductionXKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumReductionXKernel : public Kernel
{
private:

private:
    std::string source = 
        #include "cle_sum_reduction_x.h" 
        ;

public:
    SumReductionXKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_reduction_x",
                {"dst", "src", "blocksize"}
        )
    {
        m_Sources.insert({this->m_KernelName + "", source});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetBlocksize(int);
    void Execute();

};

} // namespace cle

#endif // __cleSumReductionXKernel_h
