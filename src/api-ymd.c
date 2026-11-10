#include "civil_from_days.h"
#include "days_from_civil.h"
#include "leapyear.h"
#include "utils.h"

#include <stdbool.h>

#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

/* From musl. */
/* More efficient than using the functions for out purposes. */
/* https://git.musl-libc.org/cgit/musl/tree/src/ctype/isdigit.c */
/* https://git.musl-libc.org/cgit/musl/tree/src/ctype/isspace.c */
#define ISDIGIT(c) ((unsigned)(c)-'0' < 10)
#define ISSPACE(c) ((c) == ' ' || (unsigned)(c)-'\t' < 5)

#define MAX_YEAR 9999

/* ----------------------------------------------------------------------
 * Prototypes
 * ---------------------------------------------------------------------- */
static int days_in_month(int year, unsigned int month);
static bool valid_ymd(int year, int month, int day, bool *warn);


/* ----------------------------------------------------------------------
 * R INTERFACE - Date from <year, month, day> input
 * ---------------------------------------------------------------------- */
SEXP ymd(SEXP y, SEXP m, SEXP d)
{
	R_xlen_t size = XLENGTH(y);
	SEXP out      = PROTECT(Rf_allocVector(INTSXP, size));
	int* pout     = INTEGER(out);
	const int* py = INTEGER_RO(y);
	const int* pm = INTEGER_RO(m);
	const int* pd = INTEGER_RO(d);

	bool warn = false;
	for (R_xlen_t i = 0; i < size; i++) {
		int year  = py[i];
		int month = pm[i];
		int day   = pd[i];

		/* TODO - Is this too harsh? Should we have a flag? */
		if (year != NA_INTEGER && (abs(year) > MAX_YEAR))
			Rf_error("Years must be in the range [%d, %d]. y[%td] is %d.", -MAX_YEAR, MAX_YEAR, i, year);

		if (valid_ymd(year, month, day, &warn)) {
			pout[i] = days_from_civil(year, month, day);
			continue;
		}

		/* invalid year */
		pout[i] = NA_INTEGER;
	}

	if (warn)
		Rf_warning("NAs introduced due to invalid month and/or day combinations.");

	/* set class to "Date" before returning */
	Rf_classgets(out, Rf_mkString("Date"));
	UNPROTECT(1);
	return out;
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Date from "year-month-day" character input
 * ---------------------------------------------------------------------- */
SEXP ymd_character(SEXP y, SEXP strict)
{
	if ((!IS_SCALAR(strict, LGLSXP)) || LOGICAL_RO(strict)[0] == NA_LOGICAL)
		Rf_error("`strict` must be a bool.");

	Rboolean strict_ = LOGICAL_RO(strict)[0];

	R_xlen_t size = XLENGTH(y);
	SEXP out = PROTECT(Rf_allocVector(INTSXP, size));
	const SEXP* py = STRING_PTR_RO(y);
	int* pout = INTEGER(out);
	bool warn = false;
	for (R_xlen_t i = 0; i < size; i++) {

		if (py[i] == NA_STRING) {
			pout[i] = NA_INTEGER;
			continue;
		}

		const char *c = CHAR(py[i]);

		/* skip leading whitespace */
		while(ISSPACE(*c))
			c++;

		/* handle negatives */
		bool negative = false;
		if (*c == '-') {
			negative = true;
			c++;
		}

		if (!ISDIGIT(*c)) {
			pout[i] = NA_INTEGER;
			warn = true;
			continue;
		}

		/* Otherwise, string starts (correctly) with digit */

		/* handle the year with arbitrary MAXIMUM */
		int year = 0;
		while (ISDIGIT(*c)) {
			year = year * 10 + (*c - '0');
			if (year > MAX_YEAR)
				Rf_error("Years must be in the range [%d, %d]. y[%td] is %d.", -MAX_YEAR, MAX_YEAR, i, negative ? -year : year);
			c++;
		}
		if (negative)
			year= -year;

		/* skip non-digit values */
		while (*c != '\0' && !ISDIGIT(*c))
			c++;

		/* handle the month */
		bool invalid = false;
		int month = 0;
		while (*c && ISDIGIT(*c)) {
			month = month * 10 + (*c - '0');
			if (month > 12)	{
				invalid = true;
				break;
			}
			c++;
		}
		if (invalid || month ==  0) {
			pout[i] = NA_INTEGER;
			warn = true;
			continue;
		}

		/* skip non-digit values */
		while (*c != '\0' && !ISDIGIT(*c))
			c++;

		/* handle the day */
		invalid = false;
		int daysinmonth = days_in_month(year, month);
		int day = 0;
		while (*c != '\0' && ISDIGIT(*c)) {
			day = day * 10 + (*c - '0');
			if (day > daysinmonth) {
				invalid = true;
				break;
			}
			c++;
		}
		if (invalid || day ==  0) {
			pout[i] = NA_INTEGER;
			warn = true;
			continue;
		}

		/* skip trailing whitespace */
		while(ISSPACE(*c))
			c++;

		/* if strict we allow nothing at the end */
		if (strict_ && *c != '\0') {
			pout[i] = NA_INTEGER;
			warn = true;
			continue;
		}
		pout[i] = days_from_civil(year, month, day);
	}

	if (warn)
		Rf_warning("NAs introduced due to invalid date strings.");

	/* set class to "Date" */
	Rf_classgets(out, Rf_mkString("Date"));


	UNPROTECT(1);
	return out;
}

/* ----------------------------------------------------------------------
 * R INTERFACE - List of <year, month, day> from Date input
 * ---------------------------------------------------------------------- */
SEXP get_ymd(SEXP x)
{
	if (!Rf_inherits(x, "Date"))
		Rf_error("Input `x` must be a <Date> object.");

	int protected = 0;

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
	SEXP month = PROTECT(Rf_allocVector(INTSXP, n)); protected ++;
	SEXP day = PROTECT(Rf_allocVector(INTSXP, n)); protected ++;
	int* py = INTEGER(year);
	int* pm = INTEGER(month);
	int* pd = INTEGER(day);

	/* loop over input and calculate ymd */
	for (R_xlen_t i = 0; i < n; i ++) {
		int value = px[i];
		if (value == NA_INTEGER) {
			py[i] = pm[i] = pd[i] = NA_INTEGER;
			continue;
		}
		civil_from_days(value, &py[i], &pm[i], &pd[i]);
	}

	// Create a named list to hold the output
	const char *names[] = {"year", "month", "day", ""};
	SEXP out = PROTECT(Rf_mkNamed(VECSXP, names)); protected++;
	SET_VECTOR_ELT(out, 0, year);
	SET_VECTOR_ELT(out, 1, month);
	SET_VECTOR_ELT(out, 2, day);

	UNPROTECT(protected);

	return out;
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the year from a date
 * ---------------------------------------------------------------------- */
SEXP get_year(SEXP x)
{
	return date_in_int_out(x, &year_from_days);
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the month from a date
 * ---------------------------------------------------------------------- */
SEXP get_month(SEXP x)
{
	return date_in_int_out(x, &month_from_days);
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the day of the month from a date
 * ---------------------------------------------------------------------- */
SEXP get_mday(SEXP x)
{
	return date_in_int_out(x, &day_from_days);
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the day of the year from a date
 * ---------------------------------------------------------------------- */
SEXP get_yday(SEXP x)
{
	return date_in_int_out(x, &day_of_year);
}

/* ----------------------------------------------------------------------
 * R INTERFACE - Get the weekday: 0 (Sunday) to 6 (Saturday)
 * ---------------------------------------------------------------------- */
SEXP get_wday(SEXP x)
{
	return date_in_int_out(x, &day_of_week);
}

/* ----------------------------------------------------------------------
 * INTERNALS
 * ---------------------------------------------------------------------- */
static int days_in_month(int year, unsigned int month)
{
	const int     days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	return month == 2 && ISLEAP(year) ? 29 : days[month - 1];
}

static bool valid_ymd(int year, int month, int day, bool *warn)
{
	if (year == NA_INTEGER || month == NA_INTEGER || day == NA_INTEGER)
		return false;

	if (month < 1 || month > 12) {
		*warn = true;
		return false;
	}

	if (day < 1 || day > days_in_month(year, month)) {
		*warn = true;
		return false;
	}

	return true;
}
