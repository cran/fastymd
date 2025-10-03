litedown::reactor(print = NA)

library(fastymd)
cdate <- c("2025-04-16", "2025-04-17")
(res <- fymd(cdate))
res == as.Date(cdate)
get_ymd(res)
fymd(2025, 4, 16) == res[1L]

fymd(2021, 02, 29) # not a leap year

timelt <- as.POSIXlt(Sys.time(), tz = "UTC")
(timestamp <- strftime(timelt, "%Y-%m-%dT%H:%M:%S%z"))

(res <- fymd(timestamp))
res == as.Date(timestamp, tz = "UTC")

cdate <- "2025-04-16nonsense "
fymd(cdate)
fymd(cdate, strict = TRUE)

library(microbenchmark)

# 1970-01-01 (UNIX epoch) to "2199-01-01"
dates <- seq.Date(from = .Date(0), to = fymd("2199-01-01"), by = "day")

# comparison timings for fymd (character method)
cdates  <- format(dates)
(res_c <- microbenchmark(
    fasttime  = fasttime::fastDate(cdates),
    fastymd   = fymd(cdates),
    ymd       = ymd::ymd(cdates),
    lubridate = lubridate::ymd(cdates),
    check     = "equal"
))

# comparison timings for fymd (numeric method)
ymd  <- get_ymd(dates)
(res_n <- microbenchmark(
    fastymd   = fymd(ymd[[1]], ymd[[2]], ymd[[3]]),
    lubridate = lubridate::make_date(ymd[[1]], ymd[[2]], ymd[[3]]),
    check     = "equal"
))

# comparison timings for year getter
(res_get_year <- microbenchmark(
    fastymd   = get_year(dates),
    ymd       = ymd::year(dates),
    lubridate = lubridate::year(dates),
    check     = "equal"
))

# comparison timings for month getter
(res_get_month <- microbenchmark(
    fastymd   = get_month(dates),
    ymd       = ymd::month(dates),
    lubridate = lubridate::month(dates),
    check     = "equal"
))

# comparison timings for mday getter
(res_get_mday <- microbenchmark(
    fastymd   = get_mday(dates),
    ymd       = ymd::mday(dates),
    lubridate = lubridate::day(dates),
    check     = "equal"
))

