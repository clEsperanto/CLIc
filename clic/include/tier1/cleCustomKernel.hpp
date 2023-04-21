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
CustomKernel_Call(const std::shared_ptr<cle::Processor> &                             device,
                  const std::string &                                                 file_name,
                  const std::string &                                                 kernel_name,
                  const size_t &                                                      range_x,
                  const size_t &                                                      range_y,
                  const size_t &                                                      range_z,
                  const std::map<std::string, std::variant<cle::Image, float, int>> & parameters) -> void
{

  // TODO: cleaner but not working with macos 10.13 and earlier
  // if (std::holds_alternative<cle::Image>(ite->second))
  // {
  //   kernel.AddImage(ite->first, std::get<cle::Image>(ite->second));
  // }
  // else if (std::holds_alternative<float>(ite->second))
  // {
  //   kernel.AddScalar(ite->first, std::get<float>(ite->second));
  // }
  // else if (std::holds_alternative<int>(ite->second))
  // {
  //   kernel.AddScalar(ite->first, std::get<int>(ite->second));
  // }

  CustomKernel kernel(device, file_name, kernel_name, parameters.size());
  for (auto ite = parameters.begin(); ite != parameters.end(); ite++)
  {
    std::visit(
      [&](auto && arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, cle::Image>)
        {
          kernel.AddImage(ite->first, arg);
        }
        else if constexpr (std::is_same_v<T, float>)
        {
          kernel.AddScalar(ite->first, arg);
        }
        else if constexpr (std::is_same_v<T, int>)
        {
          kernel.AddScalar(ite->first, arg);
        }
      },
      ite->second);
  }
  kernel.SetRange({ range_x, range_y, range_z });
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLECUSTOMKERNEL_HPP
