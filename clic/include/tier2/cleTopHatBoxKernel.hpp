
#ifndef __cleTopHatBoxKernel_hpp
#define __cleTopHatBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class TopHatBoxKernel : public Kernel
{
public:
    TopHatBoxKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetRadius(float =1, float =1, float =1);
    void Execute();

private:
    float m_x =1;
    float m_y =1;
    float m_z =1;
};

} // namespace cle

#endif // __cleTopHatBoxKernel_hpp
