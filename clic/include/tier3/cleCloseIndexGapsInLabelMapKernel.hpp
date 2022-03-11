
#ifndef __cleCloseIndexGapsInLabelMapKernel_hpp
#define __cleCloseIndexGapsInLabelMapKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class CloseIndexGapsInLabelMapKernel : public Kernel
{
private:
    int m_Blocksize = 0;

public:
    CloseIndexGapsInLabelMapKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetBlockSize(int);
    void Execute();
};

} // namespace cle

#endif // __cleCloseIndexGapsInLabelMapKernel_hpp
