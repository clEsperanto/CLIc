

#ifndef __cleNonzeroMinimumBoxKernel_hpp
#define __cleNonzeroMinimumBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class NonzeroMinimumBoxKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_nonzero_minimum_box_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_nonzero_minimum_box_3d.h" 
        ;

public:
    NonzeroMinimumBoxKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetOutputFlag(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleNonzeroMinimumBoxKernel_hpp
