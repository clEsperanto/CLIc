#include "cleBuffer.hpp"

namespace cle
{

Buffer::Buffer(const cl::Buffer* t_ocl_buffer, const std::array<int,3>& t_shape, const DataType t_dtype) : 
    Object(t_shape, t_dtype), m_Ocl(std::shared_ptr<const cl::Buffer>(t_ocl_buffer))
{};

const cl::Buffer* Buffer::Data() const 
{ 
    return const_cast<cl::Buffer*>(this->m_Ocl.get()); 
}

const std::string Buffer::Info() const
{
    std::string out = std::to_string(this->nDim()) + "d " + std::string(this->GetObjectType()) + "("+ this->GetDataType() +")";
    out += " [" + std::to_string(this->Shape()[0]) + "," + std::to_string(this->Shape()[1]) + "," + std::to_string(this->Shape()[2]) + "] (" + std::to_string(this->Size()) + " elements)";
    return out;
}

const char* Buffer::GetObjectType() const 
{ 
    return "buffer"; 
}   

const bool Buffer::IsObjectType(const char* t_str) const
{
    return strncmp("buffer", t_str, strlen(t_str)) == 0;
}

}