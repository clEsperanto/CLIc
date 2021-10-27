#ifndef __cleBuffer_hpp
#define __cleBuffer_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

#include "cleObject.hpp"

namespace cle
{

class Buffer : public Object
{
public: 

    Buffer() =default;
    Buffer(const cl::Buffer*, const std::array<int,3>&, const DataType =cle::Object::FLOAT);
    ~Buffer() =default;

    const cl::Buffer* Data() const;

    const std::string Info() const;

    const char* GetObjectType() const;
    const bool IsObjectType(const char*) const;

private:
    std::shared_ptr<const cl::Buffer> m_Ocl;

};

}

#endif //__cleBuffer_hpp