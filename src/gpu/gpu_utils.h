#ifndef GPU_UTILS_H
#define GPU_UTILS_H

bool checkGPUSupport();
void* allocateMemory(size_t size);
void handleError(const char* errorMessage);

#endif // GPU_UTILS_H