#include "civil_from_days.h"
#include "days_from_civil.h"
#include "utils.h"

#include <stddef.h> // For NULL
#include <math.h>   // For floor

#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

#define INTDIV(x,y) ((int) floor(((double) (x)) / ((double) (y))))

void yearweekday(int z, int firstday, int *year, int *week, int *day);
int yearweek_week(int z, int firstday);
int yearweek_year(int z, int firstday);
static inline int isoweek(int z);
static inline int isoyear(int z);
static inline int epiweek(int z);
static inline int epiyear(int z);


/* ----------------------------------------------------------------------
 * R INTERFACE - List of <year, week, day> from Date input with first day
 * ---------------------------------------------------------------------- */
SEXP get_yearweekday(SEXP x, SEXP firstday)
{
	int protected = 0;
	if (!Rf_inherits(x, "Date"))
		Rf_error("Input `x` must be a <Date> object.");

	if (!Rf_isNumeric(firstday) || XLENGTH(firstday) != 1)
		Rf_error("`firstday` must be a non-missing integer between 1 and 7.");
	firstday = PROTECT(Rf_coerceVector(firstday, INTSXP)); protected++;
	int first = INTEGER_RO(firstday)[0];
	if (first == NA_INTEGER || first < 1 || first > 7)
		Rf_error("`firstday` must be a non-missing integer between 1 and 7.");

	/* How many inputs */
	R_xlen_t n = XLENGTH(x);

	/* if double we want to floor the input before coercing to integer */
	if (Rf_isReal(x)) {
		x = PROTECT(Rf_duplicate(x)); protected ++;
		double* xx = REAL(x);
		for (R_xlen_t i = 0; i < n; i ++)
			xx[i] = floor(xx[i]);
		x = PROTECT(Rf_coerceVector(x, INTSXP)); protected ++;
	}

	/* from now on x is an integer */
	const int* px = INTEGER_RO(x);

	/* vectors to hold the output */
	SEXP year = PROTECT(Rf_allocVector(INTSXP, n)); protected ++;
	SEXP week = PROTECT(Rf_allocVector(INTSXP, n)); protected ++;
	SEXP day = PROTECT(Rf_allocVector(INTSXP, n)); protected ++;
	int* py = INTEGER(year);
	int* pw = INTEGER(week);
	int* pd = INTEGER(day);


	/* loop over input and calculate ymd */
	for (R_xlen_t i = 0; i < n; i ++) {
		int value = px[i];
		if (value == NA_INTEGER) {
			py[i] = pw[i] = pd[i] = NA_INTEGER;
			continue;
		}
		yearweekday(value, first, &py[i], &pw[i], &pd[i]);
	}

	// Create a named list to hold the output
	const char *names[] = {"year", "week", "day", ""};
	SEXP out = PROTECT(Rf_mkNamed(VECSXP, names)); protected++;
	SET_VECTOR_ELT(out, 0, year);
	SET_VECTOR_ELT(out, 1, week);
	SET_VECTOR_ELT(out, 2, day);

	UNPROTECT(protected);

	return out;
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Week from Date input with first day
 * ---------------------------------------------------------------------- */
SEXP get_yearweekday_week(SEXP x, SEXP firstday)
{
	int protected = 0;
	if (!Rf_inherits(x, "Date"))
		Rf_error("Input `x` must be a <Date> object.");

	if (!Rf_isNumeric(firstday) || XLENGTH(firstday) != 1)
		Rf_error("`firstday` must be a non-missing integer between 1 and 7.");
	firstday = PROTECT(Rf_coerceVector(firstday, INTSXP)); protected++;
	int first = INTEGER_RO(firstday)[0];
	if (first == NA_INTEGER || first < 1 || first > 7)
		Rf_error("`firstday` must be a non-missing integer between 1 and 7.");

	/* How many inputs */
	R_xlen_t n = XLENGTH(x);

	/* if double we want to floor the input before coercing to integer */
	if (Rf_isReal(x)) {
		x = PROTECT(Rf_duplicate(x)); protected ++;
		double* xx = REAL(x);
		for (R_xlen_t i = 0; i < n; i ++)
			xx[i] = floor(xx[i]);
		x = PROTECT(Rf_coerceVector(x, INTSXP)); protected ++;
	}

	/* from now on x is an integer */
	const int* px = INTEGER_RO(x);

	/* vectors to hold the output */
	SEXP week = PROTECT(Rf_allocVector(INTSXP, n)); protected ++;
	int* pw = INTEGER(week);

	/* loop over input and calculate ymd */
	for (R_xlen_t i = 0; i < n; i ++) {
		int value = px[i];
		pw[i] = value == NA_INTEGER ? NA_INTEGER : yearweek_week(value, first);
	}

	UNPROTECT(protected);

	return week;
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Year from Date input with first day
 * ---------------------------------------------------------------------- */
SEXP get_yearweekday_year(SEXP x, SEXP firstday)
{
	int protected = 0;
	if (!Rf_inherits(x, "Date"))
		Rf_error("Input `x` must be a <Date> object.");

	if (!Rf_isNumeric(firstday) || XLENGTH(firstday) != 1)
		Rf_error("`firstday` must be a non-missing integer between 1 and 7.");
	firstday = PROTECT(Rf_coerceVector(firstday, INTSXP)); protected++;
	int first = INTEGER_RO(firstday)[0];
	if (first == NA_INTEGER || first < 1 || first > 7)
		Rf_error("`firstday` must be a non-missing integer between 1 and 7.");

	/* How many inputs */
	R_xlen_t n = XLENGTH(x);

	/* if double we want to floor the input before coercing to integer */
	if (Rf_isReal(x)) {
		x = PROTECT(Rf_duplicate(x)); protected ++;
		double* xx = REAL(x);
		for (R_xlen_t i = 0; i < n; i ++)
			xx[i] = floor(xx[i]);
		x = PROTECT(Rf_coerceVector(x, INTSXP)); protected ++;
	}

	/* from now on x is an integer */
	const int* px = INTEGER_RO(x);

	/* vectors to hold the output */
	SEXP year = PROTECT(Rf_allocVector(INTSXP, n)); protected ++;
	int* py = INTEGER(year);

	/* loop over input and calculate ymd */
	for (R_xlen_t i = 0; i < n; i ++) {
		int value = px[i];
		py[i] = value == NA_INTEGER ? NA_INTEGER : yearweek_year(value, first);
	}

	UNPROTECT(protected);

	return year;
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the (iso) week number from a date
 * ---------------------------------------------------------------------- */
SEXP get_isoweek(SEXP x)
{
	return date_in_int_out(x, &isoweek);
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the (iso) year number from a date
 * ---------------------------------------------------------------------- */
SEXP get_isoyear(SEXP x)
{
	return date_in_int_out(x, &isoyear);
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the (epi) week number from a date
 * ---------------------------------------------------------------------- */
SEXP get_epiweek(SEXP x)
{
	return date_in_int_out(x, &epiweek);
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the (iso) year number from a date
 * ---------------------------------------------------------------------- */
SEXP get_epiyear(SEXP x)
{
	return date_in_int_out(x, &epiyear);
}

/* ----------------------------------------------------------------------
 * INTERNALS
 * ---------------------------------------------------------------------- */

void yearweekday(int z, int firstday, int *year, int *week, int *day)
{
	int week_since_epoch = INTDIV(z + 4 - firstday, 7);
	int date = week_since_epoch * 7 + (firstday - 4);
	int midweek_date = date + 3;
	*year = year_from_days(midweek_date);
	int jan1 = days_from_civil(*year, 1, 1);
	*week = 1 + INTDIV(midweek_date - jan1, 7);
	*day = (z - date) + 1;
}

void yearweek(int z, int firstday, int *year, int *week)
{
	int week_since_epoch = INTDIV(z + 4 - firstday, 7);
	int date = week_since_epoch * 7 + (firstday - 4);
	int midweek_date = date + 3;
	*year = year_from_days(midweek_date);
	int jan1 = days_from_civil(*year, 1, 1);
	*week = 1 + INTDIV(midweek_date - jan1, 7);
}

int yearweek_week(int z, int firstday)
{
	int week_since_epoch = INTDIV(z + 4 - firstday, 7);
	int date = week_since_epoch * 7 + (firstday - 4);
	int midweek_date = date + 3;
	int year = year_from_days(midweek_date);
	int jan1 = days_from_civil(year, 1, 1);
	return 1 + INTDIV(midweek_date - jan1, 7);
}

int yearweek_year(int z, int firstday)
{
	int week_since_epoch = INTDIV(z + 4 - firstday, 7);
	int date = week_since_epoch * 7 + (firstday - 4);
	int midweek_date = date + 3;
	return year_from_days(midweek_date);
}

static inline int isoweek(int z)
{
	return yearweek_week(z, 1);
}

static inline int isoyear(int z)
{
	return yearweek_year(z, 1);
}

static inline int epiweek(int z)
{
	return yearweek_week(z, 7);
}

static inline int epiyear(int z)
{
	return yearweek_year(z, 7);
}

