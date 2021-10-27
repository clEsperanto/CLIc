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
    Image(const cl::Image*, const std::array<int,3>&, const DataType =cle::Object::FLOAT);
    ~Image() =default;

    const cl::Image* Data() const;

    const std::string Info() const;

    const std::array<int,3> Origin() const;
    const std::array<int,3> Region() const;
    const cl::ImageFormat Format() const;

    const char* GetObjectType() const;
    const bool IsObjectType(const char*) const;

private:
    std::shared_ptr<const cl::Image> m_Ocl;
    cl::ImageFormat m_Format;
    std::array<int,3> m_Origin {0, 0, 0};
    std::array<int,3> m_Region {1, 1, 1};

};

}

#endif //__cleImage_hpp