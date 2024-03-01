#ifndef __INCLUDE_ARRAY_HPP
#define __INCLUDE_ARRAY_HPP

#include "backend.hpp"
#include "device.hpp"
#include "utils.hpp"

namespace cle
{

/**
 * @brief Array class
 *
 * This is the main data structure of the library. It is used to store data on the device and to perform operations on
 * it. The Array class is holding a device memory pointer (void *), its size and its type. An Array is not manipulated
 * directly, but through a smart pointer (Array::Pointer).
 */
class Array
{
public:
  using Pointer = std::shared_ptr<Array>;

  /**
   * @brief Create a new Array::Pointer object
   */
  static auto
  New() -> Array::Pointer
  {
    return std::shared_ptr<Array>(new Array());
  }

  /**
   * @brief Create an empty new Array::Pointer object
   * @param width width of the array
   * @param height height of the array
   * @param depth depth of the array
   * @param dimension dimension of the array (1, 2 or 3)
   * @param data_type data type of the array
   * @param mem_type memory type of the array
   * @param device_ptr device where the array is stored
   * @return Array::Pointer
   */
  static auto
  create(size_t                  width,
         size_t                  height,
         size_t                  depth,
         size_t                  dimension,
         const dType &           data_type,
         const mType &           mem_type,
         const Device::Pointer & device_ptr) -> Array::Pointer;

  /**
   * @brief Create a new Array::Pointer object from host data pointer
   * @param width width of the array
   * @param height height of the array
   * @param depth depth of the array
   * @param dimension dimension of the array (1, 2 or 3)
   * @param data_type data type of the array
   * @param mem_type memory type of the array
   * @param host_data pointer to the host data
   * @param device_ptr device where the array is stored
   * @return Array::Pointer
   */
  static auto
  create(size_t                  width,
         size_t                  height,
         size_t                  depth,
         size_t                  dimension,
         const dType &           data_type,
         const mType &           mem_type,
         const void *            host_data,
         const Device::Pointer & device_ptr) -> Array::Pointer;

  /**
   * @brief Create a new Array::Pointer object from another Array::Pointer
   * @param array Array::Pointer
   * @return Array::Pointer
   */
  static auto
  create(const Array::Pointer & array) -> Array::Pointer;

  /**
   * @brief operator << to print the Array::Pointer
   */
  friend auto
  operator<<(std::ostream & out, const Array::Pointer & array) -> std::ostream &;

  /**
   * @brief Allocate memory space of the array on the device
   */
  auto
  allocate() -> void;

  /**
   * @brief Write host data into the Array
   * @param host_data pointer to the host data
   */
  auto
  write(const void * host_data) -> void;

  /**
   * @brief Write host data into the Array region
   * @param host_data pointer to the host data
   * @param region region of the array to write
   * @param buffer_origin origin of the buffer to write
   */
  auto
  write(const void * host_data, const std::array<size_t, 3> & region, const std::array<size_t, 3> & buffer_origin)
    -> void;

  /**
   * @brief Write host data into the Array at a specific position (x, y, z)
   * @param host_data pointer to the host data
   * @param x_coord x coordinate
   * @param y_coord y coordinate
   * @param z_coord z coordinate
   */
  auto
  write(const void * host_data, size_t x_coord, size_t y_coord, size_t z_coord) -> void;

  /**
   * @brief Read the Array data into host memory
   * @param host_data pointer to the host data
   */
  auto
  read(void * host_data) const -> void;

  /**
   * @brief Read the Array region data into host memory
   * @param host_data pointer to the host data
   * @param region region of the array to read
   * @param buffer_origin origin of the buffer to read
   */
  auto
  read(void * host_data, const std::array<size_t, 3> & region, const std::array<size_t, 3> & buffer_origin) const
    -> void;

  /**
   * @brief Read the Array data at a specific position (x, y, z) into host memory
   * @param host_data pointer to the host data
   * @param x_coord x coordinate
   * @param y_coord y coordinate
   * @param z_coord z coordinate
   */
  auto
  read(void * host_data, size_t x_coord, size_t y_coord, size_t z_coord) const -> void;

