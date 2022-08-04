#ifndef __PLANE_H__
#define __PLANE_H__

#include <stddef.h>
#include <fstream>

#include "upsample.h"

enum class PaddingType
{
  CLEAR,
  CLIP
};

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
class Plane
{
private:
  T * buffer;
  T ** rows;

  size_t width;
  size_t height;
  size_t padding;
  size_t stride;
  size_t plane_size;
  size_t buffer_size;

public:
  Plane();
  Plane(size_t height, size_t width, size_t padding = 0);
  Plane(const Plane<T> & other, size_t padding = 0);
  ~Plane();

  bool                allocate        (size_t height, size_t width, size_t padding = 0);
  void                release         ();
  bool                valid           ();

  void                update_padding  (const PaddingType type);

  bool                write_plane     (FILE * fp) const;
  bool                read_plane      (FILE * fp);
  bool                write_plane     (std::ofstream & stream) const;
  bool                read_plane      (std::ifstream & stream);

  bool                copy            (Plane<T> & dst) const;
  Plane<T> &          operator=       (const Plane<T> & other);

  bool                upsampleH       (Plane<T> & dst, size_t factor) const;
  bool                upsampleV       (Plane<T> & dst, size_t factor) const;
  bool                subsampleH      (Plane<T> & dst, size_t factor) const;
  bool                subsampleV      (Plane<T> & dst, size_t factor) const;

  inline       size_t get_width       ()               const { return width;      }
  inline       size_t get_height      ()               const { return height;     }
  inline       size_t get_padding     ()               const { return padding;    }
  inline       size_t get_plane_size  ()               const { return plane_size; }
  inline       T *    operator[]      (const long row)       { return rows[padding + row];  }
  inline const T *    operator[]      (const long row) const { return rows[padding + row];  }
};


// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
Plane<T>::Plane()
  : buffer(nullptr)
  , rows(nullptr)
  , width(0)
  , height(0)
  , padding(0)
  , stride(0)
  , plane_size(0)
  , buffer_size(0)
{ }

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
Plane<T>::Plane(size_t height, size_t width, size_t padding)
  : Plane()
{
  this->allocate(height, width, padding);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
Plane<T>::Plane(const Plane<T> & other, size_t padding)
  : Plane()
{
  if (allocate(other.height, other.width, padding))
  {
    for (size_t y = 0; y < height; y++)
    {
      memccpy(rows[y], other.rows[y], width * sizeof(T), 1);
    }
  }
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
Plane<T>::~Plane()
{
  this->release();
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::allocate(size_t height, size_t width, size_t padding)
{
  if (buffer != nullptr) this->release();

  size_t double_padding = 2 * padding;
  size_t buffer_height = double_padding + height;

  size_t stride = double_padding + width;
  size_t plane_size = width * height;
  size_t buffer_size = buffer_height * stride;

  bool success = false;
  
  if ((buffer = new T[buffer_size]) != nullptr)
  {
    if ((rows = new T*[buffer_height]) != nullptr)
    {
      this->width = width;
      this->height = height;
      this->stride = stride;
      this->padding = padding;
      this->plane_size = plane_size;
      this->buffer_size = buffer_size;

      success = true;

      size_t plane_addr = padding * stride;
      for (size_t y = 0; y < buffer_height; y++)
      {
        rows[y] = &(buffer[plane_addr + y * stride]);
      }
    }
    else
    {
      delete[] buffer;
      buffer = nullptr;
    }
  }

  if (!success)
  {
    std::cerr << "[error] - could not allocate enough memory (" << buffer_size << " bytes plane allocation)" << std::endl;
  }

  return success;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
void Plane<T>::release()
{
  if (rows != nullptr)
  {
    delete[] rows;
    rows = nullptr;
  }
  if (buffer != nullptr)
  {
    delete[] buffer;
    buffer = nullptr;
  }
  width   = 0;
  height  = 0;
  padding = 0;
  stride  = 0;
  plane_size = 0;
  buffer_size = 0;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::valid()
{
  return buffer != nullptr;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
Plane<T> & Plane<T>::operator=(const Plane<T> & other)
{
  if (width == other.width && height == other.height || allocate(other.height, other.width, padding))
  {
    for (size_t y = 0; y < height; y++)
    {
      memccpy(rows[y], other.rows[y], width * sizeof(T), 1);
    }
  }
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
void Plane<T>::update_padding(const PaddingType type)
{
  if (padding > 0)
  {
    if (type == PaddingType::CLEAR)
    {
      for (int n = 1; n <= padding; n++)
      {
        int offset = n * stride;
        memset(&buffer[     0    ][      -offset    ], 0, width);   // top
        memset(&buffer[height - 1][       offset    ], 0, width);   // bottom
        memset(&buffer[     0    ][-offset - padding], 0, padding); // top left
        memset(&buffer[     0    ][  width - offset ], 0, padding); // top right
        memset(&buffer[height - 1][ offset - padding], 0, padding); // bottom left
        memset(&buffer[height - 1][  width + offset ], 0, padding); // bottom right
      }
      for (int y = 0; y < height; y++)
      {
        memset(&buffer[y][-padding], 0, padding); // left
        memset(&buffer[y][  width ], 0, padding); // right
      }
    }
    else if (type == PaddingType::CLIP)
    {
      for (int n = 1; n <= padding; n++)
      {
        int offset = n * stride;
        memcpy(&buffer[     0    ][     -offset     ], buffer[     0    ], width);              // top
        memcpy(&buffer[height - 1][      offset     ], buffer[height - 1], width);              // bottom
        memset(&buffer[     0    ][-offset - padding], buffer[     0    ][    0    ], padding); // top left
        memset(&buffer[     0    ][  width - offset ], buffer[     0    ][width - 1], padding); // top right
        memset(&buffer[height - 1][ offset - padding], buffer[height - 1][    0    ], padding); // bottom left
        memset(&buffer[height - 1][  width + offset ], buffer[height - 1][width - 1], padding); // bottom right
      }
      for (int y = 0; y < height; y++)
      {
        memset(&buffer[y][-padding], buffer[y][    0    ], padding); // left
        memset(&buffer[y][  width ], buffer[y][width - 1], padding); // right
      }
    }
    else
    {
      fprintf(stderr, "unsupported border type\n");
    }
  }
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::write_plane(FILE * fp) const
{
  if (fp == NULL)
  {
    std::cerr << "[error] - uninitialized file handle (plane writting)" << std::endl;
    return false;
  }
  if (buffer == nullptr)
  {
    std::cerr << "[error] - trying to write empty plane to file" << std::endl;
    return false;
  }
  
  for (size_t y = 0; y < height; y++)
  {
    if (fwrite(rows[y], width * sizeof(T), 1, fp) != 1)
    {
      std::cerr << "[error] - unknwon error while writting plane" << std::endl;
      return false;
    }
  }
  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::write_plane(std::ofstream & stream) const
{
  if (!stream.is_open())
  {
    std::cerr << "[error] - uninitialized file handle (plane writting)" << std::endl;
    return false;
  }
  if (buffer == nullptr)
  {
    std::cerr << "[error] - trying to write empty plane to file" << std::endl;
    return false;
  }
  
  for (size_t y = 0; y < height; y++)
  {
    if (!stream.write((char*)rows[y], width * sizeof(T)))
    {
      std::cerr << "[error] - unknwon error while writting plane" << std::endl;
      return false;
    }
  }
  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::read_plane(FILE * fp)
{
  if (fp == NULL)
  {
    std::cerr << "[error] - uninitialized file handle (plane reading)" << std::endl;
    return false;
  }
  if (buffer == nullptr)
  {
    std::cerr << "[error] - trying to read file from an empty plane" << std::endl;
    return false;
  }
  
  for (size_t y = 0; y < height; y++)
  {
    if (fread(rows[y], width * sizeof(T), 1, fp) != 1)
    {
      std::cerr << "[error] - unexpected end of file (plane reading)" << std::endl;
      return false;
    }
  }
  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::read_plane(std::ifstream & stream)
{
  if (!stream.is_open())
  {
    std::cerr << "[error] - uninitialized file handle (plane reading)" << std::endl;
    return false;
  }
  if (buffer == nullptr)
  {
    std::cerr << "[error] - trying to read file from an empty plane" << std::endl;
    return false;
  }
  
  for (size_t y = 0; y < height; y++)
  {
    if (!stream.read((char*)rows[y], width * sizeof(T)))
    {
      std::cerr << "[error] - unexpected end of file (plane reading)" << std::endl;
      return false;
    }
  }
  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::copy(Plane<T> & dst) const
{
  if (width != dst.width || height != dst.height)
  {
    std::cerr << "[error] - invalid destination for plane copy (" << width << "x" << height << " to " << dst.width << "x" << dst.height << ")" << std::endl;
    return false;
  }
  else
  {
    for (size_t y = 0; y < height; y++)
    {
      memcpy(dst.rows[y], this->rows[y], width * sizeof(T));
    }
  }
  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::upsampleH(Plane<T> & dst, size_t factor) const
{
  switch (factor)
  {
  case 1:
    this->copy(dst);
    break;
  case 2:
    HorizontalCubicFilter_2D_half(this->rows, dst.rows, width, height, 0);
    break;
  case 4:
    HorizontalCubicFilter_2D_qpel(this->rows, dst.rows, width, height, 0);
    break;
  case 8:
    HorizontalCubicFilter_2D_oct(this->rows, dst.rows, width, height, 0);
    break;
  default:
    std::cerr << "[error] - plane horizontal upsampling factor not supported (" << factor << ")" << std::endl;
    return false;
    break;
  }
  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::upsampleV(Plane<T> & dst, size_t factor) const
{
  switch (factor)
  {
  case 1:
    break;
  case 2:
    VerticalCubicFilter_2D_half(this->rows, dst.rows, width, height, 0);
    break;
  case 4:
    VerticalCubicFilter_2D_qpel(this->rows, dst.rows, width, height, 0);
    break;
  case 8:
    VerticalCubicFilter_2D_oct(this->rows, dst.rows, width, height, 0);
    break;
  default:
    std::cerr << "[error] - plane vertical upsampling factor not supported (" << factor << ")" << std::endl;
    return false;
    break;
  }
  return true;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::subsampleH(Plane<T> & dst, size_t factor) const
{
  std::cerr << "[error] - plane horizontal subsampling not supported" << std::endl;
  return false;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

template <typename T>
bool Plane<T>::subsampleV(Plane<T> & dst, size_t factor) const
{
  std::cerr << "[error] - plane vertical subsampling not supported" << std::endl;
  return false;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = //

#endif