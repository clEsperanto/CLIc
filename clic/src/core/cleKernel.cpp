
#include <stdexcept>

#include "cleKernel.hpp"

namespace cle
{

void Kernel::ManageDimensions()
{
    if(this->m_Sources.size() > 1)
    {
        for(auto it = this->m_Parameters.begin(); it != this->m_Parameters.end(); ++it)
        {
            if (it->first.find("src") != std::string::npos || it->first.find("dst") != std::string::npos)
            {
                if(it->second->nDim() == 3)
                {
                    this->m_nDimPrefix = "_3d";
                }
                else if(it->second->nDim() == 2)
                {
                    this->m_nDimPrefix = "_2d";
                }
                else
                {
                    this->m_nDimPrefix = "_1d";
                }
            }
        }
    }
}
    
const std::string Kernel::LoadPreamble() const
{
    std::string preamble = {
        #include "cle_preamble.h"
    };
    return preamble;
}

const std::string Kernel::LoadSources() const
{  
    std::string kernel_source = "Error in loading sources! " + this->m_KernelName + " sources were not found\n";
    if(!m_Sources.empty())
    {
        kernel_source = this->m_Sources.at((this->m_KernelName + this->m_nDimPrefix).c_str());
    }
    return kernel_source;
}

const std::string Kernel::BufferDefines(std::string& t_tag, std::string& t_dtype, std::string& t_dim) const
{
    std::string abbr = TypeAbbr(t_dtype.c_str());  
    std::string array_header = "\n";
    array_header += "\n#define IMAGE_" + t_tag + "_TYPE __global " + t_dtype + "*";
    array_header += "\n#define READ_" + t_tag + "_IMAGE(a,b,c) read_buffer" + t_dim + "d" + abbr + "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
    array_header += "\n#define WRITE_" + t_tag + "_IMAGE(a,b,c) write_buffer" + t_dim + "d" + abbr + "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
    return array_header;
}

const std::string Kernel::ImageDefines(std::string& t_tag, std::string& t_dtype, std::string& t_dim) const
{
    std::string img_type_name;
    if(t_tag.find("dst")!=std::string::npos || t_tag.find("destination")!=std::string::npos || 
       t_tag.find("output")!=std::string::npos)
    {
        img_type_name = "__write_only image" + t_dim + "d_t";
    }
    else
    {
        img_type_name = "__read_only image" + t_dim + "d_t";
    }
    std::string abbr = TypeAbbr(t_dtype.c_str());  
    std::string image_header = "\n";
    image_header += "\n#define IMAGE_" + t_tag + "_TYPE " + img_type_name;
    image_header += "\n#define READ_" + t_tag + "_IMAGE(a,b,c) read_image" + abbr + "(a,b,c)";
    image_header += "\n#define WRITE_" + t_tag + "_IMAGE(a,b,c) write_image" + abbr + "(a,b,c)";
    return image_header;
}

const std::string Kernel::LoadDefines() const
{
    std::string defines = "";  
    // image size defines, independent from kernel compilation
    // todo: move function to preamble for clarity
    defines = defines + "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";   // ! Not defined at runtime
    defines = defines + "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT"; // ! Not defined at runtime
    defines = defines + "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";   // ! Not defined at runtime
    defines = defines + "\n"; 

    // build defines for each non-scalar arguments 
    for (auto itr = this->m_Parameters.begin(); itr != this->m_Parameters.end(); ++itr)
    {
        if (!itr->second->IsObjectType("scalar"))  
        {     
            // get current argument information (type, size, shape, etc.)
            std::string object_tag = itr->first;                   
            std::string object_type = itr->second->GetObjectType();
            std::string data_type = itr->second->GetDataType();    
            std::string abbr_type = TypeAbbr(data_type.c_str());   
            int ndim = itr->second->nDim();

            // define position (x,y,z) information
            std::string pos_type, pos, dim;
            switch(ndim)
            {
                case 1:  dim = "1"; pos_type = "int";  pos = "(pos0)"; break;
                case 2:  dim = "2"; pos_type = "int2"; pos = "(pos0, pos1)"; break;
                case 3:  dim = "3"; pos_type = "int4"; pos = "(pos0, pos1, pos2, 0)"; break;
                default: dim = "2"; pos_type = "int2"; pos = "(pos0, pos1)"; break;
            }
                        
            // define size information
            std::string size_header = "\n";
            size_header += "\n#define IMAGE_SIZE_" + object_tag + "_WIDTH " + std::to_string(itr->second->Shape()[0]);
            size_header += "\n#define IMAGE_SIZE_" + object_tag + "_HEIGHT " + std::to_string(itr->second->Shape()[1]);
            size_header += "\n#define IMAGE_SIZE_" + object_tag + "_DEPTH " + std::to_string(itr->second->Shape()[2]);

            // define common information
            std::string common_header = "\n";
            common_header += "\n#define CONVERT_" + object_tag + "_PIXEL_TYPE clij_convert_" + data_type + "_sat";
            common_header += "\n#define IMAGE_" + object_tag + "_PIXEL_TYPE " + data_type + "";
            common_header += "\n#define POS_" + object_tag + "_TYPE " + pos_type;
            common_header += "\n#define POS_" + object_tag + "_INSTANCE(pos0,pos1,pos2,pos3) (" + pos_type + ")" + pos;
            
            // define specific information
            if (itr->second->IsObjectType("buffer"))
            {
                common_header += this->BufferDefines(object_tag, data_type, dim);
            }   
            else
            {
                common_header += this->ImageDefines(object_tag, data_type, dim);
            }
            defines += common_header + size_header + "\n";
        }
    }
    return defines;
}

std::string Kernel::TypeAbbr(const char* t_str) const
{
    size_t size = strlen(t_str);
    if (strncmp( "float", t_str, size) == 0) return "f";
    if (strncmp(  "char", t_str, size) == 0) return "c";
    if (strncmp( "uchar", t_str, size) == 0) return "uc";
    if (strncmp( "short", t_str, size) == 0) return "s";
    if (strncmp("ushort", t_str, size) == 0) return "us";
    if (strncmp(   "int", t_str, size) == 0) return "i";
    if (strncmp(  "uint", t_str, size) == 0) return "ui";
    return "?"; 
}

void Kernel::SetArguments()
{
    for(auto it = this->m_Tags.begin(); it != this->m_Tags.end(); it++ )
    {
        size_t index = it - this->m_Tags.begin();
        if(this->m_Parameters.find(it->c_str()) != this->m_Parameters.end())
        {
            std::string tag = it->c_str();
            if (this->m_Parameters.at(tag)->IsObjectType("buffer"))
            {    
                // auto object = std::dynamic_pointer_cast<cle::Buffer>(this->m_Parameters.at(tag));
                auto object = this->GetParameter<Buffer>(tag.c_str());
                try
                {
                    this->m_Kernel.setArg(index, object->Data());
                }
                catch(cl::Error& e)
                {
                    std::cerr << "Exception caught in Kernel class " << this->m_KernelName <<". Error when setting Buffer arguments to kernel." << std::endl;
                    std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;       
                }
            }
            else if (this->m_Parameters.at(tag)->IsObjectType("image"))
            {    
                // auto object = std::dynamic_pointer_cast<cle::Image>(this->m_Parameters.at(tag));
                auto object = this->GetParameter<Image>(tag.c_str());
                try
                {
                    this->m_Kernel.setArg(index, *object->Data());
                }
                catch(cl::Error& e)
                {
                    std::cerr << "Exception caught in Kernel class " << this->m_KernelName <<". Error when setting Image arguments to kernel." << std::endl;
                    std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;       
                }
            }
            else if (this->m_Parameters.at(tag)->IsObjectType("scalar"))
            {    
                if (this->m_Parameters.at(tag)->IsDataType("float")) {
                    
                    // auto object = std::dynamic_pointer_cast< cle::Scalar<float> >(this->m_Parameters.at(tag));
                    auto object = this->GetParameter<Scalar<float>>(tag.c_str());
                    this->m_Kernel.setArg(index, object->Data()); 
                }
                else if (this->m_Parameters.at(tag)->IsDataType("int")) {
                    // auto object = std::dynamic_pointer_cast< cle::Scalar<int> >(this->m_Parameters.at(tag));
                    auto object = this->GetParameter<Scalar<int>>(tag.c_str());
                    this->m_Kernel.setArg(index, object->Data()); 
                }
            }
        }
    }
}

void Kernel::AddObject(Object& t_object, const char* t_tag)
{  
    if(std::find(this->m_Tags.begin(), this->m_Tags.end(), t_tag) != this->m_Tags.end())
    {
        auto it = this->m_Parameters.find(t_tag); 
        if (it != this->m_Parameters.end())
        {
            if(t_object.IsObjectType("image"))
            {
                auto cast_object = dynamic_cast<cle::Image&>(t_object);
                it->second = std::make_shared<cle::Image>(cast_object);
            }
            else
            {
                auto cast_object = dynamic_cast<cle::Buffer&>(t_object);
                it->second = std::make_shared<cle::Buffer>(cast_object);
            }
        }
        else    
        {
            if(t_object.IsObjectType("image"))
            {
                auto cast_object = dynamic_cast<cle::Image&>(t_object);
                this->m_Parameters.insert(std::make_pair(t_tag, std::make_shared<cle::Image>(cast_object)));
            }
            else
            {
                auto cast_object = dynamic_cast<cle::Buffer&>(t_object);
                this->m_Parameters.insert(std::make_pair(t_tag, std::make_shared<cle::Buffer>(cast_object)));
            }
        }
    }
    else
    {
        throw std::runtime_error("Error in kernel " + this->m_KernelName + " execution. Invalid tag '" + t_tag + "' used to add Object.\n");      
    }
}

void Kernel::AddObject(int t_scalar, const char* t_tag)
{
    if(std::find(this->m_Tags.begin(), this->m_Tags.end(), t_tag) != this->m_Tags.end())
    {
        cle::Scalar<int> ocl_scalar(t_scalar);
        auto it = this->m_Parameters.find(t_tag); 
        if (it != this->m_Parameters.end())
        {
            it->second = std::make_shared< cle::Scalar<int> >(ocl_scalar);
        }
        else    
        {
            this->m_Parameters.insert(std::make_pair(t_tag, std::make_shared< cle::Scalar<int> >(ocl_scalar)));
        }
    }
    else
    {
        throw std::runtime_error("Error in kernel " + this->m_KernelName + " execution. Invalid tag '" + t_tag + "' used to add integer.\n");      
    }
}

void Kernel::AddObject(float t_scalar, const char* t_tag)
{
    if(std::find(this->m_Tags.begin(), this->m_Tags.end(), t_tag) != this->m_Tags.end())
    {
        cle::Scalar<float> ocl_scalar(t_scalar);
        auto it = this->m_Parameters.find(t_tag); 
        if (it != this->m_Parameters.end())
        {
            it->second = std::make_shared< cle::Scalar<float> >(ocl_scalar);
        }
        else    
        {
            this->m_Parameters.insert(std::make_pair(t_tag, std::make_shared< Scalar<float> >(ocl_scalar)));
        }
    }
    else
    {
        throw std::runtime_error("Error in kernel " + this->m_KernelName + " execution. Invalid tag '" + t_tag + "' used to add float.\n");      
    }
}

void Kernel::BuildProgramKernel()
{
    // contactenate sources
    std::string sources = LoadDefines() + LoadPreamble() + LoadSources();

    // build program and kernel from source
    std::hash<std::string> hasher;
    size_t source_hash = hasher(sources);
    if(this->m_CurrentHash != source_hash)  
    {
        // check if kernel not already in memory to save up compilation time
        if(this->m_gpu->FindProgram(source_hash))
        {
            this->m_Program = this->m_gpu->GetProgram(source_hash);
            this->m_CurrentHash = source_hash;
        }
        else
        {
            try
            {
                this->m_Program = cl::Program(this->m_gpu->Context(), sources);
            }
            catch(cl::Error& e)
            {
                std::cerr << "Exception caught in Kernel class " << this->m_KernelName << ". Error when creating Program from source." << std::endl;
                std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;   
            }
            try
            {
                this->m_Program.build({this->m_gpu->Device()});
            }
            catch(cl::Error& e)
            {
                std::cerr << "Exception caught in Kernel class " << this->m_KernelName << ". Error when building Program." << std::endl;
                std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl; 
                std::cerr << "build log:" << std::endl;
                std::cerr << this->m_Program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->m_gpu->Device()) << std::endl;
            }
            this->m_CurrentHash = source_hash;
            this->m_gpu->AddProgram(this->m_Program, m_CurrentHash);
        }
        std::string fullName = this->m_KernelName + this->m_nDimPrefix;
        try
        {
            this->m_Kernel = cl::Kernel(this->m_Program, fullName.c_str());
        }
        catch(cl::Error& e)
        {
            std::cerr << "Exception caught in Kernel class " << this->m_KernelName << ". Error when creating Kernel from program." << std::endl;
            std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl; 
        }
    }
}

