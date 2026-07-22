#ifndef QUAD_PUSH_H
#define QUAD_PUSH_H

/** Push one particle through a quadrupole slice (thin/thick symplectic map).
 *
 * Self-contained, double-precision version of impactx::elements::Quad's
 * compute_constants() + operator(): all per-slice constants are derived
 * locally instead of being cached on the element.
 *
 * Coordinates are updated in place. The transverse map is the exact linear
 * quad map; the longitudinal coordinate t picks up the slip term
 * slice_ds / (beta*gamma^2). A zero strength reduces to a drift.
 *
 * @param[in,out] x   particle position in x
 * @param[in,out] y   particle position in y
 * @param[in,out] t   particle position in t (longitudinal)
 * @param[in,out] px  particle momentum in x
 * @param[in,out] py  particle momentum in y
 * @param[in]     pt  particle momentum in t (unchanged by a quad)
 * @param[in]     k        quadrupole strength in m^-2 (MADX convention;
 *                         k > 0 horizontally focusing, k < 0 defocusing)
 * @param[in]     slice_ds length of this quad slice in m
 * @param[in]     pt_ref   reference-particle pt (defines beta*gamma^2)
 */
void quad_push (
    double & x, double & y, double & t,
    double & px, double & py, double const pt,
    double k,
    double const slice_ds,
    double const pt_ref
);

double dquad_push_dk(double * x, double * y, double * t,
    double * px, double * py, double const pt,
    double k,
    double const slice_ds,
    double const pt_ref);

#endif
