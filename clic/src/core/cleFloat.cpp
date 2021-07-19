


#include "cleFloat.h"

#include <string>

namespace cle
{

Float::Float(float obj)
{
    m_Object = obj;
}

float Float::GetObject()
{
    return m_Object;
}

size_t Float::GetSize() const
{
    return 1;
}

std::string Float::GetObjectType() const
{
    return this->ObjectTypeToString(O);
}

bool Float::IsObjectType(LightObject::ObjectType str) const
{
    return this->O == str;
}

} // namespace cle

