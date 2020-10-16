
#include "cleObject.h"

namespace cle
{

std::string Object::DataTypeToString(const DataType t) const
{
    switch (t)
    {
        case Float:  return "float";
        case Char:   return "char";
        case UChar:  return "uchar";
        case Int:    return "int";
        case UInt:   return "uint";
        case Short:  return "short";
        case UShort: return "ushort";
        default:     return "unknown";
    }
}

Object::DataType Object::StringToDataType(const std::string t) const
{
    if(t == "float")       return Object::DataType::Float;
    else if(t == "char")   return Object::DataType::Char;
    else if(t == "uchar")  return Object::DataType::UChar;
    else if(t == "int")    return Object::DataType::Int;
    else if(t == "uint")   return Object::DataType::UInt;
    else if(t == "short")  return Object::DataType::Short;
    else if(t == "ushort") return Object::DataType::UShort;
    else                   return Object::DataType::Float;
}

std::string Object::ObjectTypeToString(const ObjectType o) const
{
    switch (o)
    {
        case Scalar:  return "Scalar";
        case Buffer:  return "Buffer";
        case Image2d: return "Image2d";
        default:      return "Unknown";
    }
}

std::ostream& operator<<(std::ostream& os, const Object& p)
{
    return os << p.ToString();
}

} // namespace cle
