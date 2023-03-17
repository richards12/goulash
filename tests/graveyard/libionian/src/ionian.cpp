#ifndef IONIAN_CPP
#define IONIAN_CPP
#include "ionian.h"
#include "hip/hip_runtime.h"

__device__ double s;

__device__
void libh_add(double &d)
{ 
  d+=s;
}

__global__
void libh_setCoef(double d)
{
  s = d;
}

void libh_initialize(double **d_arr, const int size, double coeff)
{  
   //allocate d_arr on the device
   hipMalloc(d_arr,size*sizeof(double));
   
   //initialize s on the deivce
   hipLaunchKernelGGL(libh_setCoef,1,1,0,0,coeff);
   hipDeviceSynchronize();
}


#endif // IONIAN_CPP
