
#ifndef __cleAbsoluteKernel_hpp
#define __cleAbsoluteKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class AbsoluteKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_absolute_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_absolute_3d.h" 
        ;

public:
    AbsoluteKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);   // Set<Parameter>(<Type>)
    void SetOutput(Object&);
    void Execute();         
};

} // namespace cle

#endif // __cleAbsoluteKernel_hpp
