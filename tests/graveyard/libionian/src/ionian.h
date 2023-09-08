#ifndef IONIAN_H
#define IONIAN_H
#include <iostream>

__device__
void ionian_add(double &d);

__global__
void ionian_setCoef(double d);

void ionian_initialize(double **d_arr, const int size, double coeff);

#define HIP_CHECK(command) {   \
hipError_t status = command;   \
if (status!=hipSuccess) {      \
std::cerr << "Error: HIP reports " << hipGetErrorString(status) << std::endl; \
std::abort(); } }

#endif // IONIAN_H
