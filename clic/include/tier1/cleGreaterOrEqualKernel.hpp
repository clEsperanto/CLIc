
#ifndef __cleGreaterOrEqualKernel_hpp
#define __cleGreaterOrEqualKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterOrEqualKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_greater_or_equal.h" 
        };

public:
    GreaterOrEqualKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterKernel_hpp
