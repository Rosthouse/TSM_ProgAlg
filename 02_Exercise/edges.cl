////////////////////////////////////////////////////////////////////////
uint dist(int x, int y) {
	uint d = (uint)sqrt((float)(x*x) + (float)(y*y));
	return (d < 256) ? d : 255;
}

////////////////////////////////////////////////////////////////////////
// OpenCL kernel
///
__kernel void edges(__read_only image2d_t source, __write_only image2d_t dest, __constant int* hFilter, __constant int* vFilter, int fSize, sampler_t sampler) {
	const int w = get_global_size(0);
	const int h = get_global_size(1);

	const int2 pos = (int2)(get_global_id(0), get_global_id(1) );

	uint4 pixelAtPos = read_imageui(source, sampler, pos);

	//const float4 color = (float4)(0.5, 0.5, 0.5, 1.0);
	const uint4 color = (uint4)(255 - pixelAtPos.x, 255  - pixelAtPos.y, 255 - pixelAtPos.z, pixelAtPos.w);

	write_imageui(dest, pos, color);
	//write_imageui(dest, pos, { 1.0 - pixelAtPos.r, 1.0 - pixelAtPos.g, 1.0 - pixelAtPos.b, 1.0 - pixelAtPos.a});
}

