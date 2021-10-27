
#ifndef __cleSmallerKernel_hpp
#define __cleSmallerKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_smaller_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_smaller_3d.h" 
        ;

public:
    SmallerKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleSmallerKernel_hpp
