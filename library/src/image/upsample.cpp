#include "upsample.h"

#include <iostream>

#ifndef CLIP3
#define CLIP3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

#ifndef TURN3
#define TURN3(x, min, max) ((x) < (min) ? -(x) : ((x) > (max) ? (2 * (max) - (x)) : (x))) //TDU
#endif

void HorizontalCubicFilter_2D_half(BYTE **in, BYTE **out, int width, int height, int padding_size)
{
	int i, ipel[4], opel[2];
	int max_width = width + padding_size;
	int width_minus1 = width - 1;
	int h;

	for (i = -padding_size; i < max_width; i++)
	{
		ipel[0] = TURN3(i - 1, 0, width_minus1);
		ipel[1] = TURN3(i, 0, width_minus1);
		ipel[2] = TURN3(i + 1, 0, width_minus1);
		ipel[3] = TURN3(i + 2, 0, width_minus1);

		opel[0] = (i + padding_size) << 1;
		opel[1] = opel[0] + 1;

		for (h = 0; h < height; h++)
		{
			out[h][opel[0]] = in[h][ipel[1]];
			out[h][opel[1]] = CLIP3((5 * (in[h][ipel[1]] + in[h][ipel[2]]) - in[h][ipel[0]] - in[h][ipel[3]] + 4) >> 3, 0, 255);
		}
	}
}

void HorizontalCubicFilter_2D_qpel(BYTE **in, BYTE **out, int width, int height, int padding_size)
{
	int i, ipel[4], opel[4];
	int max_width = width + padding_size;
	int width_minus1 = width - 1;
	int h;

	for (i = -padding_size; i < max_width; i++)
	{
		ipel[0] = TURN3(i - 1, 0, width_minus1);
		ipel[1] = TURN3(i, 0, width_minus1);
		ipel[2] = TURN3(i + 1, 0, width_minus1);
		ipel[3] = TURN3(i + 2, 0, width_minus1);

		opel[0] = (i + padding_size) << 2;
		opel[1] = opel[0] + 1;
		opel[2] = opel[0] + 2;
		opel[3] = opel[0] + 3;

		for (h = 0; h < height; h++)
		{
			out[h][opel[0]] = in[h][ipel[1]];
			out[h][opel[1]] = CLIP3((57 * in[h][ipel[1]] + 19 * in[h][ipel[2]] - 9 * in[h][ipel[0]] - 3 * in[h][ipel[3]] + 32) >> 6, 0, 255);
			out[h][opel[2]] = CLIP3((5 * (in[h][ipel[1]] + in[h][ipel[2]]) - in[h][ipel[0]] - in[h][ipel[3]] + 4) >> 3, 0, 255);
			out[h][opel[3]] = CLIP3((19 * in[h][ipel[1]] + 57 * in[h][ipel[2]] - 3 * in[h][ipel[0]] - 9 * in[h][ipel[3]] + 32) >> 6, 0, 255);
		}
	}
}

//NHK start
void HorizontalCubicFilter_2D_oct(BYTE **in, BYTE **out, int width, int height, int padding_size)
{
	int i, ipel[4], opel[8];
	int max_width = width + padding_size;
	int width_minus1 = width - 1;
	int h;

	for (i = -padding_size; i < max_width; i++)
	{
		ipel[0] = CLIP3(i - 1, 0, width_minus1);
		ipel[1] = CLIP3(i, 0, width_minus1);
		ipel[2] = CLIP3(i + 1, 0, width_minus1);
		ipel[3] = CLIP3(i + 2, 0, width_minus1);

		opel[0] = (i + padding_size) << 3;
		opel[1] = opel[0] + 1;
		opel[2] = opel[0] + 2;
		opel[3] = opel[0] + 3;
		opel[4] = opel[0] + 4;
		opel[5] = opel[0] + 5;
		opel[6] = opel[0] + 6;
		opel[7] = opel[0] + 7;

		for (h = 0; h < height; h++)
		{
			out[h][opel[0]] = in[h][ipel[1]];
			out[h][opel[1]] = CLIP3((-6 * in[h][ipel[0]] + 62 * in[h][ipel[1]] + 9 * in[h][ipel[2]] - 1 * in[h][ipel[3]] + 32) >> 6, 0, 255);
			out[h][opel[2]] = CLIP3((-9 * in[h][ipel[0]] + 57 * in[h][ipel[1]] + 19 * in[h][ipel[2]] - 3 * in[h][ipel[3]] + 32) >> 6, 0, 255);
			out[h][opel[3]] = CLIP3((-9 * in[h][ipel[0]] + 49 * in[h][ipel[1]] + 30 * in[h][ipel[2]] - 6 * in[h][ipel[3]] + 32) >> 6, 0, 255);
			out[h][opel[4]] = CLIP3((-8 * in[h][ipel[0]] + 40 * in[h][ipel[1]] + 40 * in[h][ipel[2]] - 8 * in[h][ipel[3]] + 32) >> 6, 0, 255);
			out[h][opel[5]] = CLIP3((-6 * in[h][ipel[0]] + 30 * in[h][ipel[1]] + 49 * in[h][ipel[2]] - 9 * in[h][ipel[3]] + 32) >> 6, 0, 255);
			out[h][opel[6]] = CLIP3((-3 * in[h][ipel[0]] + 19 * in[h][ipel[1]] + 57 * in[h][ipel[2]] - 9 * in[h][ipel[3]] + 32) >> 6, 0, 255);
			out[h][opel[7]] = CLIP3((-1 * in[h][ipel[0]] + 9 * in[h][ipel[1]] + 62 * in[h][ipel[2]] - 6 * in[h][ipel[3]] + 32) >> 6, 0, 255);
		}
	}
}
//NHK end

