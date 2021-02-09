
#ifndef __cleDetectMaximaKernel_h
#define __cleDetectMaximaKernel_h

#include "cleKernel.h"

namespace cle
{
    
class DetectMaximaKernel : public Kernel
{

public:

    DetectMaximaKernel(GPU& gpu) : 
        Kernel( gpu, 
                "detect_maxima",
                {"src" , "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleDetectMaximaKernel_h
