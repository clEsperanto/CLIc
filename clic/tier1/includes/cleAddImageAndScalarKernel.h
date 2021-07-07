
#ifndef __cleAddImageAndScalarKernel_h
#define __cleAddImageAndScalarKernel_h

#include "cleKernel.h"

namespace cle
{

class AddImageAndScalarKernel : public Kernel
{
private:
    std::string source_2d = 
        #include "cle_add_image_and_scalar_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_add_image_and_scalar_3d.h"
        ;

public:
    AddImageAndScalarKernel(GPU gpu) : 
        Kernel( gpu, 
                "add_image_and_scalar", 
                {"src", "dst", "scalar"}
            ) 
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();

};

} // namespace cle

#endif // __cleAddImageAndScalarKernel_h
