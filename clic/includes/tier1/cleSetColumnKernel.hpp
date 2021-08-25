

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
    SetColumnKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetColumn(int);
    void SetValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleSetColumnKernel_hpp
