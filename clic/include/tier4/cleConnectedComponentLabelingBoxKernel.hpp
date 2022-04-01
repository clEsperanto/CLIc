
#ifndef __cleConnectedComponentLabelingBoxKernel_hpp
#define __cleConnectedComponentLabelingBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ConnectedComponentLabelingBoxKernel : public Kernel
{
public:
    ConnectedComponentLabelingBoxKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleConnectedComponentLabelingBoxKernel_hpp
