
#ifndef __cleGreaterKernel_hpp
#define __cleGreaterKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterKernel : public Kernel
{

private:
    std::string m_OclHeader = {
        #include "cle_greater.h" 
        };

public:
    GreaterKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterKernel_hpp
