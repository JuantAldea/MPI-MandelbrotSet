/*
 * main.cu
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include <cuda.h>
#include <stdio.h>

#define pix_per_thread 1
#define DEBUG

extern "C" {
#include "ppm.h"
}

__global__ void render(unsigned char *out, int width, int height, int max_iterations) {
	unsigned int x_dim = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y_dim = blockIdx.y * blockDim.y + threadIdx.y;
	int index = 3 * width * y_dim + x_dim * 3;
	float x_origin = ((float) x_dim / width) * 3.25 - 2;
	float y_origin = ((float) y_dim / height) * 2.5 - 1.25;
	float x = 0.0;
	float y = 0.0;

	int iteration = 0;
	while (x * x + y * y <= 4 && iteration < max_iterations) {
		float xtemp = x * x - y * y + x_origin;
		y = 2 * x * y + y_origin;
		x = xtemp;
		iteration++;
	}
	//out[index]++;
	if (iteration == max_iterations) {
		out[index + 0] = 0;
		out[index + 1] = 0;
		out[index + 2] = 0;
	} else {
		out[index + 0] = iteration < 255 ? iteration : 255;
		out[index + 1] = iteration < 255 ? iteration : 255;
		out[index + 2] = iteration < 255 ? iteration : 255;
	}
}

void runCUDA(int width, int height, int max_iterations) {
	size_t buffer_size = sizeof(unsigned char) * width * height * 3;
	unsigned char *device_memory, *host_memory;
	dim3 blockDim(16, 16, 1);
	dim3 gridDim(width / (blockDim.x), height / (2 * blockDim.y), 1);
	cudaError_t cuda_error;
	cudaDeviceReset();
	cuda_error = cudaSetDeviceFlags(cudaDeviceMapHost);
	printf("Set device: %s\n", cudaGetErrorString(cuda_error));

	int host_alloc = 1;

	if (cuda_error == cudaSuccess && host_alloc) {
		cuda_error = cudaHostAlloc((void**) &host_memory, buffer_size,
				cudaHostAllocMapped);
		printf("Host1 %s\n", cudaGetErrorString(cuda_error));
		cuda_error = cudaHostGetDevicePointer(&device_memory, host_memory, 0);
		printf("Host2 %s\n", cudaGetErrorString(cuda_error));
#ifdef DEBUG
		cuda_error = cudaMemset(device_memory, 255, buffer_size);
		printf("Host3 %s\n", cudaGetErrorString(cuda_error));
#endif
	} else {
		cuda_error = cudaMalloc((void **) &device_memory, buffer_size);
		printf("Device %s\n", cudaGetErrorString(cuda_error));
#ifdef DEBUG
		cuda_error = cudaMemset(device_memory, 255, buffer_size);
		printf("Device %s\n", cudaGetErrorString(cuda_error));
#endif
		host_memory = (unsigned char *) malloc(buffer_size);
	}

	/************************************************************************/
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	render<<< gridDim, blockDim, 0 >>>(device_memory, width, height, max_iterations);

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	/*****************************************************************************/

	if (cuda_error == cudaSuccess && host_alloc) {
		cuda_error = cudaThreadSynchronize();
		printf("Host barrier %s\n", cudaGetErrorString(cuda_error));
	} else {
		cuda_error = cudaMemcpy(host_memory, device_memory, buffer_size, cudaMemcpyDeviceToHost);
		printf("Device %s\n", cudaGetErrorString(cuda_error));
	}

	char path[100];
	sprintf(path, "cuda_%d_%d.ppm", height, max_iterations);
	write_ppm("path", height, width, 255, host_memory);

	if (cuda_error == cudaSuccess && host_alloc) {
		cuda_error = cudaFreeHost(host_memory);
		printf("Host %s\n", cudaGetErrorString(cuda_error));
	} else {
		cuda_error = cudaFree(device_memory);
		printf("%s\n", cudaGetErrorString(cuda_error));
		free(host_memory);
	}
}

int main(int argc, const char * argv[]) {
	int dim = atoi(argv[1]);
	int max_iterations = atoi(argv[2]);
	runCUDA(dim, dim, max_iterations);
	return 0;
}
