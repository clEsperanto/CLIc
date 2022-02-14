
#ifndef __cleSobelKernel_hpp
#define __cleSobelKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SobelKernel : public Kernel
{
private:
    std::string m_OclHeader =  {
        #include "cle_sobel.h" 
    };

public:
    SobelKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleSobelKernel_hpp
