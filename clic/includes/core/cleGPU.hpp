

#ifndef __cleGPU_hpp
#define __cleGPU_hpp

#include "clic.hpp"

#include "cleBuffer.hpp"
#include "cleImage.hpp"
#include "cleObject.hpp"


#include <map>
#include <iostream>

namespace cle
{

/**
 * Management of GPU ressources
 *
 * GPU are physically defined by a Platform (Nvidia, AMD, Intel, etc.) and
 * a Device (GeForce RTX 3080, Radeon RX 6000, etc.). A Context is create in which 
 * a GPU will operate and a CommandQueue of operation is associated to the Context 
 * and Device.
 * The low-level operation of memory allocation, reading and writing are also managed
 * here, along with a memory list of built program.
 */
class GPU
{
private:
    /// GPU ressources IDs
    cl::Platform m_Platform;
    cl::Device m_Device;
    cl::Context m_Context;
    cl::CommandQueue m_CommandQueue;
    
    /// Built program list for reusability
    std::map<size_t, cl::Program> m_ProgramList;

    /// flag for enforce GPU to finish CommandQueue execution
    bool m_WaitForFinish = false;

protected:
    /**
     * List all available Platforms.
     * @return list of Platforms. Empty if none are found.
     * @exceptsafe No-throw.
     */
    const std::vector<cl::Platform> ListPlatforms() const;

    /**
     * List all available Devices.
     * @param t_platform Platform you want to search the devices from.
     * @param t_device_type Device type (gpu, cpu, all).
     * @return list of Devices. Empty if none are found.
     * @exceptsafe No-throw.
     */
    const std::vector<cl::Device> ListDevices(const cl::Platform& t_platform, const char* t_device_type) const;

    /**
     * Allocate device by creating Context and CommandQueue.
     * @throws std::exception::runtime_error throw if could not instantiate context.
     * @throws std::exception::runtime_error throw if could not instantiate command queue.
     * @exceptsafe Basic.
     */    
    void AllocateDevice();

    /**
     * low-level create buffer space on device.
     * @param t_bitsize memory bitsize to allocate.
     * @return ocl buffer pointer.
     * @throws std::exception::runtime_error throw if could not create buffer.
     * @exceptsafe strong.
     */ 
    const cl::Buffer CreateBuffer(const size_t t_bitsize) const;
    /**
     * low-level write ocl buffer from host to device.
     * @param t_buffer ocl buffer pointer write on device.
     * @param t_data data pointer to read from host.
     * @exceptsafe No-throw.
     */ 
    void WriteBuffer(const cl::Buffer& t_buffer, void* t_data) const;
    /**
     * low-level read ocl buffer from device to host.
     * @param t_buffer ocl buffer pointer to read from device.
     * @param t_data data pointer to write on host.
     */ 
    void ReadBuffer(const cl::Buffer& t_buffer, void* t_data) const;
       
    /**
     * low-level create image space on device.
     * @param t_shape image shape to allocate.
     * @param t_format image format (number channels, data type)
     * @return ocl image pointer.
     * @throws std::exception::runtime_error throw if could not create image1d.
     * @throws std::exception::runtime_error throw if could not create image2d.
     * @throws std::exception::runtime_error throw if could not create image3d.
     * @exceptsafe strong.
     */ 
    const cl::Image* CreateImage(const std::array<size_t,3>& t_shape, const cl::ImageFormat& t_format) const;

    /**
     * low-level write ocl image from host to device.
     * @param t_image ocl image pointer write on device.
     * @param t_data data pointer to read from host.
     * @exceptsafe No-throw.
     */  
    void WriteImage(const cl::Image* t_image, void* t_data) const;

    /**
     * low-level read ocl image from device to host.
     * @param t_image ocl image pointer to read from device.
     * @param t_data data pointer to write on host.
     * @exceptsafe No-throw.
     */ 
    void ReadImage(const cl::Image* t_image, void* t_data) const;


    /**
     * convert template type to data type.
     * @return data type (FLOAT, INT, UINT, CHAR, UCHAR, SHORT, USHORT)
     * @exceptsafe No-throw.
     */ 
    template<class T>
    const cle::Object::DataType Template2DataType() const;

