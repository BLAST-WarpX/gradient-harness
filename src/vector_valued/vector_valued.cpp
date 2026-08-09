/**
 * Example of differentiating a vector-valued function (i.e. a function with array output).
 */
#include <stdlib.h>
#include <iostream>
#include <sstream>

extern int enzyme_const, enzyme_out, enzyme_dup;

template <typename RT, typename... T>
RT __enzyme_autodiff(void*, T...);

template <typename RT, typename... T>
RT __enzyme_fwddiff(void*, T...);

/**
 * Pointwise multiply arrays x and y, store results in z
 */
void pointwiseMult(int n, double * x, double * y, double * z) {
    for (int i = 0; i < n; i++) {
        z[i] = x[i] * y[i];
    }
}

/**
 * Pointwise multiply arrays x and y, stores result in y
 */ 
void pointwiseMultStoreY(int n, double * x, double * y) {
    for (int i = 0; i < n; i++) {
        y[i] = x[i] * y[i];
    }
}

std::string arrayToString(int n, double * arr) {
    std::stringstream ss;
    ss << "{ ";
    for (int i = 0; i <= n - 2; i++) {
        ss << arr[i] << ", "; 
    }
    ss << arr[n-1] << " }";
    return ss.str();
}

int main() {
    int n = 3;
    double *x = (double*) malloc(n*sizeof(double));
    double *y = (double*) malloc(n*sizeof(double));
    double *z = (double*) malloc(n*sizeof(double));
    
    // Shadow variables
    double *dx = (double*) malloc(n*sizeof(double));
    double *dy = (double*) malloc(n*sizeof(double));
    double *dz = (double*) malloc(n*sizeof(double));

    // Below let f = pointwiseMult
    // Since pointwiseMult has inputs x and y, and stores
    // the result of the operation in z, let's write z = f(x, y)
    //
    // Write the combined input vector as mu = [x, y]
    //
    // Let Jf[i][j] = d(z[i])/d(mu[j]) be the Jacobian matrix of f
    // 
    // For each of the array parameters x, y, z to the function pointwiseMult,
    // we must define "shadow" variables dx, dy, dz. These are simply variables
    // that match the structure of x, y, z. 
    // E.g. in this case if x is a double[n], dx must be a double[n]

    ////////// Forward mode //////////
    // For Enzyme forward mode (fwddiff), our inputs are the shadow arrays dx and dy.
    //
    // Let dmu = [dx, dy] be the combined input array
    // 
    // Forward mode computes the matrix-vector product: dz = Jf * dmu
    // i.e. dz[i] = sum_j d(z[i])/d(mu[j]) * dmu[j]
    // In other words, Enzyme computes the directional derivative of z in the
    // direction dmu.
    //
    // Note the difference here between dx, dy, dz, which are user defined arrays,
    // and d(z[i])/d(mu[j]), which is a derivative of an element of the vector z,
    // viewing z as a function of x and y, z = f(x, y).
    
    x[0] = 1.0; x[1] = 2.0; x[2] = 3.0;
    y[0] = 4.0; y[1] = 5.0; y[2] = 6.0;
    
    // Zero initialize output array z
    // Note Enzyme will store the output of pointwiseMult here when we 
    // call __enzyme_*diff, unless we label z as enzyme_dupnoneed
    z[0] = 0.0; z[1] = 0.0; z[2] = 0.0;
    
    // Initialize fwddiff inputs dmu = [dx, dy]
    dx[0] = 1.0; dx[1] = 0.0; dx[2] = 0.0;
    dy[0] = 0.0; dy[1] = 0.0; dy[2] = 0.0;

    // Zero initialize fwddiff output dz
    dz[0] = 0.0; dz[1] = 0.0; dz[2] = 0.0;

    std::cout << "#### Forward mode pointwiseMult ####\n";
    std::cout << "x = " << arrayToString(n, x) << "\n"; 
    std::cout << "y = " << arrayToString(n, y) << "\n";
    std::cout << "z = " << arrayToString(n, z) << "\n";
    std::cout << "dx_in = " << arrayToString(n, dx) << "\n";
    std::cout << "dy_in = " << arrayToString(n, dy) << "\n";
    std::cout << "dz_in = " << arrayToString(n, dz) << "\n";
    
    __enzyme_fwddiff<void>((void*)pointwiseMult, 
            enzyme_const, n, enzyme_dup, x, dx, enzyme_dup, y, dy, enzyme_dup, z, dz);

    std::cout << "\n";
    std::cout << "z_out = " << arrayToString(n, z) << "\n";
    std::cout << "dx_out = " << arrayToString(n, dx) << "\n";
    std::cout << "dy_out = " << arrayToString(n, dy) << "\n";
    std::cout << "dz_out = " << arrayToString(n, dz) << "\n";
    std::cout << "\n";

    ////////// Reverse mode //////////
    // For Enzyme reverse mode (autodiff), the input/output convention is reversed.
    // The input array is dz, and Enzyme computes dmu = Jf^t * dz
    // i.e. dx[i] = sum_j d(z[j])/d(x[i]) * dz[j]
    //      dy[i] = sum_j d(z[j])/d(x[i]) * dz[j]
    //
    // Rather than the directional derivative we get in the forward mode case,
    // reverse mode gives us an arbitrary linear combination of gradients of the
    // output functions z[i], weighted by the inputted array dz.
    // dx = sum_j (grad_x z[j]) * dz[j] 
    // dy = sum_j (grad_y z[j]) * dz[j]
    // 
    // Note that if we let dz = e_i (the unit vector in the ith direction),
    // then Enzyme computes dx = grad_x(z[i]), dy = grad_y(z[i]),
    // so in this way we can calculate the gradient of each output z[i] individually.
    x[0] = 1.0; x[1] = 2.0; x[2] = 3.0;
    y[0] = 4.0; y[1] = 5.0; y[2] = 6.0;
    z[0] = 0.0; z[1] = 0.0; z[2] = 0.0;
    
    // Zero initialize reverse mode output array dmu = [dx, dy]
    dx[0] = 0.0; dx[1] = 0.0; dx[2] = 0.0;
    dy[0] = 0.0; dy[1] = 0.0; dy[2] = 0.0;
    
    // Initialize reverse mode input array: dz = [1, 0, 0]
    // This tells Enzyme to compute the gradient of z[0] w.r.t x and y
    dz[0] = 1.0; dz[1] = 0.0; dz[2] = 0.0;

    std::cout << "#### Reverse mode pointwiseMult ####\n";
    std::cout << "x = " << arrayToString(n, x) << "\n"; 
    std::cout << "y = " << arrayToString(n, y) << "\n";
    std::cout << "z = " << arrayToString(n, z) << "\n";
    std::cout << "dx_in = " << arrayToString(n, dx) << "\n";
    std::cout << "dy_in = " << arrayToString(n, dy) << "\n";
    std::cout << "dz_in = " << arrayToString(n, dz) << "\n";
    
    __enzyme_autodiff<void>((void*)pointwiseMult, 
            enzyme_const, n, enzyme_dup, x, dx, enzyme_dup, y, dy, enzyme_dup, z, dz);

    std::cout << "\n";
    std::cout << "z_out = " << arrayToString(n, z) << "\n";
    std::cout << "dx_out = " << arrayToString(n, dx) << "\n";
    std::cout << "dy_out = " << arrayToString(n, dy) << "\n";
    std::cout << "dz_out= " << arrayToString(n, dz) << "\n";

    std::cout << "\n";

    // Note: forward and backward mode can also be used on vector-values
    // functions that store a result in one of the input variables
    // 
    // pointwiseMultStoreY(x, y) pointwise multiplies x and y and stores the result in y.
    //
    // For forward mode, we again initialize the input array dmu = [dx, dy], but in this case
    // Enzyme uses dy as the output array as well. So our input values are overwritten with
    // the desired derivatives.
    x[0] = 1.0; x[1] = 2.0; x[2] = 3.0;
    y[0] = 4.0; y[1] = 5.0; y[2] = 6.0;
    
    // Initialize fwddiff inputs dmu = [dx, dy]
    dx[0] = 1.0; dx[1] = 0.0; dx[2] = 0.0;
    dy[0] = 0.0; dy[1] = 0.0; dy[2] = 0.0;  // Enzyme will overwrite dy for pointwiseMultStoreY

    std::cout << "#### Forward mode pointwiseMultStoreY ####\n";
    std::cout << "x = " << arrayToString(n, x) << "\n"; 
    std::cout << "y = " << arrayToString(n, y) << "\n";
    std::cout << "dx_in = " << arrayToString(n, dx) << "\n";
    std::cout << "dy_in = " << arrayToString(n, dy) << "\n";
    
    __enzyme_fwddiff<void>((void*)pointwiseMultStoreY, 
            enzyme_const, n, enzyme_dup, x, dx, enzyme_dup, y, dy);

    std::cout << "\n";
    std::cout << "dx_out = " << arrayToString(n, dx) << "\n";
    std::cout << "dy_out = " << arrayToString(n, dy) << "\n";
    std::cout << "\n";

    return 0;
}

