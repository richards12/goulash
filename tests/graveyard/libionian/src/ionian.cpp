#ifndef IONIAN_CPP
#define IONIAN_CPP
#include "ionian.h"
#include "hip/hip_runtime.h"

__device__ double s;

__device__
void ionian_add(double &d)
{ 
  d+=s;
}

__global__
void ionian_setCoef(double d)
{
  s = d;
}

void ionian_initialize(double **d_arr, const int size, double coeff)
{  
   //allocate d_arr on the device
   HIP_CHECK(hipMalloc(d_arr,size*sizeof(double)));
   
   //initialize s on the deivce
   hipLaunchKernelGGL(ionian_setCoef,1,1,0,0,coeff);
   HIP_CHECK(hipDeviceSynchronize());
}


#endif // IONIAN_CPP
