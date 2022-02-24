
#ifndef __cleBinaryXorKernel_hpp
#define __cleBinaryXorKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class BinaryXorKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_binary_xor.h" 
        };

public:
    BinaryXorKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleBinaryXorKernel_hpp
