#include <stdio.h>
#include <mpc.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "Failed: %s (line %d)\n", #x, __LINE__); return 1; } } while (0)

int main(void) {
    mpc_t x, y, z;
    mpfr_t error;
    mpc_init2(x, 200); mpc_init2(y, 200); mpc_init2(z, 200);
    mpfr_init2(error, 200);
    mpc_set_si_si(x, 1, 2, MPC_RNDNN);
    mpc_set_si_si(y, 3, -4, MPC_RNDNN);
    CHECK(mpc_mul(z, x, y, MPC_RNDNN) == 0);
    CHECK(mpfr_cmp_si(mpc_realref(z), 11) == 0);
    CHECK(mpfr_cmp_si(mpc_imagref(z), 2) == 0);
    CHECK(mpc_div(z, z, y, MPC_RNDNN) == 0);
    CHECK(mpc_cmp(z, x) == 0);
    mpc_log(y, x, MPC_RNDNN);
    mpc_exp(z, y, MPC_RNDNN);
    mpc_sub(z, z, x, MPC_RNDNN);
    mpc_abs(error, z, MPFR_RNDU);
    CHECK(mpfr_cmp_ui_2exp(error, 1, -190) < 0);
    mpc_set_si_si(x, -4, 0, MPC_RNDNN);
    CHECK(mpc_sqrt(z, x, MPC_RNDNN) == 0);
    CHECK(mpfr_zero_p(mpc_realref(z)));
    CHECK(mpfr_cmp_si(mpc_imagref(z), 2) == 0);
    mpfr_clear(error); mpc_clear(x); mpc_clear(y); mpc_clear(z);
    mpfr_free_cache();
    printf("MPC %s: complex arithmetic and 200-bit transcendental consumer passed\n", mpc_get_version());
    return 0;
}
