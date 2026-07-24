import matplotlib.pyplot as plt
import numpy as np

import quad_push

x = 0.0
y = 0.0
t = 0.0
px = 1.0
py = 1.0
pt = 1.0
slice_ds = 1.0
pt_ref = 3.0

k_range = [-10.0, 10.0]
k_step = 0.1

def main():
    k_vals = np.arange(k_range[0], k_range[1], k_step, dtype=np.float64)
    dquad_push_dk_enzyme_vals = []
    for k in k_vals:
        print(quad_push.dquad_push_dk_enzyme(x, y, t, px, py, pt, k, slice_ds, pt_ref))
        dquad_push_dk_enzyme_vals.append(quad_push.dquad_push_dk_enzyme(x, y, t, px, py, pt, k, slice_ds, pt_ref))

    plt.plot(k_vals, dquad_push_dk_enzyme_vals)
    
    plt.xlabel("k")
    plt.legend(["dx", "dy", "dt", "dpx", "dpy"])
    
    plt.show()

if __name__ == '__main__':
    main()
