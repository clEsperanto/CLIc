#ifndef __CORE_CLEOPERATION_HPP
#define __CORE_CLEOPERATION_HPP

#include "cleImage.hpp"
#include "cleProcessor.hpp"
#include "cleScalar.hpp"

#include <unordered_map>

namespace cle
{

class Operation
{
public:
  using Int = Scalar<int>;
  using Float = Scalar<float>;
  using ParameterList = std::unordered_map<std::string, std::shared_ptr<LightObject>>;
  using ConstantList = std::unordered_map<std::string, std::string>;
  using ObjectPointer = std::shared_ptr<LightObject>;
  using ImagePointer = std::shared_ptr<Image>;
  using ProcessorPointer = std::shared_ptr<Processor>;
  using ShapeArray = std::array<size_t, 3>;

  Operation() = delete;
  virtual ~Operation() = default;
  Operation(const Operation & operation) = default;
  Operation(Operation && operation) = default;
  auto
  operator=(const Operation & operation) -> Operation & = default;
  auto
  operator=(Operation && operation) -> Operation & = default;
  explicit Operation(const ProcessorPointer & device, const size_t & nb_parameter = 0, const size_t & nb_constant = 0);

  auto
  AddParameter(const std::string & tag, const float & parameter) -> void;
  auto
  AddParameter(const std::string & tag, const int & parameter) -> void;
  auto
  AddParameter(const std::string & tag, const Image & parameter) -> void;
  auto
  AddConstant(const std::string & tag, const size_t & constant) -> void;
  auto
  GetParameter(const std::string & tag) -> ObjectPointer;
  auto
  GetImage(const std::string & tag) -> ImagePointer;
  auto
  SetRange(const ShapeArray & range) -> void;
  auto
  SetRange(const std::string & tag) -> void;
  auto
  SetSource(const std::string & name, const std::string & src) -> void;
  [[nodiscard]] auto
  GetSource() const -> const std::string &;
  [[nodiscard]] auto
  GetName() const -> const std::string &;
  auto
  LoadSource(const std::string & name, const std::string & file) -> void;
  virtual auto
  Execute() -> void;
  [[nodiscard]] auto
  ToString() const -> std::string;
  friend auto
  operator<<(std::ostream & out, const Operation & operation) -> std::ostream &;


protected:
  [[nodiscard]] auto
  GetDevice() const -> ProcessorPointer;
  [[nodiscard]] auto
  GetKernel() const -> const cl::Kernel &;
  [[nodiscard]] auto
  MakeDefines() const -> std::string;
  static auto
  MakePreamble() -> std::string;
  auto
  MakeKernel() -> void;
  auto
  SetKernelArguments() -> bool;
  auto
  GetKernelArgumentsInfo() -> void;
  auto
  GetArgumentsInfo() -> void;
  auto
  EnqueueOperation() -> void;
  auto
  SetNumberOfParameters(const size_t & nb_parameter) -> void;
  auto
  SetNumberOfConstants(const size_t & nb_constant) -> void;
  auto
  GenerateOutput(const std::string & input_tag = "src", const std::string & output_tag = "dst") -> void;
  static auto
  GenerateOutput(const Image & object, const ShapeArray & shape) -> Image;

private:
  cl::Kernel       kernel_;
  std::string      name_;
  std::string      source_;
  ShapeArray       range_ = { 0, 0, 0 };
  ProcessorPointer device_;
  ParameterList    parameter_map_{};
  ConstantList     constant_map_{};
};

} // namespace cle

#endif // __CORE_CLEOPERATION_HPP
