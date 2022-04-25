

#ifndef __cleGPU_hpp
#define __cleGPU_hpp

#include "clic.hpp"
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
     * @brief List all available Platforms.
     * 
     * @return list of Platforms. Empty if none are found.
     */
    const std::vector<cl::Platform> FetchPlatforms() const;

    /**
     * @brief List all available Devices.
     * 
     * @param t_platform Platform you want to search the devices from.
     * @param t_device_type Device type (gpu, cpu, all).
     * @return list of Devices. Empty if none are found.
     */
    const std::vector<cl::Device> FetchDevices(const cl::Platform& t_platform, const char* t_device_type) const;

    /**
     * @brief Allocate device by creating Context and CommandQueue.
     * 
     * @throws std::exception::runtime_error throw if could not instantiate context.
     * @throws std::exception::runtime_error throw if could not instantiate command queue.
     */    
    void AllocateDevice();

    /**
     * @brief convert template type to data type.
     * 
     * @return data type (FLOAT, INT, UINT, CHAR, UCHAR, SHORT, USHORT)
     */ 
    template<class Type>
    const cle::Object::DataType Template2DataType() const;

    // todo: low-level copy / cast methods buffer <-> image
    // todo: void CopyBufferToBuffer(cle::Buffer&, cle::Buffer&) const;
    // todo: void CopyBufferToImage(cle::Buffer&, cle::Image&) const;
    // todo: void CopyImageToBuffer(cle::Image&, cle::Buffer&) const;
    // todo: void CopyImageToImage(cle::Image&, cle::Image&) const;

    /**
     * @brief Allocate memory on device
     * 
     * @param t_buffer buffer address
     * @param t_bitsize memory size
     * @param t_ptr pointer to host memory (optional)
     * @throws std::exception::runtime_error throw if fail creating buffer
     */
    void AllocateMemory(cl::Buffer& t_buffer, const size_t t_bitsize, void* t_ptr =nullptr) const;

    /**
     * @brief Allocate memory on device
     * 
     * @param t_image 3d image address
     * @param t_shape image shape (x,y,z) 
     * @param t_format image format 
     * @param t_ptr pointer to host memory (optional)
     * @throws std::exception::runtime_error throw if fail creating image
     */
    void AllocateMemory(cl::Image3D& t_image, const std::array<size_t,3> t_shape, const cl::ImageFormat& t_format, void* t_ptr =nullptr) const;

    /**
     * @brief Allocate memory on device
     * 
     * @param t_image 2d image address
     * @param t_shape image shape (x,y,z) 
     * @param t_format image format 
     * @param t_ptr pointer to host memory (optional)
     * @throws std::exception::runtime_error throw if fail creating image
     */
    void AllocateMemory(cl::Image2D& t_image, const std::array<size_t,3> t_shape, const cl::ImageFormat& t_format, void* t_ptr =nullptr) const;

    /**
     * @brief Allocate memory on device
     * 
     * @param t_image 1d image address
     * @param t_shape image shape (x,y,z) 
     * @param t_format image format 
     * @param t_ptr pointer to host memory (optional)
     * @throws std::exception::runtime_error throw if fail creating image
     */
    void AllocateMemory(cl::Image1D& t_image, const std::array<size_t,3> t_shape, const cl::ImageFormat& t_format, void* t_ptr =nullptr) const;

    /**
     * @brief Write memory on device
     * 
     * @param t_buffer buffer address to write in
     * @param t_data data to write from host to device
     */
    void WriteMemory(const cl::Buffer& t_buffer, const void* t_data) const;

    /**
     * @brief Write memory on device
     * 
     * @param t_image image address to write in
     * @param t_data data to write from host to device
     */
    void WriteMemory(const cl::Image& t_image, const void* t_data) const;

    /**
     * @brief Read memory from device
     * 
     * @param t_buffer buffer address to read from
     * @param t_data data to write from device to host
     */
    void ReadMemory(const cl::Buffer& t_buffer, void* t_data) const;

    /**
     * @brief Read memory from device
     * 
     * @param t_buffer image address to read from
     * @param t_data data to write from device to host
     */
    void ReadMemory(const cl::Image& t_image, void* t_data) const;

