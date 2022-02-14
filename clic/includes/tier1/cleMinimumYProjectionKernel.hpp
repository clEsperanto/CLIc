

#ifndef __cleMinimumYProjectionKernel_hpp
#define __cleMinimumYProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumYProjectionKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_minimum_y_projection.h" 
        };

public:
    MinimumYProjectionKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleMinimumYProjectionKernel_hpp
