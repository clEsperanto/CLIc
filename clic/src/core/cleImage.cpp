#include "cleImage.hpp"

namespace cle
{

Image::Image(const cl::Image* t_image, const std::array<size_t,3>& t_shape, const DataType t_dtype) : 
    Object(t_shape, t_dtype, "image"), m_Ocl(std::shared_ptr<const cl::Image>(t_image))
{}

const cl::Image* Image::Data() const
{
    return const_cast<cl::Image*>(this->m_Ocl.get()); 
}

const size_t Image::Bitsize() const
{
    return this->m_Ocl->getInfo<CL_MEM_SIZE>();   
}

const std::string Image::Info() const
{
    std::string out = "";
    out += std::to_string(this->nDim()) + "d " + std::string(this->GetObjectType()) + "("+ this->GetDataType() +")";
    out += " [" + std::to_string(this->Shape()[0]) + "," + std::to_string(this->Shape()[1]) + "," + std::to_string(this->Shape()[2]) + "] (" + std::to_string(this->Size()) + " elems)";
    return out;
}

}