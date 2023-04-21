#ifndef __TIER1_CLECUSTOMKERNEL_HPP
#define __TIER1_CLECUSTOMKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class CustomKernel : public Operation
{
public:
  explicit CustomKernel(const ProcessorPointer & device,
                        const std::string &      filename,
                        const std::string &      name,
                        const size_t &           nb_parameters);

  auto
  AddImage(const std::string & tag, const cle::Image & object) -> void;

  auto
  AddScalar(const std::string & tag, const float & value) -> void;

  auto
  AddScalar(const std::string & tag, const int & value) -> void;
};

inline auto
CustomKernel_Call(const std::shared_ptr<cle::Processor> & device,
                  const std::string &                     filename,
                  const std::string &                     name,
                  const cle::ParametersMap &              parameters,
                  const std::array<size_t, 3> &           global_range = { 1, 1, 1 }) -> void
{
  CustomKernel kernel(device, filename, name, parameters.size());
  for (auto ite = parameters.begin(); ite != parameters.end(); ite++)
  {
    if (std::holds_alternative<cle::Image>(ite->second))
    {
      kernel.AddImage(ite->first, std::get<cle::Image>(ite->second));
    }
    else if (std::holds_alternative<float>(ite->second))
    {
      kernel.AddScalar(ite->first, std::get<float>(ite->second));
    }
    else if (std::holds_alternative<int>(ite->second))
    {
      kernel.AddScalar(ite->first, std::get<int>(ite->second));
    }
  }
  std::cout << "c++ global_range = ";
  for (auto x : global_range)
  {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  kernel.SetRange(global_range);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLECUSTOMKERNEL_HPP