    // todo: low-level copy / cast methods buffer <-> image
    // todo: void CopyBufferToBuffer(cle::Buffer&, cle::Buffer&) const;
    // todo: void CopyBufferToImage(cle::Buffer&, cle::Image&) const;
    // todo: void CopyImageToBuffer(cle::Image&, cle::Buffer&) const;
    // todo: void CopyImageToImage(cle::Image&, cle::Image&) const;

public:

    /**
     * Default constructor. Will allocate the first available device.
     * @throws std::exception::runtime_error throw if no platform detected.
     * @throws std::exception::runtime_error throw if no device detected.
     * @throws std::exception::runtime_error throw if could not allocate ressources on device.
     * @exceptsafe Basic.
     */
    GPU();

    /**
     * Constructor. Will allocate device based on name and type.
     * @param t_device_name name of device to allocate.
     * @param t_device_type type of device to allocate (default all).
     * @throws std::exception::runtime_error Fail in selecting device to allocate.
     * @exceptsafe Basic.
     */
    GPU(const char* t_device_name, const char* t_device_type ="all");

    /**
     * Default destructor.
     * @exceptsafe No-throw.
     */
    ~GPU();

    /**
     * Will allocate device based on name and type.
     * @param t_device_name name of device to allocate.
     * @param t_device_type type of device to allocate (default all).
     * @throws std::exception::runtime_error throw if no platform detected.
     * @throws std::exception::runtime_error throw if no device detected.
     * @throws std::exception::runtime_error throw if could not allocate ressources on device.
     * @exceptsafe Basic.
     */    
    void SelectDevice(const char* t_device_name, const char* t_device_type ="all");

    /**
     * Get information on current device.
     * @return formated string of information on current device
     * @exceptsafe No-throw.
     */
    const std::string Info() const;

    /**
     * Get name on current device.
     * @return name of current device
     * @exceptsafe No-throw.
     */
    const std::string Name() const;

    /**
     * Compute indicative score based on memory and device type.
     * @return score value
     * @exceptsafe No-throw.
     */
    const float Score() const;

    /**
     * Get device method.
     * @return Device
     * @exceptsafe No-throw.
     */
    cl::Device Device() const;

    /**
     * Get context method.
     * @return Context
     * @exceptsafe No-throw.
     */
    cl::Context Context() const;

    /**
     * Get Commande Queue method.
     * @return CommandQueue
     * @exceptsafe No-throw.
     */
    cl::CommandQueue CommandQueue() const;

    /**
     * Get platform method.
     * @return Platform
     * @exceptsafe No-throw.
     */
    cl::Platform Platform() const;

    /**
     * Find program in program list.
     * @param t_hash hash key generated from program sources.
     * @return true if found, false otherwise.
     * @exceptsafe No-throw.
     */    
    const bool FindProgram(const size_t t_hash) const;

    /**
     * Get program in program list.
     * @param t_hash hash key generated from program sources.
     * @return Program corresponding to hash key.
     * @throws std::exception::runtime_error throw if could not find program in program list.
     * @exceptsafe No-throw.
     */
    const cl::Program GetProgram(const size_t t_hash);

    /**
     * Add program to list.
     * @param t_program compiled program.
     * @param t_hash hash key generated from program sources.
     * @return true if program was added with success, false otherwise.
     * @exceptsafe No-throw.
     */
    bool AddProgram(const cl::Program& t_program, const size_t t_hash);

    /**
     * Force GPU to wait for kernel to execute before going on.
     * @param t_flag switch true or false.
     * @exceptsafe No-throw.
     */
    void SetWaitForKernelToFinish(bool);

    /**
     * Call CommandQueue to finish.
     * @exceptsafe No-throw.
     */
    void Finish() const;

    /**
     * Call CommandQueue to flush.
     * @exceptsafe No-throw.
     */
    void Flush() const;

    /**
     * Create empty Buffer space in device.
     * @param t_shape shape of Buffer space to allocate (default {1,1,1}).
     * @return Buffer object.
     * @exceptsafe No-throw.
     */    
    template<class T = float>
    cle::Buffer CreateBuffer(const std::array<size_t,3>& t_shape ={1,1,1}) const;

