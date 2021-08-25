
#ifndef __cleSetNonzeroPixelsToPixelindexKernel_hpp
#define __cleSetNonzeroPixelsToPixelindexKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SetNonzeroPixelsToPixelindexKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_set_nonzero_pixels_to_pixelindex.h" 
        ;

public:
    SetNonzeroPixelsToPixelindexKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetOffset(int);
    void Execute();
};

} // namespace cle

#endif // __cleSetNonzeroPixelsToPixelindexKernel_hpp
