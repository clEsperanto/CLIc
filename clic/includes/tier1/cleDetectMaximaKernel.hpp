
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

    DetectMaximaKernel(GPU& gpu) : 
        Kernel( gpu, 
                "detect_maxima",
                {"src" , "dst"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleDetectMaximaKernel_hpp
