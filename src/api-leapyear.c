#include "leapyear.h"

#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

SEXP is_leap_year(SEXP y)
{
	int protected = 0;

	/* support doubles coercible to integer */
	if (TYPEOF(y) == REALSXP) {
		y = PROTECT(Rf_duplicate(y)); protected ++;
		double* yy = REAL(y);
		for (R_xlen_t i = 0; i < XLENGTH(y); i ++)
			yy[i] = floor(yy[i]);
		y = PROTECT(Rf_coerceVector(y, INTSXP)); protected ++;
	} else if (TYPEOF(y) != INTSXP) {
		Rf_error("Input `x` must be a numeric object.");
	}

	/* How many inputs */
	R_xlen_t n = XLENGTH(y);

	/* vector for results */
	SEXP out = PROTECT(Rf_allocVector(LGLSXP, n)); protected++;
	int* pout = INTEGER(out);

	/* loop over input */
	const int* py = INTEGER_RO(y);
	for (R_xlen_t i = 0; i < n; i++) {
		int value = py[i];
		pout[i] = value == NA_INTEGER ? NA_INTEGER : ISLEAP(value);
	}
	UNPROTECT(protected);
	return out;
}


