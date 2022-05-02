
#ifndef __cleDilateLabelsKernel_hpp
#define __cleDilateLabelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DilateLabelsKernel : public Kernel
{
public:
    DilateLabelsKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetRadius(int =1);
    void Execute();
private:
    int m_Radius = 1;
};

} // namespace cle

#endif // __cleDilateLabelsKernel_hpp