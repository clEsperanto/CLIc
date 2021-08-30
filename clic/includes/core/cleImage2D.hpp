#ifndef __cleImage2D_hpp
#define __cleImage2D_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

#include "cleLightObject.hpp"

namespace cle
{

class Image2D : public LightObject
{
public: 

    enum DataType { FLOAT, DOUBLE, INT, UINT, CHAR, UCHAR, SHORT, USHORT };

    Image2D();
    Image2D(cl::Image2D, DataType=FLOAT);
    Image2D(cl::Image2D, std::array<int,3>, DataType=FLOAT);
    Image2D(cl::Image2D, int[3], DataType=FLOAT);
    ~Image2D() =default;

    cl::Image2D GetObject() const;
    
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    int GetSize() const;
    int GetDimension() const;

    std::array<int,3> GetShape() const;
    std::array<int,3> GetOrigin() const;
    std::array<int,3> GetRegion() const;

    void SetOrigin(std::array<int,3>);
    void SetRegion(std::array<int,3>);

    const char* GetDataType() const;
    bool IsDataType(const char*) const;
    const char* GetObjectType() const;
    bool IsObjectType(const char*) const;

private:
    cl::Image2D m_Object;
    std::array<int,3> m_Shape {1, 1, 1};
    std::array<int,3> m_Origin {0, 0, 0};
    std::array<int,3> m_Region {1, 1, 1};
    cl::ImageFormat m_ImageFormat;

    DataType m_Type = Image2D::FLOAT;
};

}

#endif //__cleImage2D_hpp