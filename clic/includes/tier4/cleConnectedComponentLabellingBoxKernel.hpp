
#ifndef __cleConnectedComponentLabellingBoxKernel_hpp
#define __cleConnectedComponentLabellingBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ConnectedComponentLabellingBoxKernel : public Kernel
{

public:
    ConnectedComponentLabellingBoxKernel (std::shared_ptr<GPU> gpu) : 
        Kernel( gpu,
                "connected_components_labelling_box",
                {"src" , "dst"}
        )
    {}
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleConnectedComponentLabellingBoxKernel_hpp
