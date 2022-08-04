#ifndef __INCLUDE_UPSAMPLE_H__
#define __INCLUDE_UPSAMPLE_H__

#ifndef BYTE
#define BYTE unsigned char
#endif

void HorizontalCubicFilter_2D_half(BYTE **in, BYTE **out, int width, int height, int padding_size);
void HorizontalCubicFilter_2D_qpel(BYTE **in, BYTE **out, int width, int height, int padding_size);
void HorizontalCubicFilter_2D_oct(BYTE **in, BYTE **out, int width, int height, int padding_size); //NHK

void VerticalCubicFilter_2D_half(BYTE **in, BYTE **out, int width, int height, int padding_size);
void VerticalCubicFilter_2D_qpel(BYTE **in, BYTE **out, int width, int height, int padding_size);
void VerticalCubicFilter_2D_oct(BYTE **in, BYTE **out, int width, int height, int padding_size); //NHK

void DummyFilter_2D(BYTE **in, BYTE **out, int width, int height, int padding_size);

#endif