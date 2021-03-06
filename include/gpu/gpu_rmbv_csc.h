#ifndef GPU_RMBV_CSC_H
#define GPU_RMBV_CSC_H
#include "types.h"
#include "properties.h"
#include "gpu_kernel_utils.h"
void drmbv_csc(DSPMAT* mat, double* x, int n,  double* y, int m, int* ierr);
#endif
