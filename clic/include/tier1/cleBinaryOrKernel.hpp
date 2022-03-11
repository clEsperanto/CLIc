
#ifndef __cleBinaryOrKernel_hpp
#define __cleBinaryOrKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class BinaryOrKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_binary_or.h" 
        };

public:
    BinaryOrKernel(std::shared_ptr<GPU>);
    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleBinaryOrKernel_hpp
