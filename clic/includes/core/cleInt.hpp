

#ifndef __cleInt_hpp
#define __cleInt_hpp

#include "cleLightObject.hpp"

#include <string>
#include <fstream>

namespace cle
{

class Int : public LightObject
{
private:
    static const ObjectType O = ObjectType::cleInt;
    static const DataType T = DataType::Int;

    int m_Object;    

public:
    Int(int);

    int GetObject();
    size_t GetSize() const;


    std::string GetDataType() const;
    std::string GetObjectType() const;
    bool IsObjectType(LightObject::ObjectType) const;

};

} // namespace cle

#endif // __cleInt_hpp
