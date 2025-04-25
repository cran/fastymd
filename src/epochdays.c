/* --------------------------------------------------------------------------
URL:  https://github.com/protocolbuffers/protobuf/blob/bdfec4340ca4575a68fad8e49c10354ae242e9b7/upb/json/decode.c#L1096-L1105
BLOG: https://blog.reverberate.org/2020/05/12/optimizing-date-algorithms.html
 -------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------

 Protocol Buffers - Google's data interchange format
 Copyright 2023 Google LLC.  All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 Neither the name of Google Inc. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

-------------------------------------------------------------------------- */

#include "epochdays.h"

#include <stdint.h>

int jsondec_epochdays(int y, int m, int d)
{
	const uint32_t year_base = 4800; /* Before min year, multiple of 400. */
	const uint32_t m_adj = m - 3;    /* March-based month. */
	const uint32_t carry = m_adj > (uint32_t)m ? 1 : 0;
	const uint32_t adjust = carry ? 12 : 0;
	const uint32_t y_adj = y + year_base - carry;
	const uint32_t month_days = ((m_adj + adjust) * 62719 + 769) / 2048;
	const uint32_t leap_days = y_adj / 4 - y_adj / 100 + y_adj / 400;
	return y_adj * 365 + leap_days + month_days + (d - 1) - 2472632;
}
