#include <vector>
#include <string>
#include <iostream>
#include "assert.h"
#include "hip/hip_runtime.h"

#ifdef ENABLE_FLIB
#include "foo_mod.h"
#endif

#ifdef ENABLE_HLIB
#include "suskevi.h"

__global__
void launchKernel(double *arr,const int size)
{
  int i = threadIdx.x + blockIdx.x*blockDim.x;
  if(i < size)
    libh_add(arr[i]);
}
#endif

int main(int argc, char** argv)
{
   double coef1 = std::stod(argv[1]);
   double coef2 = std::stod(argv[2]);
   double referenceRes = 1.0;


   int n = 1000;
   std::vector<double> aa(n, 1.0);
   


#ifdef ENABLE_FLIB
   referenceRes *= coef1;
   libf_setCoef(coef1);
   libf_mult(&(aa[0]), (int)aa.size());
#endif // ENABLE_FLIB

#ifdef ENABLE_HLIB
   referenceRes += coef2;
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
#endif // ENABLE_HLIB
   

// Verification 
   for (int ii=0; ii<aa.size(); ++ii)
   {   
    // assert(aa[ii] == referenceRes);
    if(aa[ii]!=referenceRes)
    {
      std::cout<<"Verification Erro: aa[ii]!=referenceRes | aa[ii]="<<aa[ii]<<" referenceRes="<<referenceRes<<std::endl;
      assert(aa[ii] == referenceRes);
    }
   }

   printf("Verification Passed!\n");
   fflush(stdout);
}
