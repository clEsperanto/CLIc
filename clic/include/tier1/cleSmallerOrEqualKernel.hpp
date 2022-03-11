

#ifndef __cleSmallerOrEqualKernel_hpp
#define __cleSmallerOrEqualKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerOrEqualKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_smaller_or_equal.h" 
        };

public:
    SmallerOrEqualKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleSmallerKernel_hpp