bool Kernel::SetGlobalNDRange(const char* t_tag)
{
    try
    {
        auto object = this->GetParameter<Object>(t_tag);
        this->m_GlobalRange = object->Shape();
        return true; 
    }
    catch(const std::exception &err_dst)
    {
        return false;
    }
}

bool Kernel::SetGlobalNDRange(const std::array<size_t,3>& t_shape)
{
    this->m_GlobalRange = t_shape; 
    return true;
}

void Kernel::EnqueueKernel()
{
    if(std::accumulate(this->m_GlobalRange.begin(), this->m_GlobalRange.end(), 0) == 0)
    {
        if(!this->SetGlobalNDRange("dst"))
        {
            throw std::runtime_error("Error in \"Kernel::EnqueueKernel()\". Default GlobalNDRange could not be set. Provide a specific GlobalNDRange using \'Kernel::SetGlobalNDRange()\'.\n");
        }
    }
    cl::NDRange globalND(this->m_GlobalRange[0], this->m_GlobalRange[1], this->m_GlobalRange[2]);  
    try
    {
        this->m_gpu->CommandQueue().enqueueNDRangeKernel(this->m_Kernel, cl::NullRange, globalND, cl::NullRange);
    }
    catch(const cl::Error& e)
    {
        std::cerr << "Exception caught in Kernel class " << this->m_KernelName << ". Error when enqueuing NDRange kernel." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl; 
    }
    this->m_gpu->Finish();
}

Kernel::Kernel(std::shared_ptr<GPU> t_gpu, const char* t_name, const std::vector<std::string>& t_tags) : 
    m_gpu(t_gpu), m_KernelName(t_name), m_Tags(t_tags)
{}

Kernel::~Kernel()
{}

} // namespace cle
