
#include "cleLightObject.h"

namespace cle
{

std::string LightObject::ObjectTypeToString(const ObjectType o) const
{
    switch (o)
    {
        case Buffer:  return "Buffer";
        case Image2d: return "Image2d";
        case Scalar:  return "Scalar";
        default:      return "Unknown";
    }
}

std::string LightObject::DataTypeToString(const DataType t) const
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

LightObject::DataType LightObject::StringToDataType(const std::string t) const
{
    if(t == "float")       return LightObject::DataType::Float;
    else if(t == "char")   return LightObject::DataType::Char;
    else if(t == "uchar")  return LightObject::DataType::UChar;
    else if(t == "int")    return LightObject::DataType::Int;
    else if(t == "uint")   return LightObject::DataType::UInt;
    else if(t == "short")  return LightObject::DataType::Short;
    else if(t == "ushort") return LightObject::DataType::UShort;
    else                   return LightObject::DataType::Float;
}

std::ostream& operator<<(std::ostream& os, const LightObject& p)
{
    return os << p.ToString();
}

} // namespace cle
