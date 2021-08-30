
#ifndef __cleAddImageAndScalarKernel_hpp
#define __cleAddImageAndScalarKernel_hpp

#include "cleKernel.hpp"

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
    AddImageAndScalarKernel(std::shared_ptr<GPU>);

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetInput(Image2D&);
    void SetOutput(Image2D&);
    void SetScalar(float);
    void Execute();

};

} // namespace cle

#endif // __cleAddImageAndScalarKernel_hpp
