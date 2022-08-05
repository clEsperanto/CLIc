
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

} // namespace cle

#endif // __TIER3_CLECLOSEINDEXGAPSINLABELMAPKERNEL_HPP
