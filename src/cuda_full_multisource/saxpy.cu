__device__
void saxpyImpl(int n, float * a, float * x, float * y) {
    int i = threadIdx.x + blockDim.x * blockIdx.x;

    if (i < n) {
        y[i] = (*a)*x[i] + y[i];
    }
}

__global__
void saxpy(int n, float * a, float * x, float * y) {
    saxpyImpl(n, a, x, y);
}

