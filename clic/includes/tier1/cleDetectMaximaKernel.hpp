
#ifndef __cleDetectMaximaKernel_hpp
#define __cleDetectMaximaKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DetectMaximaKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_detect_maxima.h" 
        };
public:
    DetectMaximaKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleDetectMaximaKernel_hpp
