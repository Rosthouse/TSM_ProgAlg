#include <iostream>
#include <memory.h>
#include <cassert>
#include <iomanip>
#include <cmath>
#include <Stopwatch.h>
#include "FreeImagePlus.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
// Windows Types
#ifndef WIN32
typedef DWORD COLORREF;

/*
typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
} RGBQUAD;
*/
#endif

#ifndef FSIZE
#define FSIZE 3
#endif

////////////////////////////////////////////////////////////////////////
static BYTE dist(int x, int y)
{
	int d = (int)sqrt(x * x + y * y);
	return (d < 256) ? d : 255;
}

const int H_FILTER[][FSIZE] = {
	{1, 1, 1},
	{0, 0, 0},
	{-1, -1, -1}
};
const int V_FILTER[][FSIZE] = {
	{1, 0, -1},
	{1, 0, -1},
	{1, 0, -1}
};

int filterRedH(int x, RGBQUAD pixels[])
{
	return H_FILTER[x][0] * pixels[0].rgbRed + H_FILTER[x][1] * pixels[1].rgbRed + H_FILTER[x][2] * pixels[2].rgbRed;
}

int filterRedV(int x, RGBQUAD pixels[])
{
	return V_FILTER[x][0] * pixels[0].rgbRed + H_FILTER[x][1] * pixels[1].rgbRed + H_FILTER[x][2] * pixels[2].rgbRed;
}

int filterGreenH(int x, RGBQUAD pixels[])
{
	return H_FILTER[x][0] * pixels[0].rgbGreen + H_FILTER[x][1] * pixels[1].rgbGreen + H_FILTER[x][2] * pixels[2].rgbGreen;
}

int filterGreenV(int x, RGBQUAD pixels[])
{
	return V_FILTER[x][0] * pixels[0].rgbGreen + H_FILTER[x][1] * pixels[1].rgbGreen + H_FILTER[x][2] * pixels[2].rgbGreen;
}

int filterBlueH(int x, RGBQUAD pixels[])
{
	return H_FILTER[x][0] * pixels[0].rgbBlue + H_FILTER[x][1] * pixels[1].rgbBlue + H_FILTER[x][2] * pixels[2].rgbBlue;
}

int filterBlueV(int x, RGBQUAD pixels[])
{
	return V_FILTER[x][0] * pixels[0].rgbBlue + H_FILTER[x][1] * pixels[1].rgbBlue + H_FILTER[x][2] * pixels[2].rgbBlue;
}

////////////////////////////////////////////////////////////////////////
static void processSerial(const fipImage& input, fipImage& output)
{
	assert(input.getWidth() == output.getWidth() && input.getHeight() == output.getHeight() && input.getImageSize() ==
		output.getImageSize());
	assert(input.getBitsPerPixel() == 32);

	const int fSize = 3;
	const int fSize2 = fSize / 2;
	const int hFilter[][fSize] = {
		{1, 1, 1},
		{0, 0, 0},
		{-1, -1, -1}
	};
	const int vFilter[][fSize] = {
		{1, 0, -1},
		{1, 0, -1},
		{1, 0, -1}
	};


	for (unsigned int v = fSize2; v < output.getHeight() - fSize2; v++)
	{
		for (unsigned int u = fSize2; u < output.getWidth() - fSize2; u++)
		{
			RGBQUAD iC;
			int hC[3] = {0, 0, 0};
			int vC[3] = {0, 0, 0};

			for (unsigned int j = 0; j < fSize; j++)
			{
				for (unsigned int i = 0; i < fSize; i++)
				{
					input.getPixelColor(u + i - fSize2, v + j - fSize2, &iC);
					hC[0] += hFilter[j][i] * iC.rgbBlue;
					vC[0] += vFilter[j][i] * iC.rgbBlue;
					hC[1] += hFilter[j][i] * iC.rgbGreen;
					vC[1] += vFilter[j][i] * iC.rgbGreen;
					hC[2] += hFilter[j][i] * iC.rgbRed;
					vC[2] += vFilter[j][i] * iC.rgbRed;
				}
			}
			RGBQUAD oC = {dist(hC[0], vC[0]), dist(hC[1], vC[1]), dist(hC[2], vC[2]), 255};
			output.setPixelColor(u, v, &oC);
		}
	}
}


////////////////////////////////////////////////////////////////////////
void applyFilter(const fipImage& input, fipImage& output, const unsigned int u,
                 const unsigned int v);

static void processSerialOpt(const fipImage& input, fipImage& output)
{
	const int bypp = 4;
	assert(input.getWidth() == output.getWidth() && input.getHeight() == output.getHeight() && input.getImageSize() ==
		output.getImageSize());
	assert(input.getBitsPerPixel() == bypp*8);

	const int fSize = 3;
	const int fSize2 = fSize / 2;
	const int hFilter[][fSize] = {
		{1, 1, 1},
		{0, 0, 0},
		{-1, -1, -1}
	};
	const int vFilter[][fSize] = {
		{1, 0, -1},
		{1, 0, -1},
		{1, 0, -1}
	};

	const unsigned int maxu = output.getWidth() - fSize2;
	const unsigned int maxv = output.getHeight() - fSize2;


	for (unsigned int v = fSize2; v < maxv; ++v)
	{
		for (unsigned int u = fSize2; u < maxu; ++u)
		{
			applyFilter(input, output, u, v);
		}
	}
}

