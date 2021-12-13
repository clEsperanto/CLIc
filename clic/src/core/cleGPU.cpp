

#include "cleGPU.hpp"
#include <iostream>

namespace cle
{

GPU::GPU() 
{
    std::vector<cl::Platform> m_PlatformList = this->ListPlatforms();
    this->m_Platform = m_PlatformList.front();
    std::vector<cl::Device> m_DeviceList = ListDevices(this->m_Platform, "all");
    this->m_Device = m_DeviceList.front();
    this->AllocateDevice();
}

GPU::GPU(const char* t_device_name, const char* t_device_type) 
{
    this->SelectDevice(t_device_name, t_device_type);
}

GPU::~GPU() 
{
    if (!this->m_ProgramList.empty())
    {
        this->m_ProgramList.clear();
    }
}

const std::vector<cl::Platform> GPU::ListPlatforms() const
{
    std::vector<cl::Platform> m_PlatformList;
    try
    {
        cl::Platform::get(&m_PlatformList);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Error : Fail to get platforms ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
    return m_PlatformList;
}

const std::vector<cl::Device> GPU::ListDevices(const cl::Platform& t_platform, const char* t_device_type) const
{
    std::vector<cl::Device> m_DeviceList;
    try
    {
        if (strncmp("gpu", t_device_type, strlen(t_device_type)) == 0)
            t_platform.getDevices(CL_DEVICE_TYPE_GPU, &m_DeviceList);
        else if (strncmp("cpu", t_device_type, strlen(t_device_type)) == 0)
            t_platform.getDevices(CL_DEVICE_TYPE_CPU, &m_DeviceList);
        else
            t_platform.getDevices(CL_DEVICE_TYPE_ALL, &m_DeviceList);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Error : Fail to get devices from platform ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
    return m_DeviceList;
}

void GPU::AllocateDevice()
{
    try 
    {
        this->m_Context = cl::Context(this->m_Device);
    }
    catch(const cl::Error& e)
    {
        std::cerr << "Error : Error creating context on provided device." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
    }
    try
    {
        this->m_CommandQueue = cl::CommandQueue(this->m_Context, this->m_Device);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Error : Fail to create command queue from context and device." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

void GPU::SelectDevice(const char* t_device_name, const char* t_device_type)
{
    bool found (false), deviceCheck(false);
    // Loop on all platforms
    std::vector<cl::Platform> m_PlatformList = this->ListPlatforms();
    auto platform_ite = m_PlatformList.begin();
    while(!found && platform_ite != m_PlatformList.end() )
    {
        // Loop on all devices
        std::vector<cl::Device> m_DeviceList = ListDevices(*platform_ite, t_device_type);
        auto device_ite = m_DeviceList.begin();
        while(!found && device_ite != m_DeviceList.end() )
        {
            // Only check available devices
            if(device_ite->getInfo<CL_DEVICE_AVAILABLE>())
            {
                deviceCheck = device_ite->getInfo<CL_DEVICE_NAME>().find(t_device_name) != std::string::npos;
                if(deviceCheck)
                {
                    // we found a {platform|device} with corresponding keyword
                    found = true;
                    this->m_Device = *device_ite;
                    this->m_Platform = *platform_ite;
                }
            }
            device_ite++;
        }
        platform_ite++;
    }
    if (!found)
    {
        // We take the first {platform|device}.
        this->m_Platform = m_PlatformList.front();
        this->m_Device = ListDevices(this->m_Platform, "all").front();
    }
    this->AllocateDevice();
}

cl::Device GPU::Device() const
{
    return this->m_Device;
}

cl::Context GPU::Context() const
{
    return this->m_Context;
}

cl::CommandQueue GPU::CommandQueue() const
{
    return this->m_CommandQueue;
}

cl::Platform GPU::Platform() const
{
    return this->m_Platform;
}

const cl::Program GPU::GetProgram(const size_t t_hash)
{
    auto it = m_ProgramList.find(t_hash);
    return it->second;
}

const bool GPU::FindProgram(const size_t t_hash) const
{
    auto it = m_ProgramList.find(t_hash);
    return it != m_ProgramList.end();
}

void GPU::AddProgram(const cl::Program& t_program, const size_t t_hash)
{
    m_ProgramList.insert({t_hash, t_program});
}

const std::string GPU::Name() const
{
    return this->m_Device.getInfo<CL_DEVICE_NAME>();
}

const std::string GPU::Info() const
{
    std::string out = "";
    out += "[" + this->m_Platform.getInfo<CL_PLATFORM_NAME>() + " - " + this->Name() + "]\n";
    out += "\tDevicedeviceType: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_TYPE>()) + "\n";
    out += "\tMaxComputeUnits: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>()) + "\n";
    out += "\tMaxClockFrequency: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>()) + "\n";
    out += "\tVersion: " + this->m_Device.getInfo<CL_DEVICE_VERSION>() + "\n";
    out += "\tExtensions: " + this->m_Device.getInfo<CL_DEVICE_EXTENSIONS>() + "\n";
    out += "\tGlobalMemorySizeInBytes: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>()) + "\n";
    out += "\tLocalMemorySizeInBytes: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>()) + "\n";
    out += "\tMaxMemoryAllocationSizeInBytes: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()) + "\n";
    out += "\tMaxWorkGroupSize: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>()) + "\n";
    return out;
}

const float GPU::Score() const
{
    float score = 0;
    if(this->m_Device.getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU)
        score += 4e12;
    else
        score += 2e12;
    score += this->m_Device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    return score;
}


const cl::Buffer* GPU::CreateBuffer(size_t t_bitsize) const
{
    return new cl::Buffer(this->Context(), CL_MEM_READ_WRITE, t_bitsize);
}

void GPU::WriteBuffer(const cl::Buffer* t_ocl_object, void * t_arr) const
{
    size_t bitsize = t_ocl_object->getInfo<CL_MEM_SIZE>();
    try
    {
        this->CommandQueue().enqueueWriteBuffer(*t_ocl_object, CL_TRUE, 0, bitsize, t_arr);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Error caught : Fail to write Buffer ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

void GPU::ReadBuffer(const cl::Buffer* t_ocl_object, void * t_arr) const
{
    size_t bitsize = t_ocl_object->getInfo<CL_MEM_SIZE>();
    try
    {
        this->CommandQueue().enqueueReadBuffer(*t_ocl_object, CL_TRUE, 0, bitsize, t_arr);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Error caught : Fail to read Buffer ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

const cl::Image* GPU::CreateImage(const std::array<size_t,3>& t_shape, const cl::ImageFormat& t_format) const
{
    if (t_shape[2] > 1)
    {
        return new cl::Image3D(this->Context(), CL_MEM_READ_WRITE, t_format, t_shape[0], t_shape[1], t_shape[2]);
    }
    else if (t_shape[1] > 1)
    {
        return new cl::Image2D(this->Context(), CL_MEM_READ_WRITE, t_format, t_shape[0], t_shape[1]);
    }
    else
    {
        return new cl::Image1D(this->Context(), CL_MEM_READ_WRITE, t_format, t_shape[0]);
    }
}

void GPU::WriteImage(const cl::Image* t_ocl_object, void* t_arr) const
{
    size_t row_pitch = t_ocl_object->getImageInfo<CL_IMAGE_ROW_PITCH>();
    size_t slice_pitch = t_ocl_object->getImageInfo<CL_IMAGE_SLICE_PITCH>();
    size_t width = t_ocl_object->getImageInfo<CL_IMAGE_WIDTH>();
    size_t height = t_ocl_object->getImageInfo<CL_IMAGE_HEIGHT>();
    size_t depth = t_ocl_object->getImageInfo<CL_IMAGE_DEPTH>(); 

    if(height == 0) height += 1;
    if(depth == 0) depth += 1;
    std::array<size_t,3> origin {0,0,0};
    std::array<size_t,3> region {width, height, depth};

    try
    {   
        this->CommandQueue().enqueueWriteImage(*t_ocl_object, CL_TRUE, origin, region, row_pitch, slice_pitch, t_arr);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Error caught : Fail to write Image ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

void GPU::ReadImage(const cl::Image* t_ocl_object, void* t_arr) const
{
    size_t row_pitch = t_ocl_object->getImageInfo<CL_IMAGE_ROW_PITCH>();
    size_t slice_pitch = t_ocl_object->getImageInfo<CL_IMAGE_SLICE_PITCH>();
    size_t width = t_ocl_object->getImageInfo<CL_IMAGE_WIDTH>();
    size_t height = t_ocl_object->getImageInfo<CL_IMAGE_HEIGHT>();
    size_t depth = t_ocl_object->getImageInfo<CL_IMAGE_DEPTH>();

    DEBUG(row_pitch << " " << slice_pitch << " " << width << " " << height << " " << depth << "\n" );

    if(height == 0) height = 1;
    if(depth == 0) depth = 1;
    std::array<size_t,3> origin {0,0,0};
    std::array<size_t,3> region {width, height, depth};

    try
    {
        this->CommandQueue().enqueueReadImage(*t_ocl_object, CL_TRUE, origin, region, row_pitch, slice_pitch, t_arr);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Error caught : Fail to read Image ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

// void GPU::CopyBufferToBuffer(cle::Buffer& t_src, cle::Buffer& t_dst) const
// {
//     try
//     {
//         this->CommandQueue().enqueueCopyBuffer(*t_src.Data(), *t_dst.Data(), 0, 0, t_dst.Bitsize(), nullptr, nullptr);
//     }
//     catch(cl::Error& e)
//     {
//         std::cerr << "Error caught : Fail to copy in Buffer to Buffer ..." << std::endl;
//         std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
//     }
// }

// void GPU::CopyBufferToImage(cle::Buffer& t_src, cle::Image& t_dst) const
// {
//     try
//     {
//         this->CommandQueue().enqueueCopyBufferToImage(*t_src.Data(), *t_dst.Data(), 0, t_dst.Origin(), t_dst.Region(), nullptr, nullptr);        
//     }
//     catch(cl::Error& e)
//     {
//         std::cerr << "Error caught : Fail to copy in Buffer to Image ..." << std::endl;
//         std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
//     }
// }

// void GPU::CopyImageToBuffer(cle::Image& t_src, cle::Buffer& t_dst) const
// {
//     try
//     {
//         this->CommandQueue().enqueueCopyImageToBuffer(*t_src.Data(), *t_dst.Data(), t_src.Origin(), t_src.Region(), 0, nullptr, nullptr);    
//     }
//     catch(cl::Error& e)
//     {
//         std::cerr << "Error caught : Fail to copy in Image to Buffer ..." << std::endl;
//         std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
//     }
// }

// void GPU::CopyImageToImage(cle::Image& t_src, cle::Image& t_dst) const
// {
//     try
//     {
//         this->CommandQueue().enqueueCopyImage(*t_src.Data(), *t_dst.Data(), t_src.Origin(), t_dst.Origin(), t_dst.Region(), nullptr, nullptr);
        
//     }
//     catch(cl::Error& e)
//     {
//         std::cerr << "Error caught : Fail to copy in Image to Image ..." << std::endl;
//         std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
//     }
// }




void GPU::SetWaitForKernelToFinish(bool t_flag)
{
    this->m_WaitForFinish = t_flag;
}

void GPU::Finish() const
{
    if(m_WaitForFinish)
        this->m_CommandQueue.finish();
}

void GPU::Flush() const
{
    this->m_CommandQueue.flush();
}


} // namespace cle
