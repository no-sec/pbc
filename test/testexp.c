//Boneh-Lynn-Shacham short signatures demo
//This one uses the signature library and is more realistic
#include <string.h>
#include "pbc.h"
#include "get_time.h"


int main(void)
{
    pairing_t pairing;
    element_t g1, u1, up1, g2, u2, up2, r;
    mpz_t r_mpz;
    element_pp_t g1_pp, g2_pp;
    double t0, t1;
    int i, n;

    printf("reading pairing from stdin...\n");
    pairing_init_inp_str(pairing, stdin);

    element_init(r, pairing->Zr);
    element_init(g1, pairing->G1);
    element_init(u1, pairing->G1);
    element_init(up1, pairing->G1);
    element_init(g2, pairing->G2);
    element_init(u2, pairing->G2);
    element_init(up2, pairing->G2);

    element_random(r);
    element_random(g1);
    element_random(g2);

    mpz_init(r_mpz);
    element_to_mpz(r_mpz, r);

    element_pp_init(g1_pp, g1);
    element_pp_init(g2_pp, g2);

    n = 100;
    t0 = get_time();
    for (i=0; i<n; i++) {
        element_pow_mpz(u1, g1, r_mpz);
    }
    t1 = get_time();
    printf("G1 exp:\t\t%fs\n", t1 - t0);

    n = 100;
    t0 = get_time();
    for (i=0; i<n; i++) {
        element_pow_mpz(u2, g2, r_mpz);
    }
    t1 = get_time();
    printf("G2 exp:\t\t%fs\n", t1 - t0);

    n = 100;
    t0 = get_time();
    for (i=0; i<n; i++) {
        element_pp_pow(up1, r_mpz, g1_pp);
    }
    t1 = get_time();
    printf("G1 pp exp:\t%fs\n", t1 - t0);

    n = 100;
    t0 = get_time();
    for (i=0; i<n; i++) {
        element_pp_pow(up2, r_mpz, g2_pp);
    }
    t1 = get_time();
    printf("G2 pp exp:\t%fs\n", t1 - t0);

    if (element_cmp(u1, up1)) {
      printf("Oops 1!\n");
    }
    if (element_cmp(u2, up2)) {
      printf("Oops 2!\n");
    }

    return 0;
}