
#ifndef __cleNotEqualKernel_hpp
#define __cleNotEqualKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class NotEqualKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_not_equal_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_not_equal_3d.h" 
        ;

public:
    NotEqualKernel (std::shared_ptr<GPU> gpu);
    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);

    void Execute();

};

} // namespace cle

#endif // __cleNotEqualKernel_hpp
