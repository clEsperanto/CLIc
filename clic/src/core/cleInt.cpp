


#include "cleInt.hpp"

#include <string>

namespace cle
{

Int::Int(int obj)
{
    m_Object = obj;
}

int Int::GetObject()
{
    return m_Object;
}

size_t Int::GetSize() const
{
    return 1;
}

std::string Int::GetObjectType() const
{
    return this->ObjectTypeToString(O);
}

bool Int::IsObjectType(LightObject::ObjectType str) const
{
    return this->O == str;
}



} // namespace cle

