
#ifndef __cleAbsoluteKernel_hpp
#define __cleAbsoluteKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class AbsoluteKernel : public Kernel
{
private:
    std::string source_2d = 
        #include "cle_absolute_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_absolute_3d.h" 
        ;

public:
    AbsoluteKernel(std::shared_ptr<GPU>);

    void SetInput(Buffer&);   // Set<Parameter>(<Type>)
    void SetOutput(Buffer&);
    void Execute();         
};

} // namespace cle

#endif // __cleAbsoluteKernel_hpp
