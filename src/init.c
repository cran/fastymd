#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* .Call calls */
extern SEXP ymd(SEXP, SEXP, SEXP);
extern SEXP ymd_character(SEXP, SEXP);
extern SEXP is_leap_year(SEXP);
extern SEXP get_ymd(SEXP);
extern SEXP get_year(SEXP);
extern SEXP get_month(SEXP);
extern SEXP get_mday(SEXP);
extern SEXP get_yday(SEXP);
extern SEXP get_wday(SEXP);
extern SEXP get_yearweekday(SEXP, SEXP);
extern SEXP get_yearweekday_week(SEXP, SEXP);
extern SEXP get_yearweekday_year(SEXP, SEXP);
extern SEXP get_isoweek(SEXP);
extern SEXP get_isoyear(SEXP);
extern SEXP get_epiweek(SEXP);
extern SEXP get_epiyear(SEXP);


static const R_CallMethodDef CallEntries[] = {
    {"ymd",                     (DL_FUNC) &ymd,                     3},
    {"ymd_character",           (DL_FUNC) &ymd_character,           2},
    {"is_leap_year",            (DL_FUNC) &is_leap_year,            1},
    {"get_ymd",                 (DL_FUNC) &get_ymd,                 1},
    {"get_year",                (DL_FUNC) &get_year,                1},
    {"get_month",               (DL_FUNC) &get_month,               1},
    {"get_mday",                (DL_FUNC) &get_mday,                1},
    {"get_yday",                (DL_FUNC) &get_yday,                1},
    {"get_wday",                (DL_FUNC) &get_wday,                1},
    {"get_yearweekday",         (DL_FUNC) &get_yearweekday,         2},
    {"get_yearweekday_week",    (DL_FUNC) &get_yearweekday_week,    2},
    {"get_yearweekday_year",    (DL_FUNC) &get_yearweekday_year,    2},
    {"get_isoweek",             (DL_FUNC) &get_isoweek,             1},
    {"get_isoyear",             (DL_FUNC) &get_isoyear,             1},
    {"get_epiweek",             (DL_FUNC) &get_epiweek,             1},
    {"get_epiyear",             (DL_FUNC) &get_epiyear,             1},
    {NULL,                      NULL,                               0}
};

void R_init_fastymd(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}

