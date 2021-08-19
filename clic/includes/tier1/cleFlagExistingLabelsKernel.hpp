
#ifndef __cleFlagExistingLabelsKernel_hpp
#define __cleFlagExistingLabelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class FlagExistingLabelsKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_flag_existing_labels.h" 
        ;

public:
    FlagExistingLabelsKernel (GPU* gpu) : 
        Kernel( gpu,
                "flag_existing_labels",
                {"dst" , "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "", source});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleFlagExistingLabelsKernel_hpp
