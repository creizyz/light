#ifndef __PIC_H__
#define __PIC_H__

#include <cstring>
#include <iostream>
#include <type_traits>

#include "plane.h"
#include "color.h"

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
class Pic
{
public:
  Plane<T> plane[3];
  color_range_t range;
  size_t bitdepth;

protected:
  Pic();

  bool allocate_default_impl(size_t height, size_t width, size_t padding = 0);

public:
  void release();

  bool write_frame(FILE *fp) const;
  bool write_frame(std::ofstream &stream) const;
  bool read_frame(std::ifstream &stream, color_range_t range, size_t bitdepth, int frame_number = -1);
  bool read_frame(FILE *fp, color_range_t range, size_t bitdepth, int frame_number = -1);

  bool upsampleH(Pic<T, space> &dst, size_t factor) const;
  bool upsampleV(Pic<T, space> &dst, size_t factor) const;

  template <typename T2, typename Function>
  bool convert_range(Pic<T2, space> &dst, color_range_t range, size_t bitdepth, Function transfer_function) const;
  template <color_space_t space_2>
  bool convert_to(Pic<T, space_2> &dst) const;
};

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
class PicLab : public Pic<T, Lab>
{
public:
  Plane<T> &L;
  Plane<T> &a;
  Plane<T> &b;

  PicLab();
  PicLab(size_t height, size_t width, size_t padding = 0);

  bool allocate(size_t height, size_t width, size_t padding = 0);
};

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
class PicRGB : public Pic<T, RGB>
{
public:
  Plane<T> &R;
  Plane<T> &G;
  Plane<T> &B;

  PicRGB();
  PicRGB(size_t height, size_t width, size_t padding = 0);
  bool allocate(size_t height, size_t width, size_t padding = 0);
};

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
class PicXYZ : public Pic<T, XYZ>
{
public:
  Plane<T> &X;
  Plane<T> &Y;
  Plane<T> &Z;

  PicXYZ();
  PicXYZ(size_t height, size_t width, size_t padding = 0);
  bool allocate(size_t height, size_t width, size_t padding = 0);
};

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
class PicYUV : public Pic<T, YUV>
{
protected:
  size_t sampling;

public:
  Plane<T> &Y;
  Plane<T> &U;
  Plane<T> &V;

  PicYUV();
  PicYUV(size_t height, size_t width, size_t format, size_t padding = 0);
  bool allocate(size_t height, size_t width, size_t sampling, size_t padding = 0);
  bool resample(PicYUV<T> &dst, size_t sampling);
};

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
Pic<T, space>::Pic()
: range(full_range)
, bitdepth(0)
{}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
bool Pic<T, space>::allocate_default_impl(size_t height, size_t width, size_t padding)
{
  return plane[0].allocate(height, width, padding)
      && plane[1].allocate(height, width, padding)
      && plane[2].allocate(height, width, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
void Pic<T, space>::release()
{
  plane[0].release();
  plane[1].release();
  plane[2].release();
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
bool Pic<T, space>::write_frame(FILE * fp) const
{
  return plane[0].write_plane(fp)
      && plane[1].write_plane(fp)
      && plane[2].write_plane(fp);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
bool Pic<T, space>::write_frame(std::ofstream & stream) const
{
  return plane[0].write_plane(stream)
      && plane[1].write_plane(stream)
      && plane[2].write_plane(stream);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
bool Pic<T, space>::read_frame(std::ifstream & stream, color_range_t range, size_t bitdepth, int frame_number)
{
  if (frame_number >= 0)
  {
    size_t frame_offset = (plane[0].get_plane_size()
                          + plane[1].get_plane_size()
                          + plane[2].get_plane_size()) * sizeof(T);
    stream.seekg(frame_offset * frame_number, stream.beg);
  }

  if (plane[0].read_plane(stream)
    && plane[1].read_plane(stream)
    && plane[2].read_plane(stream)
  ) {
    this->range    = range;
    this->bitdepth = bitdepth;
    return true;
  }
  return false;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
bool Pic<T, space>::read_frame(FILE * fp, color_range_t range, size_t bitdepth, int frame_number)
{
  if (frame_number >= 0)
  {
    size_t frame_offset = (plane[0].get_plane_size()
                          + plane[1].get_plane_size()
                          + plane[2].get_plane_size()) * sizeof(T);
    fseek(fp, frame_offset * frame_number, SEEK_SET);
  }

  if (plane[0].read_plane(fp)
    && plane[1].read_plane(fp)
    && plane[2].read_plane(fp)
  ) {
    this->range    = range;
    this->bitdepth = bitdepth;
    return true;
  }
  return false;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
template <typename T2, typename Function>
bool Pic<T, space>::convert_range(Pic<T2, space> & dst, color_range_t range, size_t bitdepth, Function transfer_function) const
{
  triple_t<T>  src_min   = color_range_details<T,  space>::min(this->range, this->bitdepth);
  triple_t<T>  src_range = color_range_details<T,  space>::range(this->range, this->bitdepth);
  triple_t<T2> dst_min   = color_range_details<T2, space>::min(range, bitdepth);
  triple_t<T2> dst_range = color_range_details<T2, space>::range(range, bitdepth);

  size_t width  = plane[0].get_width();
  size_t height = plane[0].get_height();

  for (size_t y = 0; y < height; y++)
  {
    for (size_t x = 0; x < width; x++)
    {
      dst.plane[0][y][x] = (T2) (transfer_function(((plane[0][y][x] - src_min[0]) / (double) src_range[0])) * (double) dst_range[0] + (double) dst_min[0] + (std::is_floating_point<T2>::value ? 0.0 : 0.5));
      dst.plane[1][y][x] = (T2) (transfer_function(((plane[1][y][x] - src_min[1]) / (double) src_range[1])) * (double) dst_range[1] + (double) dst_min[1] + (std::is_floating_point<T2>::value ? 0.0 : 0.5));
      dst.plane[2][y][x] = (T2) (transfer_function(((plane[2][y][x] - src_min[2]) / (double) src_range[2])) * (double) dst_range[2] + (double) dst_min[2] + (std::is_floating_point<T2>::value ? 0.0 : 0.5));
    }
  }
  
  dst.range    = range;
  dst.bitdepth = bitdepth;

  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
template <color_space_t space_2>
bool Pic<T, space>::convert_to(Pic<T, space_2> & dst) const
{
  size_t width  = plane[0].get_width();
  size_t height = plane[0].get_height();

  for (size_t y = 0; y < height; y++)
  {
    for (size_t x = 0; x < width; x++)
    {
      color_space_conversion<T, space, space_2>::convert(
          this->plane[0][y][x]
        , this->plane[1][y][x]
        , this->plane[2][y][x]
        , dst.plane[0][y][x]
        , dst.plane[1][y][x]
        , dst.plane[2][y][x]
      );
    }
  }
  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
bool Pic<T, space>::upsampleH(Pic<T, space> & dst, size_t factor) const
{
  if (plane[0].upsampleH(dst.plane[0], factor)
    && plane[1].upsampleH(dst.plane[1], factor)
    && plane[2].upsampleH(dst.plane[2], factor)
  ) {
    dst.space    = space;
    dst.range    = range;
    dst.bitdepth = bitdepth;
    return true;
  }
  return false;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T, color_space_t space>
bool Pic<T, space>::upsampleV(Pic<T, space> & dst, size_t factor) const
{
  if (plane[0].upsampleV(dst.plane[0], factor)
    && plane[1].upsampleV(dst.plane[1], factor)
    && plane[2].upsampleV(dst.plane[2], factor)
  ) {
    dst.space    = space;
    dst.range    = range;
    dst.bitdepth = bitdepth;
    return true;
  }
  return false;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
PicLab<T>::PicLab()
  : Pic<T, Lab>()
  , L(this->plane[0])
  , a(this->plane[1])
  , b(this->plane[2])
{ }

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
PicLab<T>::PicLab(size_t height, size_t width, size_t padding)
  : PicLab()
{
  this->allocate(height, width, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool PicLab<T>::allocate(size_t height, size_t width, size_t padding)
{
  return this->allocate_default_impl(height, width, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
PicRGB<T>::PicRGB()
  : Pic<T, RGB>()
  , R(this->plane[0])
  , G(this->plane[1])
  , B(this->plane[2])
{ }

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
PicRGB<T>::PicRGB(size_t height, size_t width, size_t padding)
  : PicRGB()
{
  this->allocate(height, width, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool PicRGB<T>::allocate(size_t height, size_t width, size_t padding)
{
  return this->allocate_default_impl(height, width, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
PicXYZ<T>::PicXYZ()
  : Pic<T, XYZ>()
  , X(this->plane[0])
  , Y(this->plane[1])
  , Z(this->plane[2])
{ }

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
PicXYZ<T>::PicXYZ(size_t height, size_t width, size_t padding)
  : PicXYZ()
{
  this->allocate(height, width, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool PicXYZ<T>::allocate(size_t height, size_t width, size_t padding)
{
  return this->allocate_default_impl(height, width, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
PicYUV<T>::PicYUV()
  : Pic<T, YUV>()
  , sampling(0)
  , Y(this->plane[0])
  , U(this->plane[1])
  , V(this->plane[2])
{ }

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
PicYUV<T>::PicYUV(size_t height, size_t width, size_t format, size_t padding)
  : PicYUV()
{
  this->allocate(height, width, format, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool PicYUV<T>::allocate(size_t height, size_t width, size_t sampling, size_t padding)
{
  bool success = true;
  switch (sampling)
  {
  case 400:
    success = this->plane[0].allocate(height, width, padding);
    break;
  case 420:
    success = this->plane[0].allocate(height, width, padding)
            && this->plane[1].allocate(height / 2, width / 2, padding)
            && this->plane[2].allocate(height / 2, width / 2, padding);
    break;
  case 422:
    success = this->plane[0].allocate(height, width, padding)
            && this->plane[1].allocate(height / 2, width, padding)
            && this->plane[2].allocate(height / 2, width, padding);
    break;
  case 444:
    success = this->plane[0].allocate(height, width, padding)
            && this->plane[1].allocate(height, width, padding)
            && this->plane[2].allocate(height, width, padding);
    break;
  default:
    std::cerr << "[error] - YUV subsampling format not recognized (" << sampling << ")" << std::endl;
    success = false;
    break;
  }
  if (success)
  {
    this->sampling = sampling;
  }
  return success;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool PicYUV<T>::resample(PicYUV<T> & dst, size_t sampling)
{
  bool success = true;
  if (this->sampling == dst.sampling)
  {
    success = this->plane[0].copy(dst.plane[0])
        && this->plane[1].copy(dst.plane[1])
        && this->plane[2].copy(dst.plane[2]);
  }
  else if (this->sampling == 420 && dst.sampling == 422)
  {
    success = this->plane[0].copy(dst.plane[0])
        && this->plane[1].upsampleH(dst.plane[1], 2)
        && this->plane[2].upsampleH(dst.plane[2], 2);
  }
  else if (this->sampling == 422 && dst.sampling == 444)
  {
    success = this->plane[0].copy(dst.plane[0])
        && this->plane[1].upsampleV(dst.plane[1], 2)
        && this->plane[2].upsampleV(dst.plane[2], 2);
  }
  else if (this->sampling == 420 && dst.sampling == 444)
  {
    Plane<T> U420(this->plane[0].get_height() / 2, this->plane[0].get_width());
    Plane<T> V420(this->plane[0].get_height() / 2, this->plane[0].get_width());
    success = this->plane[1].upsampleH(U420, 2)
        && this->plane[2].upsampleH(V420, 2)
        && this->plane[0].copy(dst.plane[0])
        && U420.upsampleV(dst.plane[1], 2)
        && V420.upsampleV(dst.plane[2], 2);
  }
  else
  {
    std::cerr << "[error] - YUV format conversion not supported (YUV" << this->sampling << " to " << dst.sampling << ")" << std::endl;
    return false;
  }
  if (success)
  {
    dst.sampling = sampling;
    dst.range = this->range;
    dst.bitdepth = this->bitdepth;
  }
  return success;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //

#endif