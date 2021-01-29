
#ifndef __cleConnectedComponentLabellingBoxKernel_h
#define __cleConnectedComponentLabellingBoxKernel_h

#include "cleKernel.h"

namespace cle
{
    
class ConnectedComponentLabellingBoxKernel : public Kernel
{

public:
    ConnectedComponentLabellingBoxKernel(GPU& gpu) : 
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

#endif // __cleConnectedComponentLabellingBoxKernel_h
