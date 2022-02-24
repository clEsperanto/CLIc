
#ifndef __cleBinarySubtractKernel_hpp
#define __cleBinarySubtractKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class BinarySubtractKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_binary_subtract.h" 
        };

public:
    BinarySubtractKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleBinarySubtractKernel_hpp
