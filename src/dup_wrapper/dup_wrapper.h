#ifndef DUP_WRAPPER_H
#define DUP_WRAPPER_H

struct Point2D {
    double x;
    double y;
};

double mul(double a, double b, double c);
double mul_wrapper(double* a_ptr, double* b_ptr, double* c_ptr);

double distance_squared(struct Point2D p1, struct Point2D p2);
double distance_squared_wrapper(struct Point2D* p1_ptr, struct Point2D* p2_ptr); 

#endif 
