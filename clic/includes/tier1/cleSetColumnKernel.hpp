

#ifndef __cleSetColumnKernel_hpp
#define __cleSetColumnKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SetColumnKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_set_column_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_set_column_3d.h" 
        ;

public:
    SetColumnKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetColumn(int);
    void SetValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleSetColumnKernel_hpp
