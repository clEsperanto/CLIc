
#ifndef __cleBlockEnumerateKernel_hpp
#define __cleBlockEnumerateKernel_hpp

#include "cleKernel.hpp"

namespace cle
{

// todo: make kernel work with Image
    
class BlockEnumerateKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_block_enumerate.h" 
        };

public:
    BlockEnumerateKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetInputSums(Object&);
    void SetOutput(Object&);
    void SetBlocksize(int);
    void Execute();
};

} // namespace cle

#endif // __cleBlockEnumerateKernel_hpp
