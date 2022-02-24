
#ifndef __cleBinaryNotKernel_hpp
#define __cleBinaryNotKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class BinaryNotKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_binary_not.h" 
        };

public:
    BinaryNotKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleBinaryNotKernel_hpp
