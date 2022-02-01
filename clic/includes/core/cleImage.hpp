#ifndef __cleImage_hpp
#define __cleImage_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

#include "cleObject.hpp"

namespace cle
{

/**
 * Image class which hold dimension, type, shape, and pointer of an OpenCL Image pointer 
 *
 * The class is a data class which maintain image parameters (shape, type, etc.) along with a 
 * cl::Image pointer. The cl::Image being a memory address of an image in the GPU device.
 * Here a cl::Image pointer is used to allow polymorphism between the different opencl image type.
 */
class Image : public Object
{
public:
    /**
     * Default constructor.
     */
    Image() =default;

    /**
     * constructor.
     * @param t_image a cl::Image pointer.
     * @param t_shape the shape of the image as an array of size 3, default {1,1,1}.
     * @param t_dtype the data type of the image, default FLOAT.
     */
    Image(const cl::Image* t_image, const std::array<size_t,3>& t_shape ={1,1,1}, const DataType t_dtype =cle::Object::FLOAT);

    /**
     * Default constructor.
     */
    ~Image() =default;

    /**
     * Return the memory size in bits occupy by the image.
     * This is defined by the number of elements times the 
     * bitsize of the datatype used.
     * @return the total bitsize of the image.
     */
    const size_t Bitsize() const;

    /**
     * Return the data address hold by the Image.
     * @return the cl::Image as a pointer.
     */
    const cl::Image* Data() const;

    /**
     * Print information on the Image.
     * @return information as a formated string.
     */
    const std::string Info() const;

private:
    /// image object pointing to data in GPU
    std::shared_ptr<const cl::Image> m_Ocl =nullptr;
};

} // namespace cle

#endif //__cleImage_hpp