#ifndef __cleBuffer_hpp
#define __cleBuffer_hpp

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
 * Buffer class which hold dimension, type, shape, and pointer of an OpenCL Buffer pointer 
 *
 * The class is a data class which maintain image parameters (shape, type, etc.) along with a 
 * cl::Buffer. The cl::Buffer being a memory address of a buffer in the GPU device.
 */
class Buffer : public Object
{
public: 
    /**
     * Default constructor.
     */
    Buffer() =default;

    /**
     * constructor.
     * @param t_buffer a cl::Buffer object.
     * @param t_shape the shape of the buffer as an array of size 3, default {1,1,1}.
     * @param t_dtype the data type of the buffer, default FLOAT.
     */
    Buffer(const cl::Buffer& t_buffer, const std::array<size_t,3>& t_shape ={1,1,1}, const DataType t_dtype =cle::Object::FLOAT);

    /**
     * Default constructor.
     */
    ~Buffer() =default;

    /**
     * Return the memory size in bits occupy by the buffer.
     * This is defined by the number of elements times the 
     * bitsize of the datatype used.
     * @return the total bitsize of the buffer.
     */
    const size_t Bitsize() const;

    /**
     * Return the data address hold by the Buffer.
     * @return the cl::Buffer.
     */
    const cl::Buffer Data() const;

    /**
     * Print information on the Buffer.
     * @return All usefull information as a formated string.
     */
    const std::string Info() const;

private:
    /// buffer object pointing to data in GPU
    cl::Buffer m_Ocl;
};

} // namespace cle

#endif //__cleBuffer_hpp
