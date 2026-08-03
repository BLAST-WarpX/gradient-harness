import matplotlib.pyplot as plt
import numpy as np

import quad_push

x0 = 1.0
y0 = 0.0
t0 = 0.0
px0 = 0.0
py0 = 0.0
pt = 0.0
slice_ds = 1.0
pt_ref = 3.0

k_range = [-5.5, 5.5]
k_step = 0.1
k_step_taylor = 0.01

xlims = [-5, 5]
ylims = [-3.75, 10.5]

def main():
    k_vals = np.arange(k_range[0], k_range[1], k_step, dtype=np.float64)
    k_vals = np.append(k_vals, [0.0])  # ensure one k_val is identically zero
    k_vals.sort()

    k_vals_taylor = np.arange(k_range[0], k_range[1], k_step_taylor, dtype=np.float64)
    k_vals_taylor = np.append(k_vals_taylor, [0.0])  # ensure one k_val is identically zero
    k_vals_taylor.sort()

    quad_push_vals = []
    dquad_push_dk_enzyme_vals = []
    for k in k_vals:
        quad_push_vals.append(quad_push.quad_push(x0, y0, t0, px0, py0, pt, k, slice_ds, pt_ref))
        dquad_push_dk_enzyme_vals.append(quad_push.dquad_push_dk_enzyme(x0, y0, t0, px0, py0, pt, k, slice_ds, pt_ref))

    quad_push_taylor_vals = []
    dquad_push_taylor_dk_enzyme_vals = []
    for k in k_vals_taylor:
        quad_push_taylor_vals.append(quad_push.quad_push_taylor(x0, y0, t0, px0, py0, pt, k, slice_ds, pt_ref))
        dquad_push_taylor_dk_enzyme_vals.append(quad_push.dquad_push_taylor_dk_enzyme(x0, y0, t0, px0, py0, pt, k, slice_ds, pt_ref))
    
    x = [item[0] for item in quad_push_vals]
    px = [item[3] for item in quad_push_vals]

    dx = [item[0] for item in dquad_push_dk_enzyme_vals]
    dpx = [item[3] for item in dquad_push_dk_enzyme_vals] 

    fig1, ax1 = plt.subplots()
    ax1.plot(k_vals, x, '-', lw = 3.0, alpha=0.9)
    ax1.plot(k_vals, px, '-', lw = 3.0, alpha=0.9)

    ax1.plot(k_vals, dx, '-', lw=3.5, alpha=0.9)
    ax1.plot(k_vals, dpx, '-', lw=2.5, alpha=0.9)

    ax1.legend(["x", "px", "dx/dk", "d(px)/dk"], fontsize=24, loc="upper right", framealpha=1.0)

    ax1.set_xlim(xlims[0], xlims[1])
    ax1.set_ylim(ylims[0], ylims[1])

    ax1.tick_params(labelsize=18)
    ax1.set_xlabel("k (quadrupole strength)", fontsize=24)
    ax1.set_title(f"quad_push gradient at (x, y) = ({x0}, {y0})", fontsize=30)
    ax1.grid(linestyle='--', linewidth=0.5, alpha=0.7)

    fig2, ax2 = plt.subplots()
    x_taylor = [item[0] for item in quad_push_taylor_vals]
    px_taylor = [item[3] for item in quad_push_taylor_vals]

    dx_taylor = [item[0] for item in dquad_push_taylor_dk_enzyme_vals]
    dpx_taylor = [item[3] for item in dquad_push_taylor_dk_enzyme_vals]

    ax2.axvline(x=-4, color='black', linewidth=1.5, alpha=0.9, linestyle="--", label="_nolegend_")
    ax2.axvline(x=4, color='black', linewidth=1.5, alpha=0.9, linestyle="--", label="_nolegend_")
    
    ax2.plot(k_vals_taylor, x_taylor, '-', lw = 3.0, alpha=0.9)
    ax2.plot(k_vals_taylor, px_taylor, '-', lw = 3.0, alpha=0.9)

    ax2.plot(k_vals_taylor, dx_taylor, '-', lw=3.0, alpha=0.9)
    ax2.plot(k_vals_taylor, dpx_taylor, '-', lw=3.0)

    ax2.set_xlim(xlims[0], xlims[1])
    ax2.set_ylim(ylims[0], ylims[1])

    ax2.legend(["x", "px", "dx/dk", "d(px)/dk"], fontsize=24, loc="upper right", framealpha=1.0)
    ax2.tick_params(labelsize=18)
    ax2.set_xlabel("k (quadrupole strength)", fontsize=24)
    ax2.set_title(f"quad_push_taylor gradient at (x, y) = ({x0}, {y0})\n(exaggerated scale)", fontsize=30)
    ax2.grid(linestyle='--', linewidth=0.5, alpha=0.7)

    plt.show()

if __name__ == '__main__':
    main()
