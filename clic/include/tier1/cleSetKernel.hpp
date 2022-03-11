
#ifndef __cleSetKernel_hpp
#define __cleSetKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SetKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_set.h" 
        };

public:
    SetKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleSetKernel_hpp
