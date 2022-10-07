#include <vector>
#include <string>
#include "assert.h"
#include "hip/hip_runtime.h"

#include "foo_mod.h"

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

__global__
void launchKernel(double *arr,const int size)
{
  int i = threadIdx.x + blockIdx.x*blockDim.x;
  if(i < size)
    libh_add(arr[i]);
}

int main(int argc, char** argv)
{
   double coef1 = std::stod(argv[1]);
   double coef2 = std::stod(argv[2]);

   int n = 1000;
   std::vector<double> aa(n, 1.0);
   

   libf_setCoef(coef1);
   libf_mult(&(aa[0]), (int)aa.size());

   double *d_aa=NULL;
   libh_initialize(&d_aa,n,coef2);

    if(d_aa==NULL)
    {
      printf("d_aa=NULL!\n");
      fflush(stdout);
      abort();
    }
   
   //copy aa to d_aa on the device
   hipMemcpy(d_aa,&(aa[0]),n*sizeof(double), hipMemcpyHostToDevice);
   hipDeviceSynchronize();
   hipLaunchKernelGGL(launchKernel,n,1,0,0,d_aa, (int)aa.size());
   hipDeviceSynchronize();
   hipMemcpy(&(aa[0]),d_aa,n*sizeof(double), hipMemcpyDeviceToHost);
   hipDeviceSynchronize();
   

// Verification 
   for (int ii=0; ii<aa.size(); ++ii)
    assert(aa[ii] == coef1+coef2);

   printf("Verification Passed!\n");
   fflush(stdout);
}
