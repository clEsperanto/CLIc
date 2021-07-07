
#ifndef __cleMeanSphereKernel_h
#define __cleMeanSphereKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MeanSphereKernel : public Kernel
{
private:
    std::string source_2d = 
        #include "cle_mean_sphere_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_mean_sphere_3d.h" 
        ;

public:
    MeanSphereKernel(GPU& gpu) : 
        Kernel( gpu,
                "mean_sphere",
                {"dst", "src", "radius_x", "radius_y", "radius_z"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetRadiusX(int);
    void SetRadiusY(int);
    void SetRadiusZ(int);
    void Execute();

private:
    int Radius2KernelSize(float);
};

} // namespace cle

#endif // __cleMeanSphereKernel_h
