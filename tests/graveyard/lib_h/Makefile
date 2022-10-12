CC ?= CC
CXX_FLAGS ?= -O3
HIP_FLAGS ?= -I${ROCM_PATH}/include -fgpu-rdc --rocm-path=${ROCM_PATH} --offload-arch=gfx90a -x hip


libhlib.a: suskevi.o
	ar rcs $@ $^

suskevi.o: suskevi.cpp
	$(CC) $(CXX_FLAGS) $(HIP_FLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf libhlib.a suskevi.o
