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
dates  <- seq.Date(from = start, to = end, by = 1)
if (!nzchar(Sys.getenv("FASTYMD_TEST_ALL", unset = ""))) {
    n <- length(dates) %/% 1000
    dates <- c(head(dates, n), sample(dates, n), tail(dates, n))
}
lt     <- as.POSIXlt(dates)
years  <- lt$year + 1900
months <- lt$mon + 1
days   <- lt$mday
ydays  <- lt$yday + 1
wdays  <- lt$wday
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
expect_equal(get_yday(dates), ydays)
#expect_equal(get_wday(dates), (wdays - 1) %% 7 + 1)
expect_equal(get_wday(dates), wdays)
expect_equal(get_ymd(dates), data.frame(year = years, month = months, day = days))

expect_equal(get_year(dates[0]), integer())
expect_equal(get_month(dates[0]), integer())
expect_equal(get_mday(dates[0]), integer())
expect_equal(get_yday(dates[0]), integer())
expect_equal(get_wday(dates[0]), integer())
expect_equal(get_ymd(dates[0]), data.frame(year = integer(0), month = integer(0), day = integer(0)))


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
expect_equal(fymd(9999, 12, 31), date)
expect_error(
    fymd("10000-01-01"),
    "Years must be in the range [-9999, 9999]. y[0] is 10000.",
    fixed = TRUE
)
expect_error(
    fymd(10000, 01, 01),
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
    fymd(-10000, 12, 31),
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
.is_leap_year <- function(x) ((x %% 4 == 0) & (x %% 100 != 0)) | (x %% 400 == 0)

# matches the basic implementation
x <- (-9999):9999
expect_identical(is_leap_year(x), .is_leap_year(x))

# alias works
expect_identical(is_leap_year(x), is_leap(x))


# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# yearweek checks (From grates which were in turn adapted from aweek)
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
yrs <- 2001:2019
dates <- sprintf("%d-01-01", yrs)
dates <- as.Date(c(dates, NA_character_))

# ISOWEEKS

expected_year <- c(
	2001L, 2002L, 2003L, 2004L, 2004L,
	2005L, 2007L, 2008L, 2009L, 2009L,
	2010L, 2011L, 2013L, 2014L, 2015L,
	2015L, 2016L, 2018L, 2019L, NA_integer_
)

expected_week <- c(
	01L, 01L, 01L, 01L, 53L,
	52L, 01L, 01L, 01L, 53L,
	52L, 52L, 01L, 01L, 01L,
	53L, 52L, 01L, 01L, NA_integer_
)

expect_identical(get_isoyear(dates), expected_year)
expect_identical(get_isoweek(dates), expected_week)
expect_identical(get_yearweekday(dates, 1)$year, expected_year)
expect_identical(get_yearweekday(dates, 1)$week, expected_week)
expect_identical(get_yearweekday_year(dates, 1), expected_year)
expect_identical(get_yearweekday_week(dates, 1), expected_week)


# EPIWEEKS

expected_year <- c(
	2001L, 2002L, 2003L, 2003L, 2004L,
	2006L, 2007L, 2008L, 2008L, 2009L,
	2010L, 2012L, 2013L, 2014L, 2014L,
	2015L, 2017L, 2018L, 2019L, NA_integer_)

expected_week <- c(
	1L, 1L, 1L, 53L, 52L,
	1L, 1L, 1L, 53L, 52L,
	52L, 1L, 1L, 1L, 53L,
	52L, 1L, 1L, 1L, NA_integer_
)