void applyFilter(const fipImage& input, fipImage& output, const unsigned int u,
                 const unsigned int v)
{
	int hC[3] = {0, 0, 0};
	int vC[3] = {0, 0, 0};

	//Top Row
	RGBQUAD pixels[3][3];
	input.getPixelColor(u - 1, v - 1, &pixels[0][0]);
	input.getPixelColor(u - 1, v, &pixels[0][1]);
	input.getPixelColor(u - 1, v + 1, &pixels[0][2]);
	//Center row
	input.getPixelColor(u, v - 1, &pixels[1][0]);
	input.getPixelColor(u, v, &pixels[1][1]);
	input.getPixelColor(u, v + 1, &pixels[1][2]);

	//Bottom Row
	input.getPixelColor(u + 1, v - 1, &pixels[2][0]);
	input.getPixelColor(u + 1, v, &pixels[2][1]);
	input.getPixelColor(u + 1, v + 1, &pixels[2][2]);

	hC[0] += filterBlueH(0, pixels[0]) + filterBlueH(1, pixels[1]) + filterBlueH(2, pixels[2]);
	hC[1] += filterGreenH(0, pixels[0]) + filterGreenH(1, pixels[1]) + filterGreenH(2, pixels[2]);
	hC[2] += filterRedH(0, pixels[0]) + filterRedH(1, pixels[1]) + filterRedH(2, pixels[2]);

	vC[0] += filterBlueV(0, pixels[0]) + filterBlueV(1, pixels[1]) + filterBlueV(2, pixels[2]);
	vC[1] += filterGreenV(0, pixels[0]) + filterGreenV(1, pixels[1]) + filterGreenV(2, pixels[2]);
	vC[2] += filterRedV(0, pixels[0]) + filterRedV(1, pixels[1]) + filterRedV(2, pixels[2]);


	RGBQUAD oC = {dist(hC[0], vC[0]), dist(hC[1], vC[1]), dist(hC[2], vC[2]), 255};
	output.setPixelColor(u, v, &oC);
}


////////////////////////////////////////////////////////////////////////
static void processParallel(const fipImage& input, fipImage& output)
{
	const int bypp = 4;


	assert(input.getWidth() == output.getWidth() && input.getHeight() == output.getHeight() && input.getImageSize() ==
		output.getImageSize());
	assert(input.getBitsPerPixel() == 32);
}

////////////////////////////////////////////////////////////////////////
static bool operator==(const fipImage& im1, const fipImage& im2)
{
	assert(im1.getWidth() == im2.getWidth() && im1.getHeight() == im2.getHeight() && im1.getImageSize() == im2.getImageSize
		());
	assert(im1.getBitsPerPixel() == 32);

	for (unsigned int i = 0; i < im1.getHeight(); i++)
	{
		COLORREF* row1 = reinterpret_cast<COLORREF*>(im1.getScanLine(i));
		COLORREF* row2 = reinterpret_cast<COLORREF*>(im2.getScanLine(i));
		for (unsigned int j = 0; j < im1.getWidth(); j++)
		{
			if (row1[j] != row2[j]) return false;
		}
	}
	return true;
}

static bool operator!=(const fipImage& im1, const fipImage& im2)
{
	return !(im1 == im2);
}

////////////////////////////////////////////////////////////////////////
int imageProcessing(int argc, const char* argv[])
{
	cout << "Image processing started" << endl;
	if (argc < 3)
	{
		cerr << "Usage: " << argv[0] << " input-file-name output-file-name" << endl;
		return -1;
	}

	Stopwatch sw;
	fipImage image;

	// load image
	if (!image.load(argv[1]))
	{
		cerr << "Image not found: " << argv[1] << endl;
		return -1;
	}

	// create output images
	fipImage out1(image), out2(image), out3(image);

	// process image sequentially and produce out1
	cout << "Start sequential process" << endl;
	sw.Start();
	processSerial(image, out1);
	sw.Stop();
	double seqTime = sw.GetElapsedTimeMilliseconds();
	cout << seqTime << " ms" << endl;

	// process image sequentially but optimized and produce out2
	cout << "Start optimized sequential process" << endl;
	sw.Start();
	processSerialOpt(image, out2);
	sw.Stop();
	double seqOptTime = sw.GetElapsedTimeMilliseconds();
	cout << seqOptTime << " ms, speedup = " << seqTime / seqOptTime << endl;

	// compare out1 with out2
	cout << boolalpha << "The two operations produce the same results: " << (out1 == out2) << endl;

	// process image in parallel and produce out3
	cout << "Start parallel process" << endl;
	sw.Start();
	processParallel(image, out3);
	sw.Stop();
	cout << sw.GetElapsedTimeMilliseconds() << " ms, speedup = " << seqOptTime / sw.GetElapsedTimeMilliseconds() << endl;

	// compare out1 with out3
	cout << boolalpha << "The two operations produce the same results: " << (out1 == out3) << endl;

	// save output image
	if (!out1.save(argv[2]))
	{
		cerr << "Image not saved: " << argv[2] << endl;
		return -1;
	}

	// save output image
	if (!out2.save(argv[3]))
	{
		cerr << "Image not saved: " << argv[3] << endl;
		return -1;
	}

	return 0;
}