public:

    /**
     * @brief Default constructor. Will allocate the first available device.
     * 
     * @throws std::exception::runtime_error throw if no platform detected.
     * @throws std::exception::runtime_error throw if no device detected.
     * @throws std::exception::runtime_error throw if could not allocate ressources on device.
     */
    GPU();

    /**
     * @brief Constructor. Will allocate device based on name and type.
     * 
     * @param t_device_name name of device to allocate.
     * @param t_device_type type of device to allocate (default all).
     * @throws std::exception::runtime_error Fail in selecting device to allocate.
     */
    GPU(const char* t_device_name, const char* t_device_type ="all");

    /**
     * @brief Default destructor.
     */
    ~GPU();

    /**
     * @brief Will allocate device based on name and type.
     * 
     * @param t_device_name name of device to allocate.
     * @param t_device_type type of device to allocate (default all).
     * @throws std::exception::runtime_error throw if no platform detected.
     * @throws std::exception::runtime_error throw if no device detected.
     * @throws std::exception::runtime_error throw if could not allocate ressources on device.
     */    
    const std::string SelectDevice(const char* t_device_name, const char* t_device_type ="all");

    /**
     * @brief List all visible devices.
     * 
     * @param t_device_type type of device to allocate (default all).
     * @throws std::exception::runtime_error throw if no platform detected.
     * @throws std::exception::runtime_error throw if no device detected.
     */    
    const std::vector<std::string> ListAvailableDevices(const char* t_device_type ="all") const;

    /**
     * @brief Get information on current device.
     * 
     * @return formated string of information on current device
     */
    const std::string Info() const;

    /**
     * @brief Get name on current device.
     * 
     * @return name of current device
     */
    const std::string Name() const;

    /**
     * @brief Compute indicative score based on memory and device type.
     * 
     * @return score value
     */
    const float Score() const;

    /**
     * @brief Get device method.
     * 
     * @return Device
     */
    cl::Device Device() const;

    /**
     * @brief Get context method.
     * 
     * @return Context
     */
    cl::Context Context() const;

    /**
     * @brief Get Commande Queue method.
     * 
     * @return CommandQueue
     */
    cl::CommandQueue CommandQueue() const;

    /**
     * @brief Get platform method.
     * 
     * @return Platform
     */
    cl::Platform Platform() const;

    /**
     * @brief Find program in program list.
     * 
     * @param t_hash hash key generated from program sources.
     * @return true if found, false otherwise.
     */    
    const bool FindProgram(const size_t t_hash) const;

    /**
     * @brief Get program in program list.
     * 
     * @param t_hash hash key generated from program sources.
     * @return Program corresponding to hash key.
     * @throws std::exception::runtime_error throw if could not find program in program list.
     */
    const cl::Program GetProgram(const size_t t_hash);

    /**
     * @brief Add program to list.
     * 
     * @param t_program compiled program.
     * @param t_hash hash key generated from program sources.
     * @return true if program was added with success, false otherwise.
     */
    bool AddProgram(const cl::Program& t_program, const size_t t_hash);

    /**
     * @brief Force GPU to wait for kernel to execute before going on.
     * 
     * @param t_flag switch true or false.
     */
    void SetWaitForKernelToFinish(bool);

    /**
     * @brief Call CommandQueue to finish.
     */
    void Finish() const;

    /**
     * @brief Call CommandQueue to flush.
     */
    void Flush() const;

    /**
     * @brief Create empty object in device.
     * 
     * @tparam Type data pixel type 
     * @param t_shape shape of object to allocate (default {1,1,1}).
     * @param t_type object type, buffer or image (default buffer).
     * @return empty object.
     */ 
    template<class Type>
    const Object Create(const std::array<size_t,3>& t_shape ={1,1,1}, const std::string t_type ="buffer") const;

    /**
     * @brief Create object in device and write data from host to device.
     * 
     * @tparam Type data pixel type 
     * @param t_data data to write into Image.
     * @param t_shape shape of Image space to allocate (default {1,1,1}).
     * @param t_type object type, buffer or image (default buffer).
     * @return Image object.
     */ 
    template<class Type>
    const Object Push(const std::vector<Type>& t_data ={0}, const std::array<size_t,3>& t_shape ={1,1,1}, const std::string t_type ="buffer") const;

    /**
     * @brief Read object from Device to host.
     * 
     * @tparam Type data pixel type 
     * @param t_object object in device.
     * @return array of data.
     */ 
    template<class Type>
    const std::vector<Type> Pull(const Object& t_object) const;

    /**
     * @brief Define image format based on data type
     * 
     * @tparam Type data pixel type 
     */
    template<class Type>
    const cl::ImageFormat SetImageFormat() const;
};

template<class Type>
const cle::Object::DataType GPU::Template2DataType() const
{
    if(std::is_same<Type, float>::value) return cle::Object::FLOAT;
    if(std::is_same<Type, int>::value) return cle::Object::INT;
    if(std::is_same<Type, unsigned int>::value) return cle::Object::UINT;
    if(std::is_same<Type, char>::value) return cle::Object::CHAR;
    if(std::is_same<Type, unsigned char>::value) return cle::Object::UCHAR;
    if(std::is_same<Type, double>::value) return cle::Object::DOUBLE;
    if(std::is_same<Type, short>::value) return cle::Object::SHORT;
    if(std::is_same<Type, unsigned short>::value) return cle::Object::USHORT;
    return cle::Object::FLOAT;
}


