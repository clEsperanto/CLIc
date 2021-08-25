
#ifndef __cleGreaterKernel_hpp
#define __cleGreaterKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_greater_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_greater_3d.h" 
        ;

public:
    GreaterKernel (std::shared_ptr<GPU> gpu);
    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleGreaterKernel_hpp
