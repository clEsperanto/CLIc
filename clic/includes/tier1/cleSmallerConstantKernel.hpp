
#ifndef __cleSmallerConstantKernel_hpp
#define __cleSmallerConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerConstantKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_smaller_constant_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_smaller_constant_3d.h" 
        ;

public:
    SmallerConstantKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetConstant(float);
    void Execute();

};

} // namespace cle

#endif // __cleSmallerConstantKernel_hpp
