
#ifndef __cleBlockEnumerateKernel_h
#define __cleBlockEnumerateKernel_h

#include "cleKernel.h"

namespace cle
{
    
class BlockEnumerateKernel : public Kernel
{

public:
    BlockEnumerateKernel(GPU& gpu) : 
        Kernel( gpu,
                "block_enumerate",
                {"dst", "src", "src_sums", "blocksize"}
        )
    {}

    void SetInput(Buffer&);
    void SetInputSums(Buffer&);
    void SetOutput(Buffer&);
    void SetBlocksize(int);
    void Execute();

};

} // namespace cle

#endif // __cleBlockEnumerateKernel_h
