
#ifndef __cleMeanSphereKernel_h
#define __cleMeanSphereKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MeanSphereKernel : public Kernel
{
private:
    int Radius2KernelSize(float);  // kernel specific methods

public:
    MeanSphereKernel(GPU& gpu) : 
        Kernel( gpu,
                "mean_sphere",
                {"dst", "src", "radius_x", "radius_y", "radius_z"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetRadiusX(int);
    void SetRadiusY(int);
    void SetRadiusZ(int);
    void Execute();
};

} // namespace cle

#endif // __cleMeanSphereKernel_h
