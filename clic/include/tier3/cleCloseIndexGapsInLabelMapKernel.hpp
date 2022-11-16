
#ifndef __TIER3_CLECLOSEINDEXGAPSINLABELMAPKERNEL_HPP
#define __TIER3_CLECLOSEINDEXGAPSINLABELMAPKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class CloseIndexGapsInLabelMapKernel : public Operation
{
private:
  int block_size_ = 0;

public:
  explicit CloseIndexGapsInLabelMapKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetBlockSize(const int & value) -> void;
  auto
  Execute() -> void override;
};

inline auto
CloseIndexGapsInLabelMapKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                    const Image &                           src,
                                    const Image &                           dst,
                                    const int &                             value) -> void
{
  CloseIndexGapsInLabelMapKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetBlockSize(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER3_CLECLOSEINDEXGAPSINLABELMAPKERNEL_HPP
