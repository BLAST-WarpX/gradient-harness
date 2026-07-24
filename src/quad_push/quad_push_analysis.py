import matplotlib.pyplot as plt
import numpy as np

import quad_push

x = 1.0
y = 0.0
t = 0.0
px = 0.0
py = 0.0
pt = 0.0
slice_ds = 1.0
pt_ref = 3.0

k_range = [-10.0, 10.0]
k_step = 0.1

def main():
    k_vals = np.arange(k_range[0], k_range[1], k_step, dtype=np.float64)
    k_vals = np.append(k_vals, [0.0])  # ensure one k_val is identically zero

    dquad_push_dk_enzyme_vals = []
    for k in k_vals:
        #print(quad_push.dquad_push_dk_enzyme(x, y, t, px, py, pt, k, slice_ds, pt_ref))
        dquad_push_dk_enzyme_vals.append(quad_push.dquad_push_dk_enzyme(x, y, t, px, py, pt, k, slice_ds, pt_ref))

    dx = [item[0] for item in dquad_push_dk_enzyme_vals]
    dy = [item[1] for item in dquad_push_dk_enzyme_vals] 
    dt = [item[2] for item in dquad_push_dk_enzyme_vals]
    dpx = [item[3] for item in dquad_push_dk_enzyme_vals]
    dpy = [item[4] for item in dquad_push_dk_enzyme_vals]

    plt.plot(k_vals, dx, '-')
    plt.plot(k_vals, dy, '--', lw=2.0)
    plt.plot(k_vals, dt, '-.')
    plt.plot(k_vals, dpx, ':')
    plt.plot(k_vals, dpy, '--')
    
    plt.xlabel("k (quadrupole strength)")
    plt.legend(["dx/dk", "dy/dk", "dt/dk", "d(px)/dk", "d(py)/dk"])
    plt.title(f"quad_push derivatives at (x, y) = ({x}, {y})")

    plt.show()

if __name__ == '__main__':
    main()
