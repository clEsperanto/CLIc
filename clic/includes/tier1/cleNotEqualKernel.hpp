
#ifndef __cleNotEqualKernel_hpp
#define __cleNotEqualKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class NotEqualKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_not_equal.h" 
        };

public:
    NotEqualKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleNotEqualKernel_hpp
