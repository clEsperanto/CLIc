
#ifndef __cleSeparableKernel_hpp
#define __cleSeparableKernel_hpp

#include "cleKernel.hpp"

namespace cle
{

class SeparableKernel : public Kernel
{
    
public:
    SeparableKernel(GPU& gpu) : 
        Kernel( gpu, 
                "",
                {"dst" , "src", "dim", "N", "s"}
        ){}

    void SetSources(std::map<std::string, std::string>);
    void SetKernelName(std::string);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSize(int);
    void SetSigma(float);
    void SetDimension(int);
    void Execute();
};

} // namespace cle

#endif // __cleSeparableKernel_hpp
