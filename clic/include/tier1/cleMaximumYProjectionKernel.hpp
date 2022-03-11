
#ifndef __cleMaximumYProjectionKernel_hpp
#define __cleMaximumYProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaximumYProjectionKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_maximum_y_projection.h" 
        };

public:
    MaximumYProjectionKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumYProjectionKernel_hpp
