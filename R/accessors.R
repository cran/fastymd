# -------------------------------------------------------------------------
#' Generics for accessing the year, month and month-day of an object
#'
# -------------------------------------------------------------------------
#' Fast methods are provided for `Date` objects. The underlying algorithm
#' follows the approach described in Hinnant (2021) for converting days since
#' the [UNIX Epoch](https://en.wikipedia.org/wiki/Unix_time) to [Gregorian
#' Calendar](https://en.wikipedia.org/wiki/Gregorian_calendar) dates.
#'
# -------------------------------------------------------------------------
#' @param x An \R object.
#'
#' @param ... Further arguments passed to or from other methods.
#'
# -------------------------------------------------------------------------
#' @examples
#' date <- as.Date("2025-04-17")
#' get_ymd(date)
#' get_year(date)
#' get_month(date)
#' get_mday(date)
#'
# -------------------------------------------------------------------------
#' @return
#'
#' For `get_ymd()` a data frame with integer columns year, month and mday. For
#' `get_year()`, `get_month()` and `get_mday()`, integer vectors of the
#' requested components.
#'
# -------------------------------------------------------------------------
#' @references
#'
#' Hinnant, J. (2021) _chrono-Compatible Low-Level Date Algorithms_.
#' Available at: <https://howardhinnant.github.io/date_algorithms.html#civil_from_days>
#' (Accessed 17 April 2025).
#'
# -------------------------------------------------------------------------
#' @name accessors
NULL


# -------------------------------------------------------------------------
#' @rdname accessors
#' @export
get_ymd <- function(x, ...) {
	UseMethod("get_ymd")
}

# -------------------------------------------------------------------------
#' @export
get_ymd.default <- function(x, ...) {
	stop(sprintf("Not implemented for objects of class <%s>.",toString(class(x))))
}

# -------------------------------------------------------------------------
#' @export
get_ymd.Date <- function(x, ...) {
	if (length(x)) {
		list2DF(.Call(C_get_ymd, x))
	} else {
		list2DF(list(year = integer(), month = integer(), day = integer()))
	}
}

# -------------------------------------------------------------------------
#' @rdname accessors
#' @export
get_year <- function(x, ...) {
	UseMethod("get_year")
}

# -------------------------------------------------------------------------
#' @export
get_year.default <- function(x, ...) {
	stop(sprintf("Not implemented for objects of class <%s>.", toString(class(x))))
}

# -------------------------------------------------------------------------
#' @export
get_year.Date <- function(x, ...) {
	if (length(x)) .Call(C_get_year, x) else integer()
}

# -------------------------------------------------------------------------
#' @rdname accessors
#' @export
get_month <- function(x, ...) {
	UseMethod("get_month")
}

# -------------------------------------------------------------------------
#' @export
get_month.default <- function(x, ...) {
	stop(sprintf("Not implemented for objects of class <%s>.", toString(class(x))))
}

# -------------------------------------------------------------------------
#' @export
get_month.Date <- function(x, ...) {
	if (length(x)) .Call(C_get_month, x) else integer()
}

# -------------------------------------------------------------------------
#' @rdname accessors
#' @export
get_mday <- function(x, ...) {
	UseMethod("get_mday")
}

# -------------------------------------------------------------------------
#' @export
get_mday.default <- function(x, ...) {
	stop(sprintf("Not implemented for objects of class <%s>.", toString(class(x))))
}

# -------------------------------------------------------------------------
#' @export
get_mday.Date <- function(x, ...) {
	if (length(x)) .Call(C_get_mday, x) else integer()
}
