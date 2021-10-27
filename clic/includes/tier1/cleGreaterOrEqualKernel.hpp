
#ifndef __cleGreaterOrEqualKernel_hpp
#define __cleGreaterOrEqualKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterOrEqualKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_greater_or_equal_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_greater_or_equal_3d.h" 
        ;

public:
    GreaterOrEqualKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterKernel_hpp
