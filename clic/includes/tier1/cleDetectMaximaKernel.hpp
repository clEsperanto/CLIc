
#ifndef __cleDetectMaximaKernel_hpp
#define __cleDetectMaximaKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DetectMaximaKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_detect_maxima_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_detect_maxima_3d.h" 
        ;

public:

    DetectMaximaKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleDetectMaximaKernel_hpp
