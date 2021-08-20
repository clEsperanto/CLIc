

#ifndef __cleMinimumYProjectionKernel_hpp
#define __cleMinimumYProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumYProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_minimum_y_projection.h" 
        ;

public:
    MinimumYProjectionKernel (std::shared_ptr<GPU> gpu) : 
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

#endif // __cleMinimumYProjectionKernel_hpp
