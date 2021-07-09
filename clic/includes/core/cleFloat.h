

#ifndef __cleFloat_h
#define __cleFloat_h

#include "cleLightObject.h"

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
    size_t GetBitSize() const;


    std::string GetDataType() const;
    std::string GetObjectType() const;
    bool IsObject(LightObject::ObjectType) const;

};

} // namespace cle

#endif // __cleScalar_h
