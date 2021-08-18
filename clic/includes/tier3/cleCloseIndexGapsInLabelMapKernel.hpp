
#ifndef __cleCloseIndexGapsInLabelMapKernel_hpp
#define __cleCloseIndexGapsInLabelMapKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class CloseIndexGapsInLabelMapKernel : public Kernel
{
private:

    int blocksize = 0;

public:

    CloseIndexGapsInLabelMapKernel(GPU& gpu) : 
        Kernel( gpu,
                "close_index_gaps_in_label_map",
                {"src", "dst", "blocksize"}
        )
    {}
    void SetInput(Buffer&);
    void SetOutput(Buffer&);

    void SetBlockSize(int);
    void Execute();

};

} // namespace cle

#endif // __cleCloseIndexGapsInLabelMapKernel_hpp
