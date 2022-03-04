
#ifndef __cleBinaryAndKernel_hpp
#define __cleBinaryAndKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class BinaryAndKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_binary_and.h" 
        };

public:
    BinaryAndKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleBinaryAndKernel_hpp