void VerticalCubicFilter_2D_half(BYTE **in, BYTE **out, int width, int height, int padding_size)
{
	int i, ipel[4], opel[2];
	int max_width = width + padding_size;
	int height_minus1 = height - 1;
	int h;

	for (h = 0; h < height; h++)
	{
		ipel[0] = TURN3(h - 1, 0, height_minus1);
		ipel[1] = TURN3(h, 0, height_minus1);
		ipel[2] = TURN3(h + 1, 0, height_minus1);
		ipel[3] = TURN3(h + 2, 0, height_minus1);

		opel[0] = (h) << 1;
		opel[1] = opel[0] + 1;

		for (i = -padding_size; i < max_width; i++)
		{
			out[opel[0]][i] = in[ipel[1]][i];
			out[opel[1]][i] = CLIP3((5 * (in[ipel[1]][i] + in[ipel[2]][i]) - in[ipel[0]][i] - in[ipel[3]][i] + 4) >> 3, 0, 255);
		}
	}
}

void VerticalCubicFilter_2D_qpel(BYTE **in, BYTE **out, int width, int height, int padding_size)
{
	int i, ipel[4], opel[4];
	int max_width = width + padding_size;
	int height_minus1 = height - 1;
	int h;

	for (h = 0; h < height; h++)
	{
		ipel[0] = TURN3(h - 1, 0, height_minus1);
		ipel[1] = TURN3(h, 0, height_minus1);
		ipel[2] = TURN3(h + 1, 0, height_minus1);
		ipel[3] = TURN3(h + 2, 0, height_minus1);

		opel[0] = (h) << 2;
		opel[1] = opel[0] + 1;
		opel[2] = opel[0] + 2;
		opel[3] = opel[0] + 3;

		for (i = -padding_size; i < max_width; i++)
		{
			out[opel[0]][i] = in[ipel[1]][i];
			out[opel[1]][i] = CLIP3((57 * in[ipel[1]][i] + 19 * in[ipel[2]][i] - 9 * in[ipel[0]][i] - 3 * in[ipel[3]][i] + 32) >> 6, 0, 255);
			out[opel[2]][i] = CLIP3((5 * (in[ipel[1]][i] + in[ipel[2]][i]) - in[ipel[0]][i] - in[ipel[3]][i] + 4) >> 3, 0, 255);
			out[opel[3]][i] = CLIP3((19 * in[ipel[1]][i] + 57 * in[ipel[2]][i] - 3 * in[ipel[0]][i] - 9 * in[ipel[3]][i] + 32) >> 6, 0, 255);
		}
	}
}

//NHK start
void VerticalCubicFilter_2D_oct(BYTE **in, BYTE **out, int width, int height, int padding_size)
{
	int i, ipel[4], opel[8];
	int max_width = width + padding_size;
	int height_minus1 = height - 1;
	int h;

	for (h = 0; h < height; h++)
	{
		ipel[0] = CLIP3(h - 1, 0, height_minus1);	//NICT
		ipel[1] = CLIP3(h, 0, height_minus1);			//NICT
		ipel[2] = CLIP3(h + 1, 0, height_minus1);	//NICT
		ipel[3] = CLIP3(h + 2, 0, height_minus1);	//NICT

		opel[0] = (h) << 3;
		opel[1] = opel[0] + 1;
		opel[2] = opel[0] + 2;
		opel[3] = opel[0] + 3;
		opel[4] = opel[0] + 4;
		opel[5] = opel[0] + 5;
		opel[6] = opel[0] + 6;
		opel[7] = opel[0] + 7;

		for (i = 0; i < max_width; i++)
		{
			out[opel[0]][i] = in[ipel[1]][i];
			out[opel[1]][i] = CLIP3((-6 * in[ipel[0]][i] + 62 * in[ipel[1]][i] +  9 * in[ipel[2]][i] - 1 * in[ipel[3]][i] + 32) >> 6, 0, 255);
			out[opel[2]][i] = CLIP3((-9 * in[ipel[0]][i] + 57 * in[ipel[1]][i] + 19 * in[ipel[2]][i] - 3 * in[ipel[3]][i] + 32) >> 6, 0, 255);
			out[opel[3]][i] = CLIP3((-9 * in[ipel[0]][i] + 49 * in[ipel[1]][i] + 30 * in[ipel[2]][i] - 6 * in[ipel[3]][i] + 32) >> 6, 0, 255);
			out[opel[4]][i] = CLIP3((-8 * in[ipel[0]][i] + 40 * in[ipel[1]][i] + 40 * in[ipel[2]][i] - 8 * in[ipel[3]][i] + 32) >> 6, 0, 255);
			out[opel[5]][i] = CLIP3((-6 * in[ipel[0]][i] + 30 * in[ipel[1]][i] + 49 * in[ipel[2]][i] - 9 * in[ipel[3]][i] + 32) >> 6, 0, 255);
			out[opel[6]][i] = CLIP3((-3 * in[ipel[0]][i] + 19 * in[ipel[1]][i] + 57 * in[ipel[2]][i] - 9 * in[ipel[3]][i] + 32) >> 6, 0, 255);
			out[opel[7]][i] = CLIP3((-1 * in[ipel[0]][i] +  9 * in[ipel[1]][i] + 62 * in[ipel[2]][i] - 6 * in[ipel[3]][i] + 32) >> 6, 0, 255);
		}
	}
}
//NHK end

void DummyFilter_2D(BYTE **in, BYTE **out, int width, int height, int padding_size)
{
	int h, i;
	int max_width = width + padding_size;
	for (h = 0; h < height; h++)
	{
		for (i = -padding_size; i < max_width; i++)
		{
			out[h][i] = in[h][i];
		}
	}
	return;
}
