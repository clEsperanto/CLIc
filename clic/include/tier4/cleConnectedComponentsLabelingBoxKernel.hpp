
#ifndef __cleConnectedComponentsLabelingBoxKernel_hpp
#define __cleConnectedComponentsLabelingBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ConnectedComponentsLabelingBoxKernel : public Kernel
{
public:
    ConnectedComponentsLabelingBoxKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleConnectedComponentsLabelingBoxKernel_hpp
