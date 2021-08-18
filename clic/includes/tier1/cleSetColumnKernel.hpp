

#ifndef __cleSetColumnKernel_hpp
#define __cleSetColumnKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SetColumnKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_set_column_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_set_column_3d.h" 
        ;

public:
    SetColumnKernel(GPU& gpu) : 
        Kernel( gpu,
                "set_column",
                {"dst" , "column", "value"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }
    void SetInput(Buffer&);
    void SetColumn(int);
    void SetValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleSetColumnKernel_hpp
