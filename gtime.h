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

#ifdef __cplusplus 
extern "C" {
#endif

#ifndef __GTIME_H
#define __GTIME_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    time_t seconds;
    int64_t nanoseconds;
} gtime_t ;

typedef int64_t gtime_duration_t;

#define MIN_DURATION (gtime_duration_t)(-1 << 63)
#define MAX_DURATION (gtime_duration_t)(1<<63)-1

#define GTIME_NANOSECOND  (gtime_duration_t)1
#define GTIME_MICROSECOND (gtime_duration_t)(1000 * GTIME_NANOSECOND)
#define GTIME_MILLISECOND (gtime_duration_t)(1000 * GTIME_MICROSECOND)
#define GTIME_SECOND      (gtime_duration_t)(1000 * GTIME_MILLISECOND)
#define GTIME_MINUTE      (gtime_duration_t)(60 * GTIME_SECOND)
#define GTIME_HOUR        (gtime_duration_t)(60 * GTIME_MINUTE)

/**
 * Returns the current local time.
 */
gtime_t
gtime_now();

/**
 * Returns the number of seconds elapsed since January 1, 1970 UTC.
 */
int64_t
gtime_unix(const gtime_t t);

/**
 * Determines if the given year is a leap year.
 */
bool
gtime_is_leap_year(int64_t year);

/**
 * Format the given time to a string with the given format.
 */
void
gtime_format(const gtime_t t, const char *layout, char *out, size_t out_size);

uint8_t
gtime_day(const gtime_t t);

bool
gtime_equal(const gtime_t t1, const gtime_t t2);

gtime_duration_t
gtime_duration_new(const int64_t d);

/**
 * Adds the given duration to the gtime_t value.
 */
gtime_t
gtime_add(const gtime_t t, const gtime_duration_t d);

gtime_duration_t
gtime_sub(const gtime_t t1, const gtime_t t2);

void
gtime_sleep(const gtime_duration_t d);

gtime_duration_t
gtime_since(const gtime_t past);

gtime_duration_t
gtime_until(const gtime_t t);

double
gtime_dur_seconds(const gtime_duration_t d);

double
gtime_dur_milliseconds(const gtime_duration_t d);

double
gtime_dur_nanoseconds(const gtime_duration_t d);

void
gtime_dur_str(const gtime_duration_t d, char *out, size_t out_size);

#endif /* __GTIME_H */
#ifdef __cplusplus
}
#endif
