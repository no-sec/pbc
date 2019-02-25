#ifndef __pbc_structs_h__
#define __pbc_structs_h__

#include <gmp.h>
#include "pbc_field.h"

// Per-field data.
typedef struct {
    field_ptr field; // The field where the curve is defined.
    element_t a, b;  // The curve is E: Y^2 = X^3 + a X + b.
    // cofac == NULL means we're using the whole group of points.
    // otherwise we're working in the subgroup of order #E / cofac,
    // where #E is the number of points in E.
    mpz_ptr cofac;
    // A generator of E.
    element_t gen_no_cofac;
    // A generator of the subgroup.
    element_t gen;
    // A non-NULL quotient_cmp means we are working with the quotient group of
    // order #E / quotient_cmp, and the points are actually coset
    // representatives. Thus for a comparison, we must multiply by quotient_cmp
    // before comparing.
    mpz_ptr quotient_cmp;
} curve_data, *curve_data_ptr;


// Per-field data.
typedef struct {
    size_t limbs;           // Number of limbs per element.
    size_t bytes;           // Number of bytes per element.
    mp_limb_t *primelimbs;  // Points to an array of limbs holding the modulus.
    mp_limb_t negpinv;      // -p^-1 mod b
    mp_limb_t *R;           // R mod p
    mp_limb_t *R3;          // R^3 mod p
} montfp_data, *montfp_data_ptr;

// Per-element data.
typedef struct {
    char flag;     // flag == 0 means the element is zero.
    mp_limb_t *d;  // Otherwise d points to an array holding the element.
} montfp_element, *montfp_element_ptr;

typedef struct {
    int exp2;
    int exp1;
    int sign1;
    int sign0;
    mpz_t r; // r = 2^exp2 + sign1 * 2^exp1 + sign0 * 1
    mpz_t q; // we work in E(F_q) (and E(F_q^2))
    mpz_t h; // r * h = q + 1
} *a_param_ptr;

typedef struct {
    field_t Fq, Fq2, Eq;
    int exp2, exp1;
    int sign1;
} *a_pairing_data_ptr;

typedef struct {
    // Elements have the form x + ya, where a is the square root of a quadratic
    // nonresidue in the base field.
    element_t x;
    element_t y;
} *fieldquadratic_ptr;

#endif /* __pbc_structs_h__ */
