

#ifndef __cleSmallerOrEqualKernel_hpp
#define __cleSmallerOrEqualKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerOrEqualKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_smaller_or_equal_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_smaller_or_equal_3d.h" 
        ;

public:
    SmallerOrEqualKernel (std::shared_ptr<GPU> gpu);
    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleSmallerKernel_hpp
