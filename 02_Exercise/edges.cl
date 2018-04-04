////////////////////////////////////////////////////////////////////////
uint dist(int x, int y) {
	uint d = (uint)sqrt((float)(x*x) + (float)(y*y));
	return (d < 256) ? d : 255;
}

////////////////////////////////////////////////////////////////////////
// OpenCL kernel
///
__kernel void edges(__read_only image2d_t source, __write_only image2d_t dest, __constant int* hFilter, __constant int* vFilter, int fSize, sampler_t sampler) {
	//const int src_image_width = get_global_size(0);
	//const int src_image_height = get_global_size(1);
	const int src_image_width = get_image_width(source);
	const int src_image_height = get_image_height(source);

	const int2 pos = (int2)(get_global_id(0), get_global_id(1) );

	const int fSizeD2 = fSize/2;

	if(pos.x <= fSizeD2 || pos.x >= (src_image_width - fSizeD2)){ 
		return;
	}
	if(pos.y <= fSizeD2 || pos.y >= (src_image_width - fSizeD2)){ 
		return;
	}

	uint4 pixelAtPos = read_imageui(source, sampler, pos);
	//const float4 color = (float4)(0.5, 0.5, 0.5, 1.0);
	
	uint4 hC = 0;
	uint4 vC = 0;
	int fi = 0;
	
    for(int j = 0; j < fSize; j++) {
		for(int i = 0; i < fSize; i++) {
			uint4 iC = read_imageui(source, sampler, (int2)(pos.x + (i - fSizeD2), pos.y + (j - fSizeD2)));
			int f = hFilter[fi];
			hC.x += f * iC.x;
			hC.y += f * iC.y;
			hC.z += f * iC.z;
			f = vFilter[fi];
			vC.x += f * iC.x;
			vC.y += f * iC.y;
			vC.z += f * iC.z;
			fi++;
		}
    }

	const uint4 outColor = (uint4)(dist(hC.x, vC.x), dist(hC.y, vC.y), dist(hC.z, vC.z), 255);
	//const uint4 outColor = (uint4)(255 - pixelAtPos.x, 255  - pixelAtPos.y, 255 - pixelAtPos.z, pixelAtPos.w);

	write_imageui(dest, pos, outColor);
}

