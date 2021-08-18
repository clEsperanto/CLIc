

#ifndef __cleSumZProjectionKernel_hpp
#define __cleSumZProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SumZProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_sum_z_projection.h" 
        ;

public:
    SumZProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_z_projection",
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

#endif // __cleSumZProjectionKernel_hpp
