
#ifndef __cleSmallerKernel_hpp
#define __cleSmallerKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_smaller_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_smaller_3d.h" 
        ;

public:
    SmallerKernel (std::shared_ptr<GPU> gpu);
    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleSmallerKernel_hpp
