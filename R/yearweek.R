# -------------------------------------------------------------------------
#' Map dates to year-week representations
#'
# -------------------------------------------------------------------------
#' Year-week-day (ywd) representations start on a user defined day of the week and
#' span a 7 day period. The first week of a "year" is considered to be the first
#' yearweek containing 4 days of the given calendar year. This means that of the
#' calendar year will sometimes be different to that of the associated
#' ywd representation.
#'
#' Isoweeks are taken to start on a Monday (`firstday = 1`) and epiweeks to
#' start on a Sunday (`firstday = 7`).
#'
# -------------------------------------------------------------------------
#' @param x `[Date]`
#'
#' @param firstday `[integer]`
#'
#' First day of the week you are mapping to.
#'
#' From 1 (Monday) to 7 (Sunday).
#'
# -------------------------------------------------------------------------
#' @returns
#'
#' - `get_yearweekday()`; A list with entries `year`, `week` and `day`.
#' - `get_yearweekday_week()`; The `week` of the corresponding ywd representation.
#' - `get_yearweekday_year()`; The `year` of the corresponding ywd representation.
#' - `get_isoweek` and `get_isoyear`; corresponding isoweek and isoyear values.
#' - `get_epiweek` and `get_isoyear`; corresponding epiweek and epiyear values.
#'
# -------------------------------------------------------------------------
#' @name yearweek
NULL

# -------------------------------------------------------------------------
#' @rdname yearweek
#' @export
get_yearweekday <- function(x, firstday) {
	.Call(C_get_yearweekday, x, firstday)
}

# -------------------------------------------------------------------------
#' @rdname yearweek
#' @export
get_yearweekday_week <- function(x, firstday) {
	.Call(C_get_yearweekday_week, x, firstday)
}

# -------------------------------------------------------------------------
#' @rdname yearweek
#' @export
get_yearweekday_year <- function(x, firstday) {
	.Call(C_get_yearweekday_year, x, firstday)
}

# -------------------------------------------------------------------------
#' @rdname yearweek
#' @export
get_isoweek <- function(x) {
	.Call(C_get_isoweek, x)
}

# -------------------------------------------------------------------------
#' @rdname yearweek
#' @export
get_isoyear <- function(x) {
	.Call(C_get_isoyear, x)
}

# -------------------------------------------------------------------------
#' @rdname yearweek
#' @export
get_epiweek <- function(x) {
	.Call(C_get_epiweek, x)
}

# -------------------------------------------------------------------------
#' @rdname yearweek
#' @export
get_epiyear <- function(x) {
	.Call(C_get_epiyear, x)
}
