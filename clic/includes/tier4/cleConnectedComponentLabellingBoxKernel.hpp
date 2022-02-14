
#ifndef __cleConnectedComponentLabellingBoxKernel_hpp
#define __cleConnectedComponentLabellingBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ConnectedComponentLabellingBoxKernel : public Kernel
{
public:
    ConnectedComponentLabellingBoxKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleConnectedComponentLabellingBoxKernel_hpp