    /**
     * Create empty Image space in device.
     * @param t_shape shape of Image space to allocate (default {1,1,1}).
     * @return Image object.
     * @exceptsafe No-throw.
     */ 
    template<class T = float>
    cle::Image CreateImage(const std::array<size_t,3>& t_shape ={1,1,1}) const;

    /**
     * Create and write Buffer space from host to device.
     * @param t_data data to write into Buffer.
     * @param t_shape shape of Buffer space to allocate (default {1,1,1}).
     * @return Buffer object.
     * @exceptsafe No-throw.
     */ 
    template<class T = float>
    cle::Buffer PushBuffer(std::vector<T>& t_data, const std::array<size_t,3>& t_shape ={1,1,1}) const;

    /**
     * Create and write Image space from host to device.
     * @param t_data data to write into Image.
     * @param t_shape shape of Image space to allocate (default {1,1,1}).
     * @return Image object.
     * @exceptsafe No-throw.
     */ 
    template<class T = float>
    cle::Image PushImage(std::vector<T>& t_data, const std::array<size_t,3>& t_shape ={1,1,1}) const;

    /**
     * Read Buffer from Device to host.
     * @param t_buffer Buffer to read.
     * @return array of data.
     * @exceptsafe No-throw.
     */ 
    template<class T = float>
    std::vector<T> Pull(const cle::Buffer& t_buffer) const;

    /**
     * Read Image from Device to host.
     * @param t_image Image to read.
     * @return array of data.
     * @exceptsafe No-throw.
     */ 
    template<class T = float>
    std::vector<T> Pull(const cle::Image& t_image) const;
};

template<class T>
const cle::Object::DataType GPU::Template2DataType() const
{
    if(std::is_same<T, float>::value) return cle::Object::FLOAT;
    if(std::is_same<T, int>::value) return cle::Object::INT;
    if(std::is_same<T, unsigned int>::value) return cle::Object::UINT;
    if(std::is_same<T, char>::value) return cle::Object::CHAR;
    if(std::is_same<T, unsigned char>::value) return cle::Object::UCHAR;
    if(std::is_same<T, double>::value) return cle::Object::DOUBLE;
    if(std::is_same<T, short>::value) return cle::Object::SHORT;
    if(std::is_same<T, unsigned short>::value) return cle::Object::USHORT;
    return cle::Object::FLOAT;
}

template<class T>
cle::Buffer GPU::CreateBuffer(const std::array<size_t,3>& t_shape) const
{
    auto buffer = this->CreateBuffer(t_shape[0] * t_shape[1] * t_shape[2] * sizeof(T));
    return cle::Buffer(buffer, t_shape, this->Template2DataType<T>());
}

template<class T>
cle::Buffer GPU::PushBuffer(std::vector<T>& t_data, const std::array<size_t,3>& t_shape) const
{
    auto buffer = this->CreateBuffer(t_data.size() * sizeof(T));
    this->WriteBuffer(buffer, t_data.data());
    return cle::Buffer(buffer, t_shape, this->Template2DataType<T>());
}

template<class T>
std::vector<T> GPU::Pull(const cle::Buffer& t_buffer) const
{
    std::vector<T> array (t_buffer.Size());
    this->ReadBuffer(t_buffer.Data(), array.data());
    return array;
}

template<class T>
cle::Image GPU::CreateImage(const std::array<size_t,3>& t_shape) const
{
    auto image = this->CreateImage(t_shape, cl::ImageFormat(CL_INTENSITY, CL_FLOAT));
    return cle::Image(image, t_shape, this->Template2DataType<T>());
}

template<class T>
cle::Image GPU::PushImage(std::vector<T>& t_data, const std::array<size_t,3>& t_shape) const
{
    auto image = this->CreateImage(t_shape, cl::ImageFormat(CL_INTENSITY, CL_FLOAT));
    this->WriteImage(image, t_data.data());
    return cle::Image(image, t_shape, this->Template2DataType<T>());
}

template<class T>
std::vector<T> GPU::Pull(const cle::Image& t_image) const
{
    std::vector<T> array (t_image.Size());
    this->ReadImage(t_image.Data(), array.data());
    return array;
}

} // namespace cle

#endif //__cleGPU_hpp
