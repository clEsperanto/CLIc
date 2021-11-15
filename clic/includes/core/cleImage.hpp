#ifndef __cleImage_hpp
#define __cleImage_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

#include "cleObject.hpp"

namespace cle
{

class Image : public Object
{
public: 
    Image() =default;
    Image(const cl::Image*, const std::array<size_t,3>& ={1,1,1}, const DataType =cle::Object::FLOAT);
    ~Image() =default;

    const size_t Bitsize() const;
    const cl::Image* Data() const;
    const std::string Info() const;

private:
    std::shared_ptr<const cl::Image> m_Ocl =nullptr;
};

}

#endif //__cleImage_hpp