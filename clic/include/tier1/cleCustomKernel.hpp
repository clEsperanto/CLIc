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
  AddImages(const std::map<std::string, Image> & images) -> void;

  auto
  AddScalars(const std::map<std::string, float> & scalars) -> void;
};

inline auto
CustomKernel_Call(const std::shared_ptr<cle::Processor> & device,
                  const std::string &                     filename,
                  const std::string &                     name,
                  const std::map<std::string, Image> &    images,
                  const std::map<std::string, float> &    scalars) -> void
{
  CustomKernel kernel(device, filename, name, images.size() + scalars.size());
  kernel.AddImages(images);
  kernel.AddScalars(scalars);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLECUSTOMKERNEL_HPP
