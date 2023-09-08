#ifndef IONIAN_H
#define IONIAN_H
__device__
void ionian_add(double &d);

__global__
void ionian_setCoef(double d);

void ionian_initialize(double **d_arr, const int size, double coeff);
#endif // IONIAN_H
