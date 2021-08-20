
#ifndef __cleCustomKernel_hpp
#define __cleCustomKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class CustomKernel : public Kernel
{

public:
    CustomKernel (std::shared_ptr<GPU> gpu, const char * kernelName, std::vector<std::string> tagList) :    
        Kernel( gpu, 
                kernelName, // kernel name
                tagList     // parameter tags
        )
    {}

    void SetSource(const char * );
    void SetParameter(Buffer& , const char* );
    void SetParameter(float , const char* );
    void SetParameter(int , const char* );
    void Execute();         
};

} // namespace cle

#endif // __cleCustomKernel_hpp