/*
 * a_Filters.h
 *
 *  Created on: Apr 13, 2016
 *      Author: tom
 */
#include "defs.h"

#ifndef A_FILTERS_H_
#define A_FILTERS_H_

#define A_FILTER_LENGTH 21

static const int16_t abpf1[A_FILTER_LENGTH] =
{
		0, 34, 146, 392, 818, 1438, 2214, 3048, 3802,
		4329, 4518, 4329, 3802, 3048, 2214, 1438,
		818, 392, 146, 34, 0
};

static const int16_t abpf2[A_FILTER_LENGTH] =
{
		-86, -236, -461, -643, -569, 0, 1195, 2905,
		4746, 6175, 6714, 6175, 4746, 2905, 1195, 0,
		-569, -643, -461, -236, -86
};

static const int16_t abpf3[A_FILTER_LENGTH] =
{
		0, 39, 67, -189, -1077, -2427, -3111, -1673,
		2119, 6377, 8253, 6377, 2119, -1673, -3111,
		-2427, -1077, -189, 67, 39, 0
};

static const int16_t abpf4[A_FILTER_LENGTH] =
{
		0, -22, 226, 463, -203, 0, 585, -4104, -7156,
		3693, 13157, 3693, -7156, -4104, 585, 0, -203,
		463, 226, -22, 0
};



static const int16_t alpf1[A_FILTER_LENGTH] =
{
		54, 150, 332, 617, 1012, 1501, 2044, 2579,
		3033, 3338, 3446, 3338, 3033, 2579, 2044,
		1501, 1012, 617, 332, 150, 54
};

static const int16_t alpf2[A_FILTER_LENGTH] =
{
		23, 80, 214, 459, 843, 1368, 1995, 2651,
		3231, 3632, 3776, 3632, 3231, 2651, 1995,
		1368, 843, 459, 214, 80, 23
};

static const int16_t alpf3[A_FILTER_LENGTH] =
{
		-22, -57, -90, -49, 180, 716, 1604, 2756,
		3936, 4829, 5162, 4829, 3936, 2756, 1604,
		716, 180, -49, -90, -57, -22
};

static const int16_t alpf4[A_FILTER_LENGTH] =
{
		0, 50, 139, 93, -327, -964, -944, 821,
		4406, 8194, 9831, 8194, 4406, 821, -944,
		-964, -327, 93, 139, 50, 0
};


static int16_t aBpfDelayLine1[A_FILTER_LENGTH + 2] = {0};
static int16_t aBpfDelayLine2[A_FILTER_LENGTH + 2] = {0};
static int16_t aBpfDelayLine3[A_FILTER_LENGTH + 2] = {0};
static int16_t aBpfDelayLine4[A_FILTER_LENGTH + 2] = {0};

static int16_t aLpfDelayLine1[A_FILTER_LENGTH + 2] = {0};
static int16_t aLpfDelayLine2[A_FILTER_LENGTH + 2] = {0};
static int16_t aLpfDelayLine3[A_FILTER_LENGTH + 2] = {0};
static int16_t aLpfDelayLine4[A_FILTER_LENGTH + 2] = {0};

const int16_t * aBpfFilters[] = {abpf1, abpf2, abpf3, abpf4 };
static int16_t * aBpfDelayLines[] = { aBpfDelayLine1, aBpfDelayLine2, aBpfDelayLine3, aBpfDelayLine4 };

const int16_t * aLpfFilters[] = {alpf1, alpf2, alpf3, alpf4 };
static int16_t * aLpfDelayLines[] = { aLpfDelayLine1, aLpfDelayLine2, aLpfDelayLine3, aLpfDelayLine4 };
#endif /* A_FILTERS_H_ */
