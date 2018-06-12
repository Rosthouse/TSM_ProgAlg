///////////////////////////////////////////////////////////////////////////////
__kernel void matrixmult(const __global int *a, const __global int *b, __global int *c, int n) {
	
   // 2D Thread ID
   int tx = get_local_id(0);
   int ty = get_local_id(1);
 
   // value stores the element 
   // that is computed by the thread
   float value = 0;
   for (int k = 0; k < n; ++k)
   {
      float elementA = a[ty * n + k];
      float elementB = b[k * n + tx];
      value += elementA * elementB;
   }
 
   // Write the matrix to device memory each 
   // thread writes one element
   c[ty * n + tx] = value;
}

