# -------------------------------------------------------------------------
#' Is value a leap year?
#'
# -------------------------------------------------------------------------
#' Determine whether an input value is a leap year using the branchless approach
#' of jerichaux (2025). Method provided for both Dates and numeric values.
#' Numeric values first floored before the calculation is made.
#'
# -------------------------------------------------------------------------
#' @param x An \R object.
#'
# -------------------------------------------------------------------------
#' @return
#'
#' `logical` result.
#'
# -------------------------------------------------------------------------
#' @references
#'
#' jerichaux. (2025) _How to find leap year programmatically in C_.
#' Available at: <https://stackoverflow.com/a/79564914>
#' (Accessed 16 April 2025).
#'
# -------------------------------------------------------------------------
#' @export
is_leap_year <- function(x) {
	UseMethod("is_leap_year")
}

#' @export
is_leap_year.numeric <- function(x) {
	if (!length(x))
		return(logical())
	.Call(C_is_leap_year, x)
}

#' @export
is_leap_year.Date <- function(x) {
	if (!length(x))
		return(logical())
	is_leap_year.numeric(get_year(x))
}

# -------------------------------------------------------------------------
#' @rdname is_leap_year
#' @export
is_leap <- is_leap_year