  /**
   * @brief Copy the Array data into another Array
   * @param dst destination Array::Pointer
   */
  auto
  copy(const Array::Pointer & dst) const -> void;

  /**
   * @brief Copy the Array region data into another Array
   * @param dst destination Array::Pointer
   * @param region region of the array to copy
   * @param src_origin origin of the source buffer
   * @param dst_origin origin of the destination buffer
   */
  auto
  copy(const Array::Pointer &        dst,
       const std::array<size_t, 3> & region,
       const std::array<size_t, 3> & src_origin,
       const std::array<size_t, 3> & dst_origin) const -> void;

  /**
   * @brief Fill the Array with a specific value
   * @param value value to fill the array with
   */
  auto
  fill(float value) -> void;

  /**
   * @brief Get the size of the Array (number of elements)
   * @return size_t
   */
  [[nodiscard]] auto
  size() const -> size_t;

  /**
   * @brief Get the width of the Array
   * @return size_t
   */
  [[nodiscard]] auto
  width() const -> size_t;

  /**
   * @brief Get the height of the Array
   * @return size_t
   */
  [[nodiscard]] auto
  height() const -> size_t;

  /**
   * @brief Get the depth of the Array
   * @return size_t
   */
  [[nodiscard]] auto
  depth() const -> size_t;

  /**
   * @brief Get the item size of the Array (bytes)
   * @return size_t
   */
  [[nodiscard]] auto
  itemSize() const -> size_t;

  /**
   * @brief Get the data type of the Array
   * @return dType
   */
  [[nodiscard]] auto
  dtype() const -> dType;

  /**
   * @brief Get the memory type of the Array
   * @return mType
   */
  [[nodiscard]] auto
  mtype() const -> mType;

  /**
   * @brief Get the device pointer where the Array is stored
   * @return Device::Pointer
   */
  [[nodiscard]] auto
  device() const -> Device::Pointer;

  /**
   * @brief Get the dimension of the Array (1, 2 or 3)
   * @return unsigned int
   */
  [[nodiscard]] auto
  dim() const -> unsigned int;

  /**
   * @brief Get the dimension of the Array (1, 2 or 3)
   * @return unsigned int
   */
  [[nodiscard]] auto
  dimension() const -> unsigned int;

  /**
   * @brief Check if the Array is initialized
   * @return bool
   */
  [[nodiscard]] auto
  initialized() const -> bool;

  /**
   * @brief Get the data type as a short string
   * @return std::string
   */
  [[nodiscard]] auto
  shortType() const -> std::string;

  /**
   * @brief Get the memory pointer of the Array
   * @return void **
   */
  [[nodiscard]] auto
  get() const -> void **;

  /**
   * @brief Get the const memory pointer of the Array
   * @return const void **
   */
  [[nodiscard]] auto
  c_get() const -> const void **;

  /**
   * @brief destructor
   */
  ~Array();

  /**
   * @brief copy constructor
   */
  Array(const Array &) = default;

  /**
   * @brief Print the Array as a matrix for debugging
   */
  template <typename T>
  friend auto
  print(const Array::Pointer & array) -> void;

private:
  using MemoryPointer = std::shared_ptr<void *>;

  Array() = default;
  Array(size_t                  width,
        size_t                  height,
        size_t                  depth,
        size_t                  dimension,
        const dType &           data_type,
        const mType &           mem_type,
        const Device::Pointer & device_ptr);

  size_t          dim_ = 1;
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

template <typename T>
auto
print(const Array::Pointer & array) -> void
{
  std::vector<T> host_data(array->size());
  array->read(host_data.data());

  for (int i = 0; i < array->depth(); i++)
  {
    std::cout << "z = " << i << std::endl;
    for (int j = 0; j < array->height(); j++)
    {
      for (int k = 0; k < array->width(); k++)
      {
        std::cout << (float)host_data[i * array->height() * array->width() + j * array->width() + k] << " ";
      }
      std::cout << std::endl;
    }
  }
}

} // namespace cle


#endif // __INCLUDE_ARRAY_HPP
