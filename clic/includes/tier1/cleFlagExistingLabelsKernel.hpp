
#ifndef __cleFlagExistingLabelsKernel_hpp
#define __cleFlagExistingLabelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class FlagExistingLabelsKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_flag_existing_labels.h" 
        };

public:
    FlagExistingLabelsKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleFlagExistingLabelsKernel_hpp
