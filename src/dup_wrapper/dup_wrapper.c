/**
 * Example code showing how wrapper functions can be used to let Enzyme
 * use duplicate arguments. Duplicate arguments can only be used for inputs
 * passed by reference.
 */
#include <stdio.h>
#include <stdlib.h>

#include "dup_wrapper.h"

int enzyme_dup, enzyme_out, enzyme_const;
double __enzyme_autodiff(void*, ...);

double mul(double a, double b, double c) {
    return a*b*c;
}

double mul_wrapper(double* a_ptr, double* b_ptr, double* c_ptr) {
    return mul(*a_ptr, *b_ptr, *c_ptr);
}

/**
 * Returns distance between points p1 and p2, squared.
 */
double distance_squared(struct Point2D p1, struct Point2D p2) {
    double delta_x = p2.x - p1.x;
    double delta_y = p2.y - p1.y;
    return delta_x*delta_x + delta_y*delta_y;
}

double distance_squared_wrapper(struct Point2D* p1_ptr, struct Point2D* p2_ptr) {
    return distance_squared(*p1_ptr, *p2_ptr);
}

int main() {
    // Test differentiating mul_wrapper function. Using the wrapper, we can
    // store the derivatives with respect to the inputs a, b, c in pointers.
    double* a_ptr = malloc(sizeof(double));
    double* b_ptr = malloc(sizeof(double));
    double* c_ptr = malloc(sizeof(double));

    // Initialize inputs
    *a_ptr = 2.0;
    *b_ptr = 3.0;
    *c_ptr = 4.0;

    double* d_a_ptr = malloc(sizeof(double));
    double* d_b_ptr = malloc(sizeof(double));
    double* d_c_ptr = malloc(sizeof(double));

    // Zero initialize shadow variables
    *d_a_ptr = 0.0;
    *d_b_ptr = 0.0;
    *d_c_ptr = 0.0;

    __enzyme_autodiff((void*)mul_wrapper,
            enzyme_dup, a_ptr, d_a_ptr,
            enzyme_dup, b_ptr, d_b_ptr,
            enzyme_dup, c_ptr, d_c_ptr);

    // We expect d(mul)/da = b*c, d(mul)/db = a*c, d(mul)/dc = a*b
    printf("*** mul_wrapper test ***\n");
    printf("*a_ptr = %f; *b_ptr = %f; *c_ptr = %f\n", *a_ptr, *b_ptr, *c_ptr);
    printf("*d_a_ptr = %f; *d_b_ptr = %f; *d_c_ptr = %f\n", *d_a_ptr, *d_b_ptr, *d_c_ptr);

    printf("\n");

    // Test differentiating distance_squared_wrapper. The wrapper lets us
    // store the derivative with respect to the struct inputs in pointers
    struct Point2D *p1_ptr = malloc(sizeof(struct Point2D));
    struct Point2D *p2_ptr = malloc(sizeof(struct Point2D));

    p1_ptr->x = 2.0; p1_ptr->y = 3.0;
    p2_ptr->x = 1.0; p2_ptr->y = 1.0;

    struct Point2D *d_p1_ptr = malloc(sizeof(struct Point2D));
    struct Point2D *d_p2_ptr = malloc(sizeof(struct Point2D));

    d_p1_ptr->x = 0.0; d_p1_ptr->y = 0.0;
    d_p2_ptr->x = 0.0; d_p2_ptr->y = 0.0;

    __enzyme_autodiff((void*)distance_squared_wrapper,
            enzyme_dup, p1_ptr, d_p1_ptr,
            enzyme_dup, p2_ptr, d_p2_ptr);

    // We expect d(distance_squared)/d(p1.x) = 2(p1.x - p2.x), etc. 
    printf("** distance_square_wrapper test ***\n");
    printf("*p1_ptr = {%f, %f}; *p2_ptr = {%f, %f}\n", p1_ptr->x, p1_ptr->y, p2_ptr->x, p2_ptr->y);
    printf("*d_p1_ptr = {%f, %f}; *d_p2_ptr = {%f, %f}\n", 
            d_p1_ptr->x, d_p1_ptr->y, d_p2_ptr->x, d_p2_ptr->y);

    return 0;
}
