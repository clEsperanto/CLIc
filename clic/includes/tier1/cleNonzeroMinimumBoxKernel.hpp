

#ifndef __cleNonzeroMinimumBoxKernel_hpp
#define __cleNonzeroMinimumBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class NonzeroMinimumBoxKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_nonzero_minimum_box_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_nonzero_minimum_box_3d.h" 
        ;

public:
    NonzeroMinimumBoxKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetOutputFlag(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleNonzeroMinimumBoxKernel_hpp
