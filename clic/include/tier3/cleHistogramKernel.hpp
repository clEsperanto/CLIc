
#ifndef __cleHistogramKernel_hpp
#define __cleHistogramKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class HistogramKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_histogram.h" 
        };

public:
    HistogramKernel(std::shared_ptr<GPU>);
    void SetInput(Object&); 
    void SetOutput(Object&);
    void SetMinimumIntensity(float); 
    void SetMaximumIntensity(float);
    void SetSteps(int =1, int =1, int =1);
    void SetNumBins(unsigned int =256);
    void Execute(); 


private:
    float m_MinIntensity = std::numeric_limits<float>::infinity();
    float m_MaxIntensity = std::numeric_limits<float>::infinity();     

};

} // namespace cle

#endif // __cleHistogramKernel_hpp
