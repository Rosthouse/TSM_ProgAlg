////////////////////////////////////////////////////////////////////////
uint dist(int x, int y) {
	uint d = (uint)sqrt((float)(x*x) + (float)(y*y));
	return (d < 256) ? d : 255;
}

////////////////////////////////////////////////////////////////////////
// OpenCL kernel
__kernel void edges(__read_only image2d_t source, __write_only image2d_t dest, __constant int* hFilter, __constant int* vFilter, int fSize, sampler_t sampler) {
	const int w = get_global_size(0);
	const int h = get_global_size(1);

	// TODO implement edge detection without using local memory
	// use read_imageui(...) and write_imageui(...) to read/write one pixel of source/dest

	/*BYTE *iCenter = input.getScanLine(v) + bypp*fSizeD2;
		BYTE *oPos = output.getScanLine(v) + bypp*fSizeD2;

		for(size_t u = fSizeD2; u < output.getWidth() - fSizeD2; u++) {
			int hC[3] = { 0, 0, 0 };
			int vC[3] = { 0, 0, 0 };
			int fi = 0;
			BYTE *iPos = iCenter - fSizeD2*stride - bypp*fSizeD2;

			for(int j = 0; j < fSize; j++) {
				for(int i = 0; i < fSize; i++) {
					const RGBQUAD *iC = reinterpret_cast<RGBQUAD*>(iPos);
					int f = hFilter[fi];

					hC[0] += f*iC->rgbBlue;
					hC[1] += f*iC->rgbGreen;
					hC[2] += f*iC->rgbRed;
					f = vFilter[fi];
					vC[0] += f*iC->rgbBlue;
					vC[1] += f*iC->rgbGreen;
					vC[2] += f*iC->rgbRed;
					iPos += bypp;
					fi++;
				}
				iPos += stride - bypp*fSize;
			}
			RGBQUAD *oC = reinterpret_cast<RGBQUAD*>(oPos);
			oC->rgbBlue = dist(hC[0], vC[0]);
			oC->rgbGreen = dist(hC[1], vC[1]);
			oC->rgbRed = dist(hC[2], vC[2]);
			oC->rgbReserved = 255;
			iCenter += bypp;
			oPos += bypp;
		}*/
}

