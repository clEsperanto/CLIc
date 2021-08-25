
#ifndef __cleCopyKernel_hpp
#define __cleCopyKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class CopyKernel : public Kernel
{
private:
    std::string source_2d = 
        #include "cle_copy_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_copy_3d.h" 
        ;

public:
    CopyKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleCopyKernel_hpp
