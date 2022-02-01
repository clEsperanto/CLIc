#include "cleBuffer.hpp"

namespace cle
{

Buffer::Buffer(const cl::Buffer& t_buffer, const std::array<size_t,3>& t_shape, const DataType t_dtype) : 
    Object(t_shape, t_dtype, "buffer"), m_Ocl(t_buffer)
{}

const cl::Buffer Buffer::Data() const 
{ 
    return this->m_Ocl; 
}

const size_t Buffer::Bitsize() const
{
    return this->m_Ocl.getInfo<CL_MEM_SIZE>();   
}

const std::string Buffer::Info() const
{
    std::string out = std::to_string(this->nDim()) + "d " + std::string(this->GetObjectType()) + "("+ this->GetDataType() +")";
    out += " [" + std::to_string(this->Shape()[0]) + "," + std::to_string(this->Shape()[1]) + "," + std::to_string(this->Shape()[2]) + "] (" + std::to_string(this->Size()) + " elements)";
    return out;
}

}// namespace cle
