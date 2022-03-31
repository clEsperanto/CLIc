
#ifndef __cleOnlyzeroOverwriteMaximumDiamondKernel_hpp
#define __cleOnlyzeroOverwriteMaximumDiamondKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class OnlyzeroOverwriteMaximumDiamondKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_onlyzero_overwrite_maximum_diamond.h" 
        };

public:
    OnlyzeroOverwriteMaximumDiamondKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput1(Object&);
    void SetOutput2(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleOnlyzeroOverwriteMaximumDiamondKernel_hpp
