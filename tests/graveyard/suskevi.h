#ifndef SUSKEVI_H
#define SUSKEVI_H
__device__
void libh_add(double &d);

__global__
void libh_setCoef(double d);

void libh_initialize(double **d_arr, const int size, double coeff);
#endif // SUSKEVI_H
