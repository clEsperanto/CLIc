
#ifndef __cleCopyKernel_hpp
#define __cleCopyKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class CopyKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_copy_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_copy_3d.h" 
        ;

public:
    CopyKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleCopyKernel_hpp
