
#ifndef __cleGreaterOrEqualKernel_hpp
#define __cleGreaterOrEqualKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterOrEqualKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_greater_or_equal_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_greater_or_equal_3d.h" 
        ;

public:
    GreaterOrEqualKernel (std::shared_ptr<GPU>);
    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleGreaterKernel_hpp
