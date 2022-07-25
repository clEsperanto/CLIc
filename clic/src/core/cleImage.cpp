#include "cleImage.hpp"
#include "cleBackend.hpp"

namespace cle
{

Image::Image (const ProcessorPointer &device, const cl::Memory &data,
              const ShapeArray &shape, const DataType &data_type,
              const ObjectType &object_type, const ChannelsType &channels_type)
{
    this->data_ = data;
    this->device_ = device;
    this->shape_ = shape;
    this->data_type_ = data_type;
    this->object_type_ = object_type;
    this->channels_type_ = channels_type;
    if (this->shape_[2] > 1)
        {
            this->dim_ = 3;
        }
    else if (this->shape_[1] > 1)
        {
            this->dim_ = 2;
        }
    else
        {
            this->dim_ = 1;
        }
}

auto
Image::Fill (float value) const -> void
{
    if (this->IsBuffer ())
        {
            Backend::EnqueueFillBuffer (this->Device ()->Queue (), this->Get (), true, 0,
                                        this->Bytes (), value);
        }
    else
        {
            cl_float4 color = { value, value, value, value };
            Backend::EnqueueFillImage (this->Device ()->Queue (), this->Get (), true,
                                       this->Origin (), this->Shape (), color);
        }
}

auto
Image::CopyDataTo (const Image &dst_obj) const -> void
{
    if (this->Device ()->Context () != dst_obj.Device ()->Context ())
        {
            std::cerr << "Error in CopyDataTo : Memory Objects does not share the same Context. \n";
            return;
        }
    if (this->Bytes () != dst_obj.Bytes ())
        {
            std::cerr << "Error in CopyDataTo : Memory Objects does not the same bytes size. \n";
            return;
        }
    if (this->IsBuffer ())
        {
            if (dst_obj.IsBuffer ())
                {
                    Backend::EnqueueCopyBuffer (this->Device ()->Queue (),
                                                this->Get (), dst_obj.Get (),
                                                true, 0, 0, this->Bytes ());
                    return;
                }
            Backend::EnqueueCopyBufferToImage (this->Device ()->Queue (),
                                               this->Get (), dst_obj.Get (),
                                               true, 0, this->Origin (), dst_obj.Shape ());
            return;
        }
    if (this->IsImage ())
        {
            if (dst_obj.IsBuffer ())
                {
                    Backend::EnqueueCopyImageToBuffer (this->Device ()->Queue (),
                                                       this->Get (), dst_obj.Get (),
                                                       true, this->Origin (), this->Shape (), 0);
                    return;
                }
            Backend::EnqueueCopyImage (this->Device ()->Queue (),
                                       this->Get (), dst_obj.Get (),
                                       true, this->Origin (), this->Origin (), this->Shape ());
            return;
        }
}

auto
Image::Get () const -> cl::Memory
{
    return this->data_;
}

auto
Image::Device () const -> ProcessorPointer
{
    return this->device_;
}

auto
Image::Bytes () const -> size_t
{
    return this->Get ().getInfo<CL_MEM_SIZE> ();
}

auto
Image::Ndim () const -> unsigned int
{
    return this->dim_;
}

auto
Image::Shape () const -> ShapeArray
{
    return this->shape_;
}

auto
Image::Origin () const -> ShapeArray
{
    return this->origin_;
}

auto
Image::ObjectInfo () const -> std::string
{
    return this->object_type_.Str ();
}

auto
Image::DataInfo () const -> std::string
{
    return this->data_type_.Str ();
}

auto
Image::DataInfoShort () const -> std::string
{
    return this->data_type_.Str_s ();
}

auto
Image::IsBuffer () const -> bool
{
    return this->object_type_.IsBuffer ();
}

auto
Image::IsImage () const -> bool
{
    return this->object_type_.IsImage ();
}

auto
Image::BitType () const -> DataType
{
    return this->data_type_;
}

auto
Image::MemType () const -> ObjectType
{
    return this->object_type_;
}

auto
Image::ToString () const -> std::string
{
    std::string str = this->ObjectInfo () + "(" + this->DataInfo () + ")";
    str += " of shape=["
           + std::to_string (this->Shape ()[0]) + ","
           + std::to_string (this->Shape ()[1]) + ","
           + std::to_string (this->Shape ()[2]) + "]";
    return str;
}

} // namespace cle