template<class Type>
const cl::ImageFormat GPU::SetImageFormat() const
{
    // if(std::is_same<Type, float>::value)               return cl::ImageFormat (CL_INTENSITY, CL_FLOAT);
    // else if(std::is_same<Type, int>::value)            return cl::ImageFormat (CL_INTENSITY, CL_SIGNED_INT32);
    // else if(std::is_same<Type, unsigned int>::value)   return cl::ImageFormat (CL_INTENSITY, CL_UNSIGNED_INT32);
    // else if(std::is_same<Type, char>::value)           return cl::ImageFormat (CL_INTENSITY, CL_SIGNED_INT8);
    // else if(std::is_same<Type, unsigned char>::value)  return cl::ImageFormat (CL_INTENSITY, CL_UNSIGNED_INT8);
    // else if(std::is_same<Type, short>::value)          return cl::ImageFormat (CL_INTENSITY, CL_SIGNED_INT16);
    // else if(std::is_same<Type, unsigned short>::value) return cl::ImageFormat (CL_INTENSITY, CL_UNSIGNED_INT16);
    // else return cl::ImageFormat (CL_INTENSITY, CL_FLOAT);
    return cl::ImageFormat (CL_INTENSITY, CL_FLOAT);
}

template<class Type>
const Object GPU::Create(const std::array<size_t,3>& t_shape, const std::string t_type) const
{
    cl::Memory mem;
    if(t_type.find("buffer") != std::string::npos) 
    {
        cl::Buffer buffer;
        AllocateMemory(buffer, t_shape[0]*t_shape[1]*t_shape[2]*sizeof(Type));
        mem = cl::Memory(buffer.get(), true);
    }
    else if(t_type.find("image") != std::string::npos) 
    {
        const cl::ImageFormat image_format = this->SetImageFormat<Type>();
        if(t_shape[2]>1)
        {
            cl::Image3D image;
            AllocateMemory(image, t_shape, image_format);
            mem = cl::Memory(image.get(), true);
        }
        else if(t_shape[1]>1)
        {
            cl::Image2D image;
            AllocateMemory(image, t_shape, image_format);
            mem = cl::Memory(image.get(), true);
        }
        else
        {
            cl::Image1D image;
            AllocateMemory(image, t_shape, image_format);
            mem = cl::Memory(image.get(), true);
        }
    }
    else
    {
        throw std::runtime_error("");
    }
    return Object(mem, t_shape, this->Template2DataType<Type>());
}

template<class Type>
const Object GPU::Push(const std::vector<Type>& t_data, const std::array<size_t,3>& t_shape, const std::string t_type) const
{
    cl::Memory mem;
    if(t_type.find("buffer") != std::string::npos) 
    {
        cl::Buffer buffer;
        AllocateMemory(buffer, t_shape[0]*t_shape[1]*t_shape[2]*sizeof(Type), const_cast<Type*>(t_data.data()));
        WriteMemory(buffer, t_data.data());
        mem = cl::Memory(buffer.get(), true);
    }
    else if(t_type.find("image") != std::string::npos) 
    {
        const cl::ImageFormat image_format = this->SetImageFormat<Type>();
        if(t_shape[2]>1)
        {
            cl::Image3D image;
            AllocateMemory(image, t_shape, image_format, const_cast<Type*>(t_data.data()));
            WriteMemory(image, t_data.data());
            mem = cl::Memory(image.get(), true);
        }
        else if(t_shape[1]>1)
        {
            cl::Image2D image;
            AllocateMemory(image, t_shape, image_format, const_cast<Type*>(t_data.data()));
            WriteMemory(image, t_data.data());
            mem = cl::Memory(image.get(), true);
        }
        else
        {
            cl::Image1D image;
            AllocateMemory(image, t_shape, image_format, const_cast<Type*>(t_data.data()));
            WriteMemory(image, t_data.data());
            mem = cl::Memory(image.get(), true);
        }
    }
    else
    {
        throw std::runtime_error("");
    }
    return Object(mem, t_shape, this->Template2DataType<Type>());
}

template<class Type>
const std::vector<Type> GPU::Pull(const Object& t_object) const
{
    std::vector<Type> out (t_object.Size());
    switch(t_object.MemoryType())
    {
        case CL_MEM_OBJECT_BUFFER:
            {
            cl::Buffer buffer(t_object.Data().get(), true);
            ReadMemory(buffer, out.data());
            break;
            }
        case CL_MEM_OBJECT_IMAGE3D: 
            {
            cl::Image3D image(t_object.Data().get(), true);
            ReadMemory(image, out.data());
            break;
            }
        case CL_MEM_OBJECT_IMAGE2D: 
            {
            cl::Image2D image(t_object.Data().get(), true);
            ReadMemory(image, out.data());
            break;
            }
        case CL_MEM_OBJECT_IMAGE1D: 
            {
            cl::Image1D image(t_object.Data().get(), true);
            ReadMemory(image, out.data());
            break;
            }
        default:
            throw std::runtime_error("");
    }
    return out;
}

} // namespace cle

#endif //__cleGPU_hpp
