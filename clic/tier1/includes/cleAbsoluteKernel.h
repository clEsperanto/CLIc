
#ifndef __cleAbsoluteKernel_h
#define __cleAbsoluteKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AbsoluteKernel : public Kernel
{
private:
    std::string source_2d = 
        #include "cle_absolute_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_absolute_3d.h" 
        ;

public:
    AbsoluteKernel(GPU& gpu) :    
        Kernel( gpu, 
                "absolute",     // kernel name
                {"src" , "dst"} // parameter tags
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }

    void SetInput(Buffer&);   // Set<Parameter>(<Type>)
    void SetOutput(Buffer&);
    void Execute();         
};

} // namespace cle

#endif // __cleAbsoluteKernel_h
