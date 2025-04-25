set.seed(1550)

# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# First check that we can recover a wide range of dates.
# End date is the maximum as.Date() can handle.
# The start date comes as a mirror of the maximum year.
# We take head/tail and a sample of the middle so not to bog down CRAN
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
start  <- .Date(0) - 4371587 # '-9999-01-01'
end    <- as.Date("9999-12-31")
dates  <- seq.Date(from = start, to = end, by=1)
if (!nzchar(Sys.getenv("FASTYMD_TEST_ALL", unset = ""))) {
	n <- length(dates) %/% 1000
	dates <- c(head(dates, n), sample(dates, n), tail(dates, n))
}
lt     <- as.POSIXlt(dates)
years  <- lt$year + 1900
months <- lt$mon + 1
days   <- lt$mday
chars  <- format(dates)

# check the numeric version of fymd
res1   <- fymd(years, months, days)
expect_equal(res1, dates)

# check the numeric version of fymd
res2   <- fymd(chars)
expect_identical(res1, res2)

# Now check the accessors

expect_equal(get_year(dates), years)
expect_equal(get_month(dates), months)
expect_equal(get_mday(dates), days)

# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# Other fymd checks
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------

# Do we match the R conversion for non-negative dates
idx <- grep("^-", chars, invert = TRUE, perl = TRUE)
expect_equal(res2[idx], as.Date(chars[idx], format = "%Y-%m-%d"))

# upper limit for year is 9999
cdate <- "9999-12-31"
date  <- as.Date(cdate)
expect_equal(fymd(cdate), date)
expect_equal(fymd(9999,12,31), date)
expect_error(
	fymd("10000-01-01"),
	"Years must be in the range [-9999, 9999]. y[0] is 10000.",
	fixed = TRUE
)
expect_error(
	fymd(10000,01,01),
	"Years must be in the range [-9999, 9999]. y[0] is 10000.",
	fixed = TRUE
)

# lower limit for year is -999
cdate <- "-9999-01-01"
date  <- .Date(0) - 4371587
expect_equal(fymd(cdate), date)
expect_equal(fymd(-9999, 01, 01), date)
expect_error(
	fymd("-10000-12-31"),
	"Years must be in the range [-9999, 9999]. y[0] is -10000.",
	fixed = TRUE
)
expect_error(
	fymd(-10000,12,31),
	"Years must be in the range [-9999, 9999]. y[0] is -10000.",
	fixed = TRUE
)

# trims whitespace like R
cdate <- "   2020-02-29   "
date  <- as.Date(cdate)
expect_equal(fymd(cdate), date)

# not strict by default
cdate <- "  2020-02-29nonsense"
expect_equal(fymd(cdate), date)

# strict will return NA with warning
expect_warning(
	expect_identical(fymd(cdate, strict = TRUE), .Date(NA_integer_)),
	"NAs introduced due to invalid date strings.",
	fixed = TRUE
)

# leap years handled
expect_equal(fymd("2020-02-29"), as.Date("2020-02-29"))
expect_warning(
	expect_identical(fymd("2021-02-29"), .Date(NA_integer_)),
	"NAs introduced due to invalid date strings.",
	fixed = TRUE
)

expect_equal(fymd(2020, 02, 29), as.Date("2020-02-29"))
expect_warning(
	expect_identical(fymd(2021, 02, 29), .Date(NA_integer_)),
	"NAs introduced due to invalid month and/or day combinations.",
	fixed = TRUE
)

# zero months handled
expect_warning(
	expect_identical(fymd("2025-00-01"), .Date(NA_integer_)),
	"NAs introduced due to invalid date strings.",
	fixed = TRUE
)

expect_warning(
	expect_identical(fymd(2025, 0, 1), .Date(NA_integer_)),
	"NAs introduced due to invalid month and/or day combinations.",
	fixed = TRUE
)

# zero days handled
expect_warning(
	expect_identical(fymd("2025-01-00"), .Date(NA_integer_)),
	"NAs introduced due to invalid date strings.",
	fixed = TRUE
)

expect_warning(
	expect_identical(fymd(2025, 1, 0), .Date(NA_integer_)),
	"NAs introduced due to invalid month and/or day combinations.",
	fixed = TRUE
)

# days default to 1
expect_identical(fymd(2025, 1), fymd(2025, 1, 1))

# months default to 1
expect_identical(fymd(y = 2025, d = 1), fymd(2025, 1, 1))

# years have no default
expect_error(fymd(m = 1, d = 1))


# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# Other leap year checks
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------

# basic implementation
.is_leap_year <- function(x) ((x%%4 == 0) & (x%%100 != 0)) | (x%%400 == 0)

# matches the basic implementation
x <- (-9999):9999
expect_identical(is_leap_year(x), .is_leap_year(x))

# alias works
expect_identical(is_leap_year(x), is_leap(x))

