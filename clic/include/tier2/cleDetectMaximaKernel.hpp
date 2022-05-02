
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
    void SetRadius(int =1, int =1, int =1);
    void Execute();
private:
    int m_x = 1;
    int m_y = 1;
    int m_z = 1;
};

} // namespace cle

#endif // __cleDetectMaximaKernel_hpp
