#ifndef __cleBuffer_hpp
#define __cleBuffer_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

#include "cleLightObject.hpp"

namespace cle
{

class Buffer : public LightObject
{
public: 

    enum DataType { FLOAT, DOUBLE, INT, UINT, CHAR, UCHAR, SHORT, USHORT };

    Buffer() =default;
    Buffer(cl::Buffer, DataType=FLOAT);
    Buffer(cl::Buffer, std::array<int,3>, DataType=FLOAT);
    Buffer(cl::Buffer, int[3], DataType=FLOAT);
    ~Buffer() =default;

    cl::Buffer GetObject() const;
    
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    int GetDimension() const;
    int GetSize() const;
    std::array<int,3> GetShape() const;

    const char* GetDataType() const;
    bool IsDataType(const char*) const;
    const char* GetObjectType() const;
    bool IsObjectType(const char*) const;

private:
    cl::Buffer m_Object;
    std::array<int,3> m_Shape = {1, 1, 1};
    int m_Dimension = 1;
    DataType m_Type = Buffer::FLOAT;
};

}

#endif //__cleBuffer_hpp