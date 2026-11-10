#ifndef FASTYMD_UTILS_H
#define FASTYMD_UTILS_H

#include <Rinternals.h>

SEXP date_in_int_out(SEXP x, int (*f) (int));

#endif
