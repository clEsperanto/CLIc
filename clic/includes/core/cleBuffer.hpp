

#ifndef __cleBuffer_hpp
#define __cleBuffer_hpp

#include "cleLightObject.hpp"

#include <array>

namespace cle
{
    
class Buffer : public LightObject
{

private:
    static const ObjectType O = ObjectType::cleBuffer;
    DataType T;

    cl::Buffer m_Object;
    std::array<unsigned int, 3> m_Dimensions = {{1, 1, 1}};
    
public:     
    Buffer() =default;
    Buffer(cl::Buffer, unsigned int[3], DataType =Float);
    Buffer(cl::Buffer, DataType =Float);

    cl::Buffer GetObject();

    size_t GetSize() const;
    unsigned int* GetDimensions();

    bool IsObjectType(LightObject::ObjectType) const;
    bool IsDataType(LightObject::DataType) const;
    std::string GetObjectType() const;
    std::string GetDataType() const;
};

} // namespace cle

#endif // __cleBuffer_hpp
