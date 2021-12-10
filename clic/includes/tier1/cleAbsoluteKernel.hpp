
#ifndef __cleAbsoluteKernel_hpp
#define __cleAbsoluteKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class AbsoluteKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_absolute.h" 
        };

public:
    AbsoluteKernel(std::shared_ptr<GPU>);
    void SetInput(Object&); 
    void SetOutput(Object&);
    void Execute();         
};

} // namespace cle

#endif // __cleAbsoluteKernel_hpp
