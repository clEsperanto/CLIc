
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
    CopyKernel (GPU* gpu) : 
        Kernel( gpu, 
                "copy",
                {"dst" , "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleCopyKernel_hpp
