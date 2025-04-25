/* --------------------------------------------------------------------------
 This is a C implementation of the days_from_civil code documented by Howard Hinnant
 in the article "chrono-Compatible Low-Level Date Algorithm":
 https://howardhinnant.github.io/date_algorithms.html#days_from_civil

 The C implementation came from a post by Thiago Adams:
 https://thradams.com/date.html

 The original code by Howard is from his date (https://github.com/HowardHinnant/date)
 library and released under the MIT licence below:

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

#include "days_from_civil.h"

int days_from_civil(int y, unsigned m, unsigned d)
{
	y -= m <= 2;
	const int era = (y >= 0 ? y : y - 399) / 400;
	const unsigned yoe = (unsigned)(y - era * 400);                       // [0, 399]
	const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;  // [0, 365]
	const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;           // [0, 146096]
	return era * 146097 + (int)(doe) - 719468;
}
