

#ifndef __cleNonzeroMinimumBoxKernel_hpp
#define __cleNonzeroMinimumBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class NonzeroMinimumBoxKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_nonzero_minimum_box.h" 
        };

public:
    NonzeroMinimumBoxKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetOutputFlag(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleNonzeroMinimumBoxKernel_hpp
