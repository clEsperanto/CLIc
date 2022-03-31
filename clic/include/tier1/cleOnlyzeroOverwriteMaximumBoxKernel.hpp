
#ifndef __cleOnlyzeroOverwriteMaximumBoxKernel_hpp
#define __cleOnlyzeroOverwriteMaximumBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class OnlyzeroOverwriteMaximumBoxKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_onlyzero_overwrite_maximum_box.h" 
        };

public:
    OnlyzeroOverwriteMaximumBoxKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput1(Object&);
    void SetOutput2(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleOnlyzeroOverwriteMaximumBoxKernel_hpp
