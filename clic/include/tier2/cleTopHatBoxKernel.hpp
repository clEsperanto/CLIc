
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
    void SetRadius(int =0, int =0, int =0);
    void Execute();

private:
    int m_x =0;
    int m_y =0;
    int m_z =0;
};

} // namespace cle

#endif // __cleTopHatBoxKernel_hpp
