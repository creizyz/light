#ifndef __COLOR_CONV_H__
#define __COLOR_CONV_H__

  #include <cmath>
  #include <climits>
  #include <stddef.h>
  #include <type_traits>
  #include <iostream>

  enum color_space_t
  {
    RGB,
    YUV,
    XYZ,
    Lab
  };

  #define CLIP(val, min, max) ((val < min) ? min : ((val > max) ? max : val))

  // Rec. ITU 709-6
  const float RGB_2_YUV[9] = {
     0.2126,  0.7152,  0.0722,
    -0.1146, -0.3854,  0.5000,
     0.5000, -0.4542, -0.0458
  };
  // Rec. ITU 709-6 (inverse)
  const float YUV_2_RGB[9] = {
    1.000, -0.0001515,  1.575,
    1.000, -0.1873,    -0.4681,
    1.000,  1.856,      0.0001057
  };

  // sRGB
  const float RGB_2_XYZ[9] = {
    0.412386563,  0.357591491,  0.180450491,
    0.212636822,  0.715182982,  0.072180196,
    0.01933062,   0.119197164,  0.950372587
  };
  // sRGB
  const float XYZ_2_RGB[9] = {
    3.241003233,  -1.537398969,  -0.498615882,
   -0.969224252,   1.875929984,   0.041554226,
    0.05563942,   -0.204011206,   1.057148977
  };
  
  const float YUV_2_XYZ[9] = {
    0.9504,	 0.2679,      0.4821,
    1.000,  -0.00001954,  0.0001335,
    1.088,   1.742,      -0.02525
  };

  const float XYZ_2_YUV[9] = {
    -0.0001347, 1.000,  0.00004000,
     0.02994,  -0.6488, 0.5697,
     2.058,    -1.611, -0.3166
  };

  namespace reference_whitepoint
  {
    float D65[3] = {
      0.9505,
      1.000,
      1.089
    };

    float D55[3] = {
      0.9568,
      1.000,
      0.9214
    };

    float D50[3] = {
      0.9642,
      1.000,
      0.8251
    };

    float A[3] = {
      1.0985,
      1.0000,
      0.3558
    };

    float C[3] = {
      0.9807,
      1.0000,
      1.1822
    };

    float E[3] = {
      1.0,
      1.0,
      1.0
    };
  }

  #define WHITEPOINT reference_whitepoint::D65

  template <typename T>
  T XYZ_to_LAB_func(T val)
  {
    if (val > 0.008856) return std::cbrt(val);
    else return (val * 903.3 + 16) / 116;
  }

  template <typename T>
  T LAB_to_XYZ_func(T val)
  {
    if (val > 6.0/29.0) return val * val * val;
    else return (0.04280 * (val - 0.1379));
  }

  template <typename T, color_space_t space_1, color_space_t space_2>
  struct color_space_conversion
  {
    static void convert(T, T, T, T &, T &, T &);
  };

  template <typename T>
  struct color_space_conversion<T, RGB, YUV>
  {
    static void convert(T R, T G, T B, T & Y, T & U, T & V)
    {
      Y = RGB_2_YUV[0] * R + RGB_2_YUV[1] * G + RGB_2_YUV[2] * B;
      U = RGB_2_YUV[3] * R + RGB_2_YUV[4] * G + RGB_2_YUV[5] * B;
      V = RGB_2_YUV[6] * R + RGB_2_YUV[7] * G + RGB_2_YUV[8] * B;
    }
  };

  template <typename T>
  struct color_space_conversion<T, YUV, RGB>
  {
    static void convert(T Y, T U, T V, T & R, T & G, T & B)
    {
      R = CLIP(YUV_2_RGB[0] * Y + YUV_2_RGB[1] * U + YUV_2_RGB[2] * V, (T) 0, (T) 1);
      G = CLIP(YUV_2_RGB[3] * Y + YUV_2_RGB[4] * U + YUV_2_RGB[5] * V, (T) 0, (T) 1);
      B = CLIP(YUV_2_RGB[6] * Y + YUV_2_RGB[7] * U + YUV_2_RGB[8] * V, (T) 0, (T) 1);
    }
  };

  template <typename T>
  struct color_space_conversion<T, RGB, XYZ>
  {
    static void convert(T R, T G, T B, T & X, T & Y, T & Z)
    {
      X = RGB_2_XYZ[0] * R + RGB_2_XYZ[1] * G + RGB_2_XYZ[2] * B;
      Y = RGB_2_XYZ[3] * R + RGB_2_XYZ[4] * G + RGB_2_XYZ[5] * B;
      Z = RGB_2_XYZ[6] * R + RGB_2_XYZ[7] * G + RGB_2_XYZ[8] * B;
    }
  };

  template <typename T>
  struct color_space_conversion<T, XYZ, RGB>
  {
    static void convert(T X, T Y, T Z, T & R, T & G, T & B)
    {
      R = XYZ_2_RGB[0] * X + XYZ_2_RGB[1] * Y + XYZ_2_RGB[2] * Z;
      G = XYZ_2_RGB[3] * X + XYZ_2_RGB[4] * Y + XYZ_2_RGB[5] * Z;
      B = XYZ_2_RGB[6] * X + XYZ_2_RGB[7] * Y + XYZ_2_RGB[8] * Z;
    }
  };

  template <typename T>
  struct color_space_conversion<T, YUV, XYZ>
  {
    static void convert(T Y_yuv, T U_yuv, T V_yuv, T & X_xyz, T & Y_xyz, T & Z_xyz)
    {
      X_xyz = YUV_2_XYZ[0] * Y_yuv + YUV_2_XYZ[1] * U_yuv + YUV_2_XYZ[2] * V_yuv;
      Y_xyz = YUV_2_XYZ[3] * Y_yuv + YUV_2_XYZ[4] * U_yuv + YUV_2_XYZ[5] * V_yuv;
      Z_xyz = YUV_2_XYZ[6] * Y_yuv + YUV_2_XYZ[7] * U_yuv + YUV_2_XYZ[8] * V_yuv;
    }
  };

  template <typename T>
  struct color_space_conversion<T, XYZ, YUV>
  {
    static void convert(T X_xyz, T Y_xyz, T Z_xyz, T & Y_yuv, T & U_yuv, T & V_yuv)
    {
      Y_yuv = XYZ_2_YUV[0] * X_xyz + XYZ_2_YUV[1] * Y_xyz + XYZ_2_YUV[2] * Z_xyz;
      U_yuv = XYZ_2_YUV[3] * X_xyz + XYZ_2_YUV[4] * Y_xyz + XYZ_2_YUV[5] * Z_xyz;
      V_yuv = XYZ_2_YUV[6] * X_xyz + XYZ_2_YUV[7] * Y_xyz + XYZ_2_YUV[8] * Z_xyz;
    }
  };

  template <typename T>
  struct color_space_conversion<T, Lab, XYZ>
  {
    static void convert(T L, T a, T b, T & X, T & Y, T & Z)
    {
      T P = (L + 16.f) / 116.f;
      Y = WHITEPOINT[1] * LAB_to_XYZ_func(P);
      X = WHITEPOINT[0] * LAB_to_XYZ_func(P + (a / 500.f));
      Z = WHITEPOINT[2] * LAB_to_XYZ_func(P - (b / 200.f));
    }
  };

  template <typename T>
  struct color_space_conversion<T, XYZ, Lab>
  {
    static void convert(T X, T Y, T Z, T & L, T & a, T & b)
    {
      T r_X = X / WHITEPOINT[0];
      T r_Y = Y / WHITEPOINT[1];
      T r_Z = Z / WHITEPOINT[2];

      L = 116.f *  XYZ_to_LAB_func(r_Y) - 16.f;
      a = 500.f * (XYZ_to_LAB_func(r_X) - XYZ_to_LAB_func(r_Y));
      b = 500.f * (XYZ_to_LAB_func(r_Y) - XYZ_to_LAB_func(r_Z));
    }
  };

  enum color_range_t
  {
    unknown_range,
    normal_range,
    full_range,
    float_range
  };

  template <typename T>
  struct triple_t
  {
    T data[3];
    T operator[](int i) { return data[i]; }
    triple_t(T a, T b, T c) : data{a,b,c} {}
  };

  template <typename T, color_space_t space>
  struct color_range_details
  {
    static triple_t<T> min(color_range_t range, size_t bitdepth);
    static triple_t<T> range(color_range_t range, size_t bitdepth);
  };

  template <typename T>
  struct color_range_details<T, RGB>
  {
    static triple_t<T> min(color_range_t range, size_t bitdepth)
    {
      if (range == normal_range)
      {
        if (bitdepth ==  8) return triple_t<T>((T)16, (T)16, (T)16);
        if (bitdepth == 16) return triple_t<T>((T)64, (T)64, (T)64);
      }
      else if (range == full_range)
      {
        return triple_t<T>(0, 0, 0);
      }
      else if (range == float_range) return triple_t<T>((T)0, (T)0, (T)0);

      std::cerr << "error : unknown color min" << std::endl;
      return triple_t<T>(0,0,0);
    }
    static triple_t<T> range(color_range_t range, size_t bitdepth)
    {
      if (range == normal_range)
      {
        if (bitdepth ==  8) return triple_t<T>((T)219, (T)219, (T)219);
        if (bitdepth == 10) return triple_t<T>((T)876, (T)876, (T)876);
      }
      else if (range == full_range)
      {
        if (bitdepth ==  8) return triple_t<T>((T)255, (T)255, (T)255);
        if (bitdepth == 10) return triple_t<T>((T)1024, (T)1024, (T)1024);
        if (bitdepth == 16) return triple_t<T>((T)65535, (T)65535, (T)65535);
      }
      else if (range == float_range) return triple_t<T>((T)1.0, (T)1.0, (T)1.0);

      std::cerr << "error : unknown color range" << std::endl;
      return triple_t<T>(0,0,0);
    }
  };

  template <typename T>
  struct color_range_details<T, YUV>
  {
    static triple_t<T> min(color_range_t range, size_t bitdepth)
    {
      if (range == normal_range)
      {
        if (bitdepth ==  8) return triple_t<T>((T)16, (T)16, (T)16);
        if (bitdepth == 16) return triple_t<T>((T)64, (T)64, (T)64);
      }
      else if (range == full_range)
      {
        return triple_t<T>(0, 0, 0);
      }
      else if (range == float_range) return triple_t<T>((T)0.0, (T)-0.5, (T)-0.5);

      std::cerr << "error : unknown color min" << std::endl;
      return triple_t<T>(0,0,0);
    }
    static triple_t<T> range(color_range_t range, size_t bitdepth)
    {
      if (range == normal_range)
      {
        if (bitdepth ==  8) return triple_t<T>((T)219, (T)224, (T)224);
        if (bitdepth == 10) return triple_t<T>((T)876, (T)896, (T)896);
      }
      else if (range == full_range)
      {
        if (bitdepth ==  8) return triple_t<T>((T)255, (T)255, (T)255);
        if (bitdepth == 10) return triple_t<T>((T)1024, (T)1024, (T)1024);
        if (bitdepth == 16) return triple_t<T>((T)65535, (T)65535, (T)65535);
      }
      else if (range == float_range) return triple_t<T>((T)1.0, (T)1.0, (T)1.0);

      std::cerr << "error : unknown color range" << std::endl;
      return triple_t<T>(0,0,0);
    }
  };

  namespace transfer_functions
  {
    template <typename T> T pass_through(T val) { return val; }
  }

#endif