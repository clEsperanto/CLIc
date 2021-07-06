
#ifndef __cleAbsoluteKernel_h
#define __cleAbsoluteKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AbsoluteKernel : public Kernel
{
private:
    std::string source_1 = 
        #include "cle_absolute.h" 
        ;
public:
    AbsoluteKernel(GPU& gpu) :    
        Kernel( gpu, 
                "absolute",     // kernel name
                {"src" , "dst"} // parameter tags
        )
    {
            m_Sources.insert({this->m_KernelName, source_1});
    }

    void SetInput(Buffer&);   // Set<Parameter>(<Type>)
    void SetOutput(Buffer&);
    void Execute();         
};

} // namespace cle

#endif // __cleAbsoluteKernel_h
