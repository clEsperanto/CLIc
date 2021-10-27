#include "cleImage.hpp"

namespace cle
{

Image::Image(const cl::Image* t_ocl_image, const std::array<int,3>& t_shape, const DataType t_dtype) : 
    Object(t_shape, t_dtype), m_Ocl(std::shared_ptr<const cl::Image>(t_ocl_image))
{
    this->m_Format.image_channel_order = t_ocl_image->getImageInfo<CL_IMAGE_FORMAT>().image_channel_order;          
    this->m_Format.image_channel_data_type = t_ocl_image->getImageInfo<CL_IMAGE_FORMAT>().image_channel_data_type;  
}

const cl::Image* Image::Data() const
{
    return const_cast<cl::Image*>(this->m_Ocl.get()); 
}

const std::string Image::Info() const
{
    std::string out = "";
    out += std::to_string(this->nDim()) + "d " + std::string(this->GetObjectType()) + "("+ this->GetDataType() +")";
    out += " [" + std::to_string(this->Shape()[0]) + "," + std::to_string(this->Shape()[1]) + "," + std::to_string(this->Shape()[2]) + "] (" + std::to_string(this->Size()) + " elements)";
    return out;
}

const std::array<int,3> Image::Origin() const
{
    return this->m_Origin;
}

const std::array<int,3> Image::Region() const
{
    return this->m_Region;
}

const cl::ImageFormat Image::Format() const
{
    return this->m_Format;
}

const char* Image::GetObjectType() const
{
    return "image";
}

const bool Image::IsObjectType(const char* t_str) const
{
    return strncmp("image", t_str, strlen(t_str)) == 0;
}

}