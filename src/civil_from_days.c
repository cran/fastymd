/* --------------------------------------------------------------------------
 This is a C implementation of the civil_from_days code documented by Howard Hinnant
 in the article "chrono-Compatible Low-Level Date Algorithm":
 https://howardhinnant.github.io/date_algorithms.html#civil_from_days

 The C implementation came from a post by Thiago Adams:
 http://thradams.com/date.html

 The original code by Howard is from his date library
 (https://github.com/HowardHinnant/date) and released under the MIT licence below:

 // The MIT License (MIT)
 //
 // Copyright (c) 2015, 2016, 2017 Howard Hinnant
 // Copyright (c) 2016 Adrian Colomitchi
 // Copyright (c) 2017 Florian Dang
 // Copyright (c) 2017 Paul Thompson
 // Copyright (c) 2018, 2019 Tomasz Kamiński
 // Copyright (c) 2019 Jiangang Zhuang
 //
 // Permission is hereby granted, free of charge, to any person obtaining a copy
 // of this software and associated documentation files (the "Software"), to deal
 // in the Software without restriction, including without limitation the rights
 // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 // copies of the Software, and to permit persons to whom the Software is
 // furnished to do so, subject to the following conditions:
 //
 // The above copyright notice and this permission notice shall be included in all
 // copies or substantial portions of the Software.
 //
 // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 // SOFTWARE.
 //
 // Our apologies.  When the previous paragraph was written, lowercase had not yet
 // been invented (that would involve another several millennia of evolution).
 // We did not mean to shout.

 -------------------------------------------------------------------------- */

#include "civil_from_days.h"

void civil_from_days(int z, int *year, int *month, int *day)
{
	z += 719468;
	const int era = (z >= 0 ? z : z - 146096) / 146097;
	const unsigned doe = (unsigned)(z - era * 146097);          // [0, 146096]
	const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
	const int y = (int)(yoe) + era * 400;
	const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
	const unsigned mp = (5 * doy + 2) / 153;                                   // [0, 11]
	const unsigned d = doy - (153 * mp + 2) / 5 + 1;                             // [1, 31]
	const unsigned m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]

	*year = y + (m <= 2);
	*month = m;
	*day = d;
}

int year_from_days(int z)
{
	z += 719468;
	const int era = (z >= 0 ? z : z - 146096) / 146097;
	const unsigned doe = (unsigned)(z - era * 146097);          // [0, 146096]
	const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
	const int y = (int)(yoe) + era * 400;
	const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
	const unsigned mp = (5 * doy + 2) / 153;                                   // [0, 11]
	const unsigned m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]

	return y + (m <= 2);
}

int month_from_days(int z)
{
	z += 719468;
	const int era = (z >= 0 ? z : z - 146096) / 146097;
	const unsigned doe = (unsigned)(z - era * 146097);          // [0, 146096]
	const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
	const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
	const unsigned mp = (5 * doy + 2) / 153;                                   // [0, 11]
	const unsigned m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]

	return m;
}

int day_from_days(int z)
{
	z += 719468;
	const int era = (z >= 0 ? z : z - 146096) / 146097;
	const unsigned doe = (unsigned)(z - era * 146097);          // [0, 146096]
	const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
	const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
	const unsigned mp = (5 * doy + 2) / 153;                                   // [0, 11]
	const unsigned d = doy - (153 * mp + 2) / 5 + 1;                             // [1, 31]

	return d;
}
