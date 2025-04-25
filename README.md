fastymd is a package for working with Year-Month-Day (YMD) style date objects.
It provides extremely fast passing of character strings and numeric values to
\R date objects as well as fast decomposition of these in to their year month
and day components. The underlying algorithms follow the
[approach of Howard Hinnant](https://howardhinnant.github.io/date_algorithms)
for calculating days from the [UNIX Epoch](https://en.wikipedia.org/wiki/Unix_time)
of [Gregorian Calendar](https://en.wikipedia.org/wiki/Gregorian_calendar) dates
and vice versa.

## Vignettes

A brief overview of fastymd is provided in the vignette distributed with the
package:

* `vignette("fastymd", package = "fastymd")`
