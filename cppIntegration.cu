#include <stdio.h>
#include <iostream>
#include <cuda_runtime.h>
#include <helper_cuda.h>
#include "template.cuh"
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

extern int res;

__device__ float map(const float& x, float x11, float x12, float x21, float x22) {
	float a = x21 + ((x22 - x21) / (x12 - x11)) * (x - x11);
	return a;
}

__device__ float2 test(float x, float y, float vx, float vy, float3& Px, float3& Py) {
	float2 a; float A, B, C, b, c, h, m, M, g, L; float sum_x; float sum_y;
	
	b = 0.1; m = 30.0; 
	g = 981.0 ; L = 17.0;

	c = 3.3746; h = sqrt(.25);
	M = 1.0;

	A = 3.3746;
	B = (3/2)*(b/m);
	C = (3/2)*(g/L);

	float& Px1 = Px.x; float& Px2 = Px.y; float& Px3 = Px.z;
	float& Py1 = Py.x; float& Py2 = Py.y; float& Py3 = Py.z;

	float s1_x, s2_x, s3_x;
	float s1_y, s2_y, s3_y;

	float r1_x, r2_x, r3_x;
	float r1_y, r2_y, r3_y;

	s1_x = Px1 - x;
	s2_x = Px2 - x;
	s3_x = Px3 - x;

	s1_y = Py1 - y;
	s2_y = Py2 - y;
	s3_y = Py3 - y;

	r1_x = s1_x / powf(powf(s1_x, 2.0) + powf(s1_y, 2.0) + powf(h, 2.0), 5.0 / 2.0);
	r2_x = s2_x / powf(powf(s2_x, 2.0) + powf(s2_y, 2.0) + powf(h, 2.0), 5.0 / 2.0);
	r3_x = s3_x / powf(powf(s3_x, 2.0) + powf(s3_y, 2.0) + powf(h, 2.0), 5.0 / 2.0);

	r1_y = s1_y / powf(powf(s1_x, 2.0) + powf(s1_y, 2.0) + powf(h, 2.0), 5.0 / 2.0);
	r2_y = s2_y / powf(powf(s2_x, 2.0) + powf(s2_y, 2.0) + powf(h, 2.0), 5.0 / 2.0);
	r3_y = s3_y / powf(powf(s3_x, 2.0) + powf(s3_y, 2.0) + powf(h, 2.0), 5.0 / 2.0);

	sum_x = r1_x + r2_x + r3_x;
	sum_y = r1_y + r2_y + r3_y;

	a.x = A * sum_x - B * vx - C * x;
	a.y = A * sum_y - B * vy - C * y;

	return a;
}

__device__ float path(float& x0, float& y0, float t, int n, float3& Px, float3& Py) {
	float2 p1; float2 r1;
	float2 p2; float2 r2;

	p1.x = x0; p1.y = y0;
	r1.x = 0; r1.y = 0;

	float dt = t / n;

	for (int i = 0; i < n; i++) {
		float dt2 = powf(dt, 2.0);

		if (i == 0) {
			float2& x2 = p1; float2& v2 = r1;
			float2 x3; float2 v3;
			float2 a2 = test(x2.x, x2.y, v2.x, v2.y, Px, Py);

			x3.x = x2.x + v2.x * dt + (1.0 / 2.0)* a2.x *dt2;
			x3.y = x2.y + v2.y * dt + (1.0 / 2.0)* a2.y *dt2;

			v3.x = v2.x + a2.x * dt;
			v3.y = v2.y + a2.y * dt;

			p2 = x3;
			r2 = v3;

			continue;
		}
		float2& x1 = p1; float2& x2 = p2; float2 x3;
		float2& v1 = r1; float2& v2 = r2; float2 v3; float2 v4;
		float2 a1; float2 a2; float2 a3;
		
		a1 = test(x1.x, x1.y, v1.x, v1.y, Px, Py);
		a2 = test(x2.x, x2.y, v2.x, v2.y, Px, Py);

		x3.x = x2.x + v2.x*dt + (2.0 / 3.0)* a2.x *dt2 - (1.0 / 6.0)* a1.x *dt2;
		x3.y = x2.y + v2.y*dt + (2.0 / 3.0)* a2.y *dt2 - (1.0 / 6.0)* a1.y *dt2;

		v3.x = v2.x + (3.0 / 2.0)* a2.x *dt - (1.0 / 2.0)* a1.x *dt;
		v3.y = v2.y + (3.0 / 2.0)* a2.y *dt - (1.0 / 2.0)* a1.y *dt;

		a3 = test(x3.x, x3.y, v3.x, v3.y, Px, Py);
		
		v4.x = v2.x + (5.0 / 12.0)* a3.x *dt + (2.0 / 3.0)* a2.x *dt - (1.0 / 12.0)* a1.x *dt;
		v4.y = v2.y + (5.0 / 12.0)* a3.y *dt + (2.0 / 3.0)* a2.y *dt - (1.0 / 12.0)* a1.y *dt;

		p1 = p2;
		r1 = r2;

		p2 = x3;
		r2 = v4;
	}

	float rx_1, rx_2, rx_3;
	rx_1 = p2.x - Px.x;
	rx_2 = p2.x - Px.y;
	rx_3 = p2.x - Px.z;

	float ry_1, ry_2, ry_3;
	ry_1 = p2.y - Py.x;
	ry_2 = p2.y - Py.y;
	ry_3 = p2.y - Py.z;

	float dis[3];
	dis[0] = powf(powf(rx_1, 2.0) + powf(ry_1, 2.0), 1.0 / 2.0);
	dis[1] = powf(powf(rx_2, 2.0) + powf(ry_2, 2.0), 1.0 / 2.0);
	dis[2] = powf(powf(rx_3, 2.0) + powf(ry_3, 2.0), 1.0 / 2.0);

	float dmin = 10000;
	float index;

	for (int i = 0; i < 3; i++)
	{
		if (dis[i] < dmin) {
			dmin = dis[i];
			index = i*1.0;
		}
	}
	return index;
}

__global__ void vectorAdd(float *A, int numElements)
{
	__shared__ float3 Px, Py;
	float pi = 3.141592;
	Px.x = 1*cosf(pi/3); Px.y = 1*cosf(pi); Px.z = 1*cosf(-pi/3);
	Py.x = 1*sinf(pi/3); Py.y = 1*sinf(pi); Py.z = 1*sinf(-pi/3);

	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < numElements)
	{
		float s1 = sqrtf(numElements);
		int off = i / s1; float x = i - off * s1; float y = off;
		float a, b;
		a = map(x, 0.0, s1, -2.5, 2.5);
		b = map(y, 0.0, s1, -2.5, 2.5);
		float v;
		v = path(a, b, 10.0, 1000, Px, Py);
		A[i*3] = a; A[i*3 + 1] = b; A[i*3 + 2] = v;
	}
}

extern "C" float* runTest()
{
	cudaError_t err = cudaSuccess;
	int numElements = pow(res, 2.0);
	size_t size = 3 * numElements * sizeof(float);

	float *h_A = (float *)malloc(size);
	float *d_A = NULL;
	err = cudaMalloc((void **)&d_A, size);

	int threadsPerBlock = 500;
	int blocksPerGrid = (numElements + threadsPerBlock) / threadsPerBlock;

	vectorAdd << <blocksPerGrid, threadsPerBlock >> > (d_A, numElements);
	err = cudaGetLastError();

	err = cudaMemcpy(h_A, d_A, size, cudaMemcpyDeviceToHost);
	err = cudaFree(d_A);
	return h_A;
}
