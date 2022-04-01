
#ifndef __cleMaskKernel_hpp
#define __cleMaskKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaskKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_mask.h" 
        };

public:
    MaskKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetMask(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterKernel_hpp