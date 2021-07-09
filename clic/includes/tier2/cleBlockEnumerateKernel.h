
#ifndef __cleBlockEnumerateKernel_h
#define __cleBlockEnumerateKernel_h

#include "cleKernel.h"

namespace cle
{
    
class BlockEnumerateKernel : public Kernel
{
private:
    std::string source = 
        #include "cle_block_enumerate.h" 
        ;

public:
    BlockEnumerateKernel(GPU& gpu) : 
        Kernel( gpu,
                "block_enumerate",
                {"dst", "src", "src_sums", "blocksize"}
        )
    {
        m_Sources.insert({this->m_KernelName, source});
    }

    void SetInput(Buffer&);
    void SetInputSums(Buffer&);
    void SetOutput(Buffer&);
    void SetBlocksize(int);
    void Execute();

};

} // namespace cle

#endif // __cleBlockEnumerateKernel_h
