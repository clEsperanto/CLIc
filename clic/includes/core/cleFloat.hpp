

#ifndef __cleFloat_hpp
#define __cleFloat_hpp

#include "cleLightObject.hpp"

#include <string>
#include <fstream>

namespace cle
{

class Float : public LightObject
{
private:
    static const ObjectType O = ObjectType::cleFloat;
    static const DataType T = DataType::Float;

    float m_Object;    

public:
    Float(float);

    float GetObject();
    size_t GetSize() const;


    std::string GetDataType() const;
    std::string GetObjectType() const;
    bool IsObjectType(LightObject::ObjectType) const;

};

} // namespace cle

#endif // __cleScalar_hpp
