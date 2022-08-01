#ifndef CLIC_INCLUDE_CORE_CLEOPERATION_HPP
#define CLIC_INCLUDE_CORE_CLEOPERATION_HPP

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
    using ParameterList = std::unordered_map<std::string, std::shared_ptr<LightObject> >;
    using ConstantList = std::unordered_map<std::string, std::string>;
    using ObjectPointer = std::shared_ptr<LightObject>;
    using ImagePointer = std::shared_ptr<Image>;
    using ProcessorPointer = std::shared_ptr<Processor>;
    using ShapeArray = std::array<size_t, 3>;

    Operation () = delete;
    ~Operation () = default;
    Operation (const Operation &operation) = default;
    Operation (Operation &&operation) = default;
    auto operator= (const Operation &operation) -> Operation & = default;
    auto operator= (Operation &&operation) -> Operation & = default;
    explicit Operation (const ProcessorPointer &device, const size_t &nb_parameter = 0, const size_t &nb_constant = 0);

    auto AddParameter (const std::string &tag, const float &parameter) -> void;
    auto AddParameter (const std::string &tag, const int &parameter) -> void;
    auto AddParameter (const std::string &tag, const Image &parameter) -> void;
    auto AddConstant (const std::string &tag, const std::string &constant) -> void;
    auto GetParameter (const std::string &tag) -> ObjectPointer;
    auto GetImage (const std::string &tag) -> ImagePointer;
    auto SetRange (const ShapeArray &range) -> void;
    auto SetRange (const std::string &tag) -> void;
    auto SetSource (const std::string &name, const std::string &src) -> void;
    auto GetSource () -> std::string;
    auto GetName () -> std::string;
    auto LoadSource (const std::string &name, const std::string &file) -> void;
    virtual auto Execute () -> void;
    [[nodiscard]] auto ToString () const -> std::string;

  protected:
    [[nodiscard]] auto Device () const -> ProcessorPointer;
    [[nodiscard]] auto Source () const -> std::string;
    [[nodiscard]] auto Name () const -> std::string;
    [[nodiscard]] auto Kernel () const -> cl::Kernel;
    [[nodiscard]] auto MakeDefines () const -> std::string;
    static auto MakePreamble () -> std::string;
    auto MakeKernel () -> void;
    auto SetKernelArguments () -> bool;
    auto GetKernelArgumentsInfo () -> void;
    auto GetArgumentsInfo () -> void;
    auto EnqueueOperation () -> void;
    auto SetNumberOfParameters (const size_t &x) -> void;
    auto SetNumberOfConstants (const size_t &x) -> void;

    auto GenerateOutput (const std::string &input_tag = "src",
                         const std::string &output_tag = "dst") -> void;
    static auto GenerateOutput (const Image &object, const ShapeArray &shape) -> Image;

  private:
    cl::Kernel kernel_;
    std::string name_;
    std::string source_;
    ShapeArray range_ = { 0, 0, 0 };
    ProcessorPointer device_;
    ParameterList parameter_map_;
    ConstantList constant_map_;
};

} // namespace cle

#endif // CLIC_INCLUDE_CORE_CLEOPERATION_HPP