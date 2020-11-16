#ifndef __cleLightObject_h
#define __cleLightObject_h

#include <string>
#include <fstream>

namespace cle
{
    

class LightObject
{

public:
    enum ObjectType {Buffer, Image2d, Scalar}; 
    enum DataType {Float, Char, UChar, Int, UInt, Short, UShort};

protected:
    std::string ObjectTypeToString(const ObjectType) const;
    std::string DataTypeToString(const DataType) const;
    DataType StringToDataType(const std::string) const;
    
    friend std::ostream & operator<<(std::ostream &, const LightObject&);

public:

    LightObject(){};
    ~LightObject(){};
    virtual std::string ToString() const = 0;

};



} // namespace cle

#endif // __cleLightObject_h