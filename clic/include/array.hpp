#ifndef __INCLUDE_ARRAY_HPP
#define __INCLUDE_ARRAY_HPP

#include "backend.hpp"
#include "device.hpp"
#include "utils.hpp"

#include <algorithm>
#include <variant>

namespace cle
{

class Array : public std::enable_shared_from_this<Array>
{
public:
  using Pointer = std::shared_ptr<Array>;
  static auto
  New() -> Array::Pointer
  {
    return std::shared_ptr<Array>(new Array());
  }
  static auto
  create(const size_t &          width,
         const size_t &          height,
         const size_t &          depth,
         const dType &           data_type,
         const mType &           mem_type,
         const Device::Pointer & device_ptr) -> Array::Pointer;
  static auto
  create(const size_t &          width,
         const size_t &          height,
         const size_t &          depth,
         const dType &           data_type,
         const mType &           mem_type,
         const void *            host_data,
         const Device::Pointer & device_ptr) -> Array::Pointer;
  static auto
  create(Array::Pointer array) -> Array::Pointer;

  friend auto
  operator<<(std::ostream & out, const Array::Pointer & array) -> std::ostream &;

  auto
  allocate() -> void;
  auto
  write(const void * host_data) -> void;
  auto
  read(void * host_data) const -> void;
  auto
  copy(const Array::Pointer & dst) const -> void;
  auto
  fill(const float & value) const -> void;

  [[nodiscard]] auto
  nbElements() const -> size_t;
  [[nodiscard]] auto
  width() const -> size_t;
  [[nodiscard]] auto
  height() const -> size_t;
  [[nodiscard]] auto
  depth() const -> size_t;
  [[nodiscard]] auto
  bytesPerElements() const -> size_t;
  [[nodiscard]] auto
  dtype() const -> dType;
  [[nodiscard]] auto
  mtype() const -> mType;
  [[nodiscard]] auto
  device() const -> Device::Pointer;
  [[nodiscard]] auto
  dim() const -> unsigned int;
  [[nodiscard]] auto
  initialized() const -> bool;
  [[nodiscard]] auto
  shortType() const -> std::string;
  [[nodiscard]] auto
  get() const -> void **;
  [[nodiscard]] auto
  c_get() const -> const void **;

  ~Array();
  Array(const Array &) = default;

private:
  using MemoryPointer = std::shared_ptr<void *>;

  Array() = default;
  Array(const size_t &          width,
        const size_t &          height,
        const size_t &          depth,
        const dType &           data_type,
        const mType &           mem_type,
        const Device::Pointer & device_ptr);

  size_t          width_ = 1;
  size_t          height_ = 1;
  size_t          depth_ = 1;
  dType           dataType_ = dType::FLOAT;
  mType           memType_ = mType::BUFFER;
  Device::Pointer device_ = nullptr;
  MemoryPointer   data_ = std::make_shared<void *>(nullptr);
  bool            initialized_ = false;
  const Backend & backend_ = cle::BackendManager::getInstance().getBackend();
};

} // namespace cle


#endif // __INCLUDE_ARRAY_HPP
