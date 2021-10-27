
#ifndef __cleSobelKernel_hpp
#define __cleSobelKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SobelKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_sobel_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_sobel_3d.h" 
        ;

public:
    SobelKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleSobelKernel_hpp
