# -------------------------------------------------------------------------
#' Construct dates from character and numeric input
#'
# -------------------------------------------------------------------------
#' `fymd()` is a generic for validated conversion of \R objects to (integer)
#' `Date`. Efficient methods are provided for `numeric` and `character`
#' inputs.
#'
# -------------------------------------------------------------------------
#' The underlying algorithm for both the numeric and character methods follow
#' the approach described in Hinnant (2021) for calculating days from the
#' [UNIX Epoch](https://en.wikipedia.org/wiki/Unix_time) from [Gregorian
#' Calendar](https://en.wikipedia.org/wiki/Gregorian_calendar) dates.
#'
#' The character version parses inputs in a fixed, year, month and day order.
#' These values must be digits but can be separated by any non-digit character.
#' It is similar in spirit to that of Simon Urbanek's [fastDate()][fasttime::fastDate()]
#' implementation in that we use pure text parsing and no system calls.
#' `fymd()` differs from [`fastDate()`][fasttime::fastDate()] in that it
#' validates all dates for correctness and supports a a much larger range of
#' dates (i.e. the [Proleptic Gregorian calendar](https://en.wikipedia.org/wiki/Proleptic_Gregorian_calendar).
#' This additional capability does come with a small performance cost but, IMO,
#' remains competetive.
#'
#' For both numeric and character versions years must be in the range
#' `[-9999, 9999]`.
#'
# -------------------------------------------------------------------------
#' @param ...
#'
#' Arguments to be passed to or from other methods.
#'
#' @param y,m,d `integerish`.
#'
#' Numeric vector corresponding to the desired years, months and days.
#'
#' Double vectors are coerced to integer.
#'
#' Length 1 vectors will be recycled to the common size across `y`, `m` and `d`.
#'
#' @param x `character`.
#'
#' Vector of year-month-date strings in a numeric format (e.g. "2020-02-01").
#'
#' Parses digits separated by non-digits.
#'
#' Leading and trailing whitespace will be ignored.
#'
#' @param strict `bool`.
#'
#' Should non-whitespace output after a valid date be allowed?
#'
#' `FALSE` (default) will ignore output after a valid date whereas `TRUE` will
#' reject said strings, returning `NA`.
#'
# -------------------------------------------------------------------------
#' @return
#'
#' A `Date` object
#'
# -------------------------------------------------------------------------
#' @examples
#'
#' cdate     <- "2025-04-16"
#' timestamp <- "2025-04-16T09:45:53+0000"
#'
#' # Ignoring the time element
#' fymd(timestamp)
#'
#' # This will return NA with a warning
#' fymd(timestamp, strict = TRUE)
#'
#' # Checking
#' as.Date(cdate) == fymd(timestamp)
#'
#' # Leap year
#' fymd(2020, 2, 29)
#'
#' # Not a leap year
#' fymd(2021, 2, 29)
#'
# -------------------------------------------------------------------------
#' @references
#'
#' Hinnant, H. (2021) _chrono-Compatible Low-Level Date Algorithms_.
#' Available at: <https://howardhinnant.github.io/date_algorithms.html#days_from_civil>
#' (Accessed 17 April 2025).
#'
#' Urbanek S (2022). _fasttime: Fast Utility Function for Time Parsing
#' and Conversion_.
#' R package version 1.1-0, \doi{10.32614/CRAN.package.fasttime}.
#'
# -------------------------------------------------------------------------
#' @export
fymd <- function(...) {
	# dispatch on the first dot if present
	if(...length()) UseMethod("fymd", ..1) else .Date(integer())
}

# -------------------------------------------------------------------------
#' @rdname fymd
#' @export
fymd.default <- function(...) {
	stop(sprintf(
		"Not implemented for objects of class <%s>.",
		toString(class(..1))
	))
}

# -------------------------------------------------------------------------
#' @rdname fymd
#' @export
fymd.numeric <- function(y, m = 1, d = 1, ...) {

	# Coerce to integer
	y <- as.integer(y)
	m <- as.integer(m)
	d <- as.integer(d)

	# Ensure that inputs are the same length or length 1
	dat     <- list(y, m, d)
	lengths <- lengths(dat)
	unique  <- unique(lengths)
	l       <- length(unique)

	# We can return early if all length 0
	if (l == 1L && unique[1L] == 0L)
		return(.Date(integer()))

	# handle different lengths
	if (!l == 1L) {

		# Cannot recycle length 0 vectors
		if (any(unique == 0L))
			stop("Unable to recycle vectors of length 0.")

		# Cannot recycle 3 different lengths
		if (l == 3L)
			stop("`year`, `month` and `day` values must have the same length (or be of length 1).")

		# l must be 2 in which case one of these values must be 1 to recycle
		if (!any(unique == 1L))
			stop("`year`, `month` and `day` values must have the same length (or be of length 1).")

		# recycle
		dat <- lapply(dat, rep_len, max(unique))
	}

	# pull out the recycled values
	y <- dat[[1L]]
	m <- dat[[2L]]
	d <- dat[[3L]]

	# Call the C
	.Call(C_ymd, y, m, d)
}

# -------------------------------------------------------------------------
#' @rdname fymd
#' @export
fymd.character <- function(x, strict = FALSE, ...) {
	if (length(x)) .Call(C_ymd_character, x, strict) else .Date(integer())
}
