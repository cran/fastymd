#include "utils.h"

#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

SEXP date_in_int_out(SEXP x, int (*f) (int))
{
	if (!Rf_inherits(x, "Date"))
		Rf_error("Input `x` must be a <Date> object.");

	int protected = 0;

	/* support doubles coercible to integer */
	if (TYPEOF(x) == REALSXP) {
		x = PROTECT(Rf_duplicate(x)); protected ++;
		double* xx = REAL(x);
		for (R_xlen_t i = 0; i < XLENGTH(x); i ++)
			xx[i] = floor(xx[i]);
		x = PROTECT(Rf_coerceVector(x, INTSXP)); protected ++;
	} else if (TYPEOF(x) != INTSXP) {
		Rf_error("Input `x` must be a numeric object.");
	}

	/* How many inputs */
	R_xlen_t n = XLENGTH(x);

	/* vector for results */
	SEXP out = PROTECT(Rf_allocVector(INTSXP, n)); protected++;
	int* pout = INTEGER(out);

	/* loop over input */
	const int* px = INTEGER_RO(x);
	for (R_xlen_t i = 0; i < n; i++) {
		int value = px[i];
		pout[i] = value == NA_INTEGER ? NA_INTEGER : f(value);
	}
	UNPROTECT(protected);
	return out;
}
