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
    Buffer(const cl::Buffer*, const std::array<size_t,3>& ={1,1,1}, const DataType =cle::Object::FLOAT);
    ~Buffer() =default;

    const size_t Bitsize() const;

    const cl::Buffer* Data() const;

    const std::string Info() const;

private:
    std::shared_ptr<const cl::Buffer> m_Ocl =nullptr;

};

}

#endif //__cleBuffer_hpp