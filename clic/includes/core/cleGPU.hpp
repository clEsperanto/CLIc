

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
    
class GPU
{
private:
    cl::Platform m_Platform;
    cl::Device m_Device;
    cl::Context m_Context;
    cl::CommandQueue m_CommandQueue;
    std::map<size_t, cl::Program> m_ProgramList;

    bool m_WaitForFinish = false;

protected:
    const std::vector<cl::Platform> ListPlatforms() const;
    const std::vector<cl::Device> ListDevices(const cl::Platform&, const char*) const;
    void AllocateDevice();

    const cl::Buffer* CreateBuffer(const size_t) const;
    void WriteBuffer(const cl::Buffer*, void*) const;
    void ReadBuffer(const cl::Buffer*, void*) const;
    
    const cl::Image* CreateImage(const std::array<size_t,3>&, const cl::ImageFormat&) const;
    void WriteImage(const cl::Image*, void*) const;
    void ReadImage(const cl::Image*, void*) const;

    template<class T>
    const cle::Object::DataType Template2DataType() const;

    // todo: enable copy / cast methods
    // void CopyBufferToBuffer(cle::Buffer&, cle::Buffer&) const;
    // void CopyBufferToImage(cle::Buffer&, cle::Image&) const;
    // void CopyImageToBuffer(cle::Image&, cle::Buffer&) const;
    // void CopyImageToImage(cle::Image&, cle::Image&) const;

public:
    GPU();
    GPU(const char*, const char* ="all");
    ~GPU();

    void SelectDevice(const char*, const char* ="all");
    const std::string Info() const;
    const std::string Name() const;
    const float Score() const;

    cl::Device Device() const;
    cl::Context Context() const;
    cl::CommandQueue CommandQueue() const;
    cl::Platform Platform() const;

    const bool FindProgram(const size_t) const;
    const cl::Program GetProgram(const size_t);
    void AddProgram(const cl::Program&, const size_t);

    void WaitForKernelToFinish();
    void Finish() const;
    void Flush() const;

    // Create / Push / Pull / Copy methods
    template<class T = float>
    cle::Buffer CreateBuffer(const std::array<size_t,3>& ={1,1,1}) const;
    template<class T = float>
    cle::Image CreateImage(const std::array<size_t,3>& ={1,1,1}) const;
    template<class T = float>
    cle::Buffer PushBuffer(std::vector<T>&, const std::array<size_t,3>& ={1,1,1}) const;
    template<class T = float>
    cle::Image PushImage(std::vector<T>&, const std::array<size_t,3>& ={1,1,1}) const;
    template<class T = float>
    std::vector<T> Pull(const cle::Buffer&) const;
    template<class T = float>
    std::vector<T> Pull(const cle::Image&) const;
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
cle::Buffer GPU::PushBuffer(std::vector<T>& t_arr, const std::array<size_t,3>& t_shape) const
{
    auto buffer = this->CreateBuffer(t_arr.size() * sizeof(T));
    this->WriteBuffer(buffer, t_arr.data());
    return cle::Buffer(buffer, t_shape, this->Template2DataType<T>());
}

template<class T>
std::vector<T> GPU::Pull(const cle::Buffer& t_buffer) const
{
    std::vector<T> arr (t_buffer.Size());
    this->ReadBuffer(t_buffer.Data(), arr.data());
    return arr;
}

template<class T>
cle::Image GPU::CreateImage(const std::array<size_t,3>& t_shape) const
{
    auto image = this->CreateImage(t_shape, cl::ImageFormat(CL_INTENSITY, CL_FLOAT));
    return cle::Image(image, t_shape, this->Template2DataType<T>());
}

template<class T>
cle::Image GPU::PushImage(std::vector<T>& t_arr, const std::array<size_t,3>& t_shape) const
{
    auto image = this->CreateImage(t_shape, cl::ImageFormat(CL_INTENSITY, CL_FLOAT));
    this->WriteImage(image, t_arr.data());
    return cle::Image(image, t_shape, this->Template2DataType<T>());
}

template<class T>
std::vector<T> GPU::Pull(const cle::Image& t_image) const
{
    std::vector<T> arr (t_image.Size());
    this->ReadImage(t_image.Data(), arr.data());
    return arr;
}

} // namespace cle

#endif //__clgpu_hpp
