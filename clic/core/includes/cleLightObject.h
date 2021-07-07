#ifndef __cleLightObject_h
#define __cleLightObject_h

#include "clic.h"

#include <string>
#include <fstream>

namespace cle
{
    
class LightObject
{   

public:
    enum ObjectType {cleBuffer, cleFloat, cleInt}; 
    enum DataType {Float, Char, UChar, Int, UInt, Short, UShort};

    LightObject() = default;
    ~LightObject() = default;

    virtual std::string GetObjectType() const = 0;
    virtual bool IsObject(ObjectType) const = 0;
    virtual size_t GetBitSize() const = 0;

protected:    
    std::string ObjectTypeToString(const ObjectType) const;
    std::string DataTypeToString(const DataType) const;
    DataType StringToDataType(const std::string) const;

};

} // namespace cle

#endif // __cleLightObject_h