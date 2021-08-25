
#ifndef __cleSetKernel_hpp
#define __cleSetKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SetKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_set_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_set_3d.h" 
        ;

public:
    SetKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleSetKernel_hpp
