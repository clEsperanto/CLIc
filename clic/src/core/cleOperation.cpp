#include "cleOperation.hpp"
#include "cleBackend.hpp"
#include "cleMemory.hpp"
#include "cle_preamble.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>

namespace cle
{

Operation::Operation(const ProcessorPointer & device, const size_t & nb_parameter, const size_t & nb_constant)
  : device_(std::move(device))
  , name_("")
  , source_("")
{
  this->parameter_map_.reserve(nb_parameter);
  this->constant_map_.reserve(nb_constant);
}

auto
Operation::AddParameter(const std::string & tag, const float & parameter) -> void
{
  Float scalar(parameter);
  auto  success_flag = this->parameter_map_.emplace(tag, std::make_shared<Float>(scalar)).second;
  if (!success_flag)
  {
    this->parameter_map_[tag] = std::make_shared<Float>(scalar);
  }
}

auto
Operation::AddParameter(const std::string & tag, const int & parameter) -> void
{
  Int  scalar(parameter);
  auto success_flag = this->parameter_map_.emplace(tag, std::make_shared<Int>(scalar)).second;
  if (!success_flag)
  {
    this->parameter_map_[tag] = std::make_shared<Int>(scalar);
  }
}

auto
Operation::AddParameter(const std::string & tag, const Image & parameter) -> void
{
  auto success_flag = this->parameter_map_.emplace(tag, std::make_shared<Image>(parameter)).second;
  if (!success_flag)
  {
    this->parameter_map_[tag] = std::make_shared<Image>(parameter);
  }
}

auto
Operation::AddConstant(const std::string & tag, const size_t & constant) -> void
{
  auto success_flag = this->constant_map_.emplace(tag, std::to_string(constant)).second;
  if (!success_flag)
  {
    this->constant_map_[tag] = std::to_string(constant);
  }
}

auto
Operation::GetParameter(const std::string & tag) -> ObjectPointer
{
  const auto ite = this->parameter_map_.find(tag);
  if (ite == this->parameter_map_.end())
  {
    std::ostringstream error_message;
    error_message << "Error in Operation::GetParameter(): could not find parameter with tag \"" << tag
                  << "\" for operation \"" << GetName() << "\".";
    throw std::runtime_error(error_message.str());
  }
  return ite->second;
}

auto
Operation::GetImage(const std::string & tag) -> ImagePointer
{
  auto memory_ptr = this->GetParameter(tag);
  if (memory_ptr == nullptr)
  {
    return nullptr;
  }
  if (memory_ptr->GetMemoryType() == SCALAR)
  {
    return nullptr;
  }
  return std::dynamic_pointer_cast<Image>(memory_ptr);
}

auto
Operation::SetRange(const ShapeArray & range) -> void
{
  this->range_ = range;
}

auto
Operation::SetSource(const std::string & name, const std::string & src) -> void
{
  this->source_ = src;
  this->name_ = name;
}

auto
Operation::GetSource() const -> const std::string &
{
  return this->source_;
}

auto
Operation::GetName() const -> const std::string &
{
  return this->name_;
}

auto
Operation::LoadSource(const std::string & name, const std::string & file) -> void
{
  this->name_ = name;
  std::ifstream file_stream(file);
  if (!file_stream.is_open())
  {
    throw std::runtime_error("Failed to load source, could not open file: " + file);
  }
  std::ostringstream buffer;
  buffer << file_stream.rdbuf();
  this->source_ = buffer.str();
}

auto
Operation::Execute() -> void
{
  this->MakeKernel();
  this->SetKernelArguments();
  this->EnqueueOperation();
}

auto
Operation::ToString() const -> std::string
{
  std::ostringstream out_string;
  out_string << "kernel: " << this->GetName();
  if (!parameter_map_.empty())
  {
    out_string << "(";
    for (auto && param : parameter_map_)
    {
      out_string << param.first << "=" << param.second->ToString() << ",";
    }
    out_string << ")";
  }
  return out_string.str();
}

auto
operator<<(std::ostream & out, const Operation & operation) -> std::ostream &
{
  out << operation.ToString();
  return out;
}


auto
Operation::GetDevice() const -> ProcessorPointer
{
  return this->device_;
}

auto
Operation::GetKernel() const -> const cl::Kernel &
{
  return this->kernel_;
}

auto
Operation::MakePreamble() -> std::string
{
  return ""; // oclKernel::preamble;
}

auto
Operation::MakeDefines() const -> std::string
{
  std::ostringstream defines;
  defines << "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";   // ! Not defined at runtime
  defines << "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT"; // ! Not defined at runtime
  defines << "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";   // ! Not defined at runtime
  defines << "\n";
  for (auto && ite : parameter_map_)
  {
    if (ite.second->GetMemoryType() != MemoryType::SCALAR)
    {
      const auto data_type = ite.second->GetDataType();
      const auto shape = ite.second->Shape();
      // define position (x,y,z) information
      std::string pos_type;
      std::string pos;
      std::string ndim;
      switch (ite.second->Ndim())
      {
        case 1:
          ndim = "1";
          pos_type = "int";
          pos = "(pos0)";
          break;
        case 2:
          ndim = "2";
          pos_type = "int2";
          pos = "(pos0, pos1)";
          break;
        case 3:
          ndim = "3";
          pos_type = "int4";
          pos = "(pos0, pos1, pos2, 0)";
          break;
        default:
          ndim = "3";
          pos_type = "int4";
          pos = "(pos0, pos1, pos2, 0)";
          break;
      }
      // define common information
      defines << "\n";
      defines << "\n#define CONVERT_" << ite.first << "_PIXEL_TYPE clij_convert_" << DataTypeToString(data_type)
              << "_sat";
      defines << "\n#define IMAGE_" << ite.first << "_PIXEL_TYPE " << DataTypeToString(data_type) << "";
      defines << "\n#define POS_" << ite.first << "_TYPE " << pos_type;
      defines << "\n#define POS_" << ite.first << "_INSTANCE(pos0,pos1,pos2,pos3) (" << pos_type << ")" << pos;
      defines << "\n";

      // define specific information
      if (ite.second->GetMemoryType() == MemoryType::BUFFER)
      {
        defines << "\n#define IMAGE_" << ite.first << "_TYPE __global " << DataTypeToString(data_type) << "*";
        defines << "\n#define READ_" << ite.first << "_IMAGE(a,b,c) read_buffer" << ndim << "d"
                << DataTypeToString(data_type, true)
                << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
        defines << "\n#define WRITE_" << ite.first << "_IMAGE(a,b,c) write_buffer" << ndim << "d"
                << DataTypeToString(data_type, true)
                << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
      }
      else
      {
        std::string img_type_name;
        if (ite.first.find("dst") != std::string::npos || ite.first.find("destination") != std::string::npos ||
            ite.first.find("output") != std::string::npos)
        {
          img_type_name = "__write_only image" + ndim + "d_t";
        }
        else
        {
          img_type_name = "__read_only image" + ndim + "d_t";
        }
        std::string prefix;
        switch (DataTypeToString(data_type, true).front())
        {
          case 'u':
            prefix = "ui";
            break;
          case 'f':
            prefix = "f";
            break;
          default:
            prefix = "i";
            break;
        }
        defines << "\n#define IMAGE_" << ite.first << "_TYPE " << img_type_name;
        defines << "\n#define READ_" << ite.first << "_IMAGE(a,b,c) read_image" << prefix << "(a,b,c)";
        defines << "\n#define WRITE_" << ite.first << "_IMAGE(a,b,c) write_image" << prefix << "(a,b,c)";
      }
      // define size information
      defines << "\n";
      defines << "\n#define IMAGE_SIZE_" << ite.first << "_WIDTH " << std::to_string(shape[0]);
      defines << "\n#define IMAGE_SIZE_" << ite.first << "_HEIGHT " << std::to_string(shape[1]);
      defines << "\n#define IMAGE_SIZE_" << ite.first << "_DEPTH " << std::to_string(shape[2]);
      defines << "\n";
    }
  }
  // add constant definition if provided
  if (!this->constant_map_.empty())
  {
    for (auto && ite : this->constant_map_)
    {
      defines << "#define " << ite.first << " " << ite.second << "\n";
    }
    defines << "\n";
  }
  defines << "\n";
  return defines.str();
}

auto
Operation::MakeKernel() -> void
{
  std::string  program_source = this->MakeDefines() + cle::Operation::MakePreamble() + this->GetSource();
  const auto   source_hash = std::hash<std::string>{}(program_source);
  const auto & program_iter = this->GetDevice()->GetProgramMemory().find(source_hash);
  if (program_iter == this->GetDevice()->GetProgramMemory().end())
  {
    const auto program = Backend::GetProgramPointer(this->GetDevice()->ContextPtr(), program_source);
    this->GetDevice()->GetProgramMemory().emplace(source_hash, program);
    Backend::BuildProgram(program, this->GetDevice()->DevicePtr(), "-cl-kernel-arg-info");
    if (program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(this->GetDevice()->DevicePtr()) != BuildStatus::SUCCESS)
    {
      auto log = Backend::GetBuildLog(this->GetDevice()->DevicePtr(), program);
      std::cout << log << std::endl;
    };
    this->kernel_ = Backend::GetKernelPointer(program, this->GetName());
  }
  else
  {
    this->kernel_ = Backend::GetKernelPointer(program_iter->second, this->GetName());
  }
}

auto
Operation::SetKernelArguments() -> bool
{
  const auto nb_arguments = Backend::GetNumberOfKernelArguments(this->kernel_);
  for (auto idx = 0; idx < nb_arguments; idx++)
  {
    const auto & kernel_arg_tag = Backend::GetKernelArgumentName(this->kernel_, idx);
    const auto & parameter_ptr = this->parameter_map_.find(kernel_arg_tag);
    if (parameter_ptr == this->parameter_map_.end())
    {
      std::cerr << "Error: missing parameter : " << kernel_arg_tag << "\n";
      return EXIT_FAILURE;
    }
    const auto & argument = parameter_ptr->second;
    if (argument->GetMemoryType() == MemoryType::SCALAR)
    {
      switch (argument->GetDataType())
      {
        case DataType::FLOAT32: {
          auto scalar = std::dynamic_pointer_cast<Float>(argument);
          Backend::SetKernelArgument(&this->kernel_, idx, scalar->Get());
          break;
        }
        case DataType::INT32: {
          auto scalar = std::dynamic_pointer_cast<Int>(argument);
          Backend::SetKernelArgument(&this->kernel_, idx, scalar->Get());
          break;
        }
        // Add more cases for other data types if needed
        default: {
          throw std::invalid_argument("Unsupported data type for scalar argument : " + kernel_arg_tag);
        }
      }
    }
    else
    {
      auto image = std::dynamic_pointer_cast<Image>(argument);
      Backend::SetKernelArgument(&this->kernel_, idx, image->Get());
    }
  }
  return EXIT_SUCCESS;
}

auto
Operation::GetKernelArgumentsInfo() -> void
{
  auto nb_arguments = Backend::GetNumberOfKernelArguments(this->kernel_);
  for (int idx = 0; idx < nb_arguments; idx++)
  {
    auto kernel_arg_tag = Backend::GetKernelArgumentName(this->kernel_, idx);
    auto arg = this->GetParameter(kernel_arg_tag);
    std::cout << kernel_arg_tag << " : " << arg->ToString() << std::endl;
  }
}

auto
Operation::GetArgumentsInfo() -> void
{
  for (auto && i : this->parameter_map_)
  {
    std::cout << i.first << " : " << i.second->ToString() << std::endl;
  }
}

auto
Operation::EnqueueOperation() -> void
{
  if (std::none_of(this->range_.begin(), this->range_.end(), [](size_t dim_range) { return dim_range != 0; }))
  {
    this->SetRange(this->GetImage("dst")->Shape());
  }
  Backend::EnqueueKernelExecution(this->GetDevice()->QueuePtr(), this->kernel_, this->range_);
  this->GetDevice()->Finish();
}


} // namespace cle
