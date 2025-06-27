/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define _DEFAULT_SOURCE 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "gtime.h"

#define SECONDS_PER_MINUTE 60
#define SECONDS_PER_HOUR   60 * SECONDS_PER_MINUTE
#define SECONDS_PER_DAY    24 * SECONDS_PER_HOUR
#define SECONDS_PER_WEEK   7 * SECONDS_PER_DAY
#define DAYS_PER_400_YEARS (365*400 + 97)

#define MARCH_THRU_DECEMBER (31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31)

// absoluteYears is the number of years we subtract from internal time to get
// absolute time. This value must be 0 mod 400, and it defines the "absolute
// zero instant" mentioned in the "Computations on Times" comment above: March
// 1, -absoluteYears. Dates before the absolute epoch will not compute
// correctly, but otherwise the value can be changed as needed.
#define ABSOLUTE_YEARS = 292277022400

// The year of the zero Time. Assumed by the unixToInternal computation below.
#define INTERNAL_YEAR 1

// Offsets to convert between internal and absolute or Unix times.
#define ABSOLUTE_TO_INTERNAL (int64_t)(-(ABSOLUTE_YEARS*365.2425 + MARCH_THRU_DECEMBER) * SECONDS_PER_DAY)
#define INTERNAL_TO_ABSOLUTE (-ABSOLUTE_TO_INTERNAL)
#define UNIX_TO_INTERNAL (int64_t)((1969*365 + 1969/4 - 1969/100 + 1969/400) * SECONDS_PER_DAY)
#define INTERNAL_TO_UNIX (int64_t)(-unixToInternal)
#define ABSOLUTE_TO_UNIX = (ABSOLUTE_TO_INTERNAL + INTERNAL_TO_UNIX)
#define UNIX_TO_ABSOLUTE = (UNIX_TO_INTERNAL + INTERNAL_TO_ABSOLUTE)
#define WALL_TO_INTERNAL (inte64_t)((1884*365 + 1884/4 - 1884/100 + 1884/400) * SECONDS_PER_DAY)

#define HAS_MONOTONIC (int64_t)(1 << 63)
#define MAX_WALL (WALL_TO_INTERNAL + (1<<3 -1)) // year 2157
#define MIN_WALL WALL_TO_INTERNAL // 1885
#define NSEC_MASK (int64_t)(1<<30)-1
#define NSEC_SHIFT (int64_t)30

gtime_t
gtime_now()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    return (gtime_t){
        .seconds = ts.tv_sec,
        .nanoseconds = ts.tv_nsec
    };
}

int64_t
gtime_unix(const gtime_t t)
{
    return (int64_t)t.seconds;
}

bool
gtime_is_leap_year(int64_t year)
{
    return ((year & 3) == 0 && ((year % 25) != 0 || (year & 15) == 0));
}

void
gtime_format(const gtime_t t, const char *layout, char *out, size_t out_size)
{
    struct tm tm;
    time_t secs = t.seconds;
    localtime_r(&secs, &tm);

    strftime(out, out_size, layout, &tm);
}

uint8_t
gtime_day(const gtime_t t)
{
    struct tm *timeinfo = localtime(&t.seconds);
    return (uint8_t)timeinfo->tm_wday;
}

bool
gtime_equal(const gtime_t t1, const gtime_t t2)
{
    return (t1.seconds == t2.seconds && t1.nanoseconds == t2.nanoseconds);
}

gtime_t
gtime_add(const gtime_t t, const gtime_duration_t d)
{
    int64_t total = (int64_t)t.seconds * 1e9 + t.nanoseconds + d;

    return (gtime_t){
        .seconds = total / (int64_t)1e9,
        .nanoseconds = total % (int64_t)1e9
    };
}

gtime_duration_t
gtime_sub(const gtime_t t1, const gtime_t t2)
{
    int64_t ns1 = (int64_t)t1.seconds * 1e9 + t1.nanoseconds;
    int64_t ns2 = (int64_t)t2.seconds * 1e9 + t2.nanoseconds;

    return (gtime_duration_t)(ns1 - ns2);
}

void
gtime_sleep(const gtime_duration_t d)
{
    struct timespec ts = {
        .tv_sec = d / (int64_t)1e9,
        .tv_nsec = d % (int64_t)1e9
    };
    nanosleep(&ts, NULL);
}

gtime_duration_t
gtime_since(const gtime_t past)
{
    gtime_t now = gtime_now();

    return gtime_sub(now, past);
}

gtime_duration_t
gtime_duration_new(const int64_t d)
{
    return (gtime_duration_t)d;
}

gtime_duration_t
gtime_until(const gtime_t t)
{
    const gtime_t now = gtime_now();
    return (gtime_duration_t)gtime_sub(t, now);
}

double
gtime_dur_seconds(const gtime_duration_t d)
{
    return d / 1e9;
}

double
gtime_dur_milliseconds(const gtime_duration_t d)
{
    return d / 1e6;
}

double
gtime_dur_nanoseconds(const gtime_duration_t d)
{
    return d;
}

void
gtime_dur_str(const gtime_duration_t d, char *out, size_t out_size)
{
    int64_t ns = d;
    char buf[64] = {0};
    char* p = buf;

    if (ns < 0) {
        *p++ = '-';
        ns = -ns;
    }

    int64_t hours = ns / (int64_t)3600e9;
    ns %= (int64_t)3600e9;
    int64_t minutes = ns / (int64_t)60e9;
    ns %= (int64_t)60e9;
    int64_t seconds = ns / (int64_t)1e9;
    ns %= (int64_t)1e9;

    if (hours > 0) {
        p += snprintf(p, sizeof(buf)-(p-buf), "%ldh", hours);
    }

    if (minutes > 0) {
        p += snprintf(p, sizeof(buf)-(p-buf), "%ldm", minutes);
    }

    if (seconds > 0 || ns > 0 || (hours == 0 && minutes == 0)) {
        if (ns == 0) {
            p += snprintf(p, sizeof(buf)-(p-buf), "%lds", seconds);
        } else {
            double s = seconds + ns / 1e9;
            p += snprintf(p, sizeof(buf)-(p-buf), "%.9gs", s);
        }
    }

    strncpy(out, buf, out_size - 1);
    out[out_size - 1] = '\0';
}
