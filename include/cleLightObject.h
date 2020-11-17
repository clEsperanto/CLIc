#ifndef __cleLightObject_h
#define __cleLightObject_h

#include <string>
#include <fstream>

namespace cle
{
    

class LightObject
{

public:
    enum ObjectType {cleBuffer, cleFloat, cleInt}; 
    enum DataType {Float, Char, UChar, Int, UInt, Short, UShort};

protected:
    std::string ObjectTypeToString(const ObjectType) const;
    std::string DataTypeToString(const DataType) const;
    DataType StringToDataType(const std::string) const;
    
    friend std::ostream & operator<<(std::ostream &, const LightObject&);

public:

    LightObject(){};
    virtual ~LightObject() = default;
    virtual std::string ToString() const = 0;
    virtual std::string GetObjectType() const = 0;
    virtual bool IsObject(std::string) const = 0;

};



} // namespace cle

#endif // __cleLightObject_h