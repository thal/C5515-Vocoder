/*
 * a_Filters.h
 *
 *  Created on: Apr 13, 2016
 *      Author: tom
 */
#include "defs.h"

#ifndef A_FILTERS_H_
#define A_FILTERS_H_

#define A_FILTER_LENGTH (31)

static const int16_t abpf1[A_FILTER_LENGTH] =
{
	-41, -64, -77, -41, 33, 61, -95, -525, -1143,	-1619, -1472, -335, 1764, 4286, 6351, 7149,	6351, 4286, 1764, -335, -1472, -1619, -1143,	-525, -95, 61, 33, -41, -77, -64, -41
};

static const int16_t abpf2[A_FILTER_LENGTH] =
{
	37, 61, -12, -162, -144, 21, -108, -376, 343,
	2022, 1981, -1635, -5359, -3509, 3269, 7152,
	3269, -3509, -5359, -1635, 1981, 2022, 343,
	-376, -108, 21, -144, -162, -12, 61, 37
};

static const int16_t abpf3[A_FILTER_LENGTH] =
{
	-20, -58, 95, 92, -173, -21, -112, 83, 1116,
	-1007, -2412, 3247, 2721, -5920, -1218, 7146,
	-1218, -5920, 2721, 3247, -2412, -1007, 1116,
	83, -112, -21, -173, 92, 95, -58, -20
};

static const int16_t abpf4[A_FILTER_LENGTH] =
{
	-4, 54, -127, 134, -25, -61, -109, 473, -431,
	-713, 2753, -4153, 3077, 742, -5180, 7147,
	-5180, 742, 3077, -4153, 2753, -713, -431,
	473, -109, -61, -25, 134, -127, 54, -4
};



static const int16_t alpf1[A_FILTER_LENGTH] =
{
	-2, -18, -53, -110, -177, -227, -216, -89, 202,
	686, 1351, 2138, 2948, 3655, 4139, 4311, 4139,
	3655, 2948, 2138, 1351, 686, 202, -89, -216,
	-227, -177, -110, -53, -18, -2
};

static const int16_t alpf2[A_FILTER_LENGTH] =
{
	-10, 20, 66, 43, -114, -267, -116, 403, 764,
	216, -1180, -1976, -305, 4127, 9083, 11259,
	9083, 4127, -305, -1976, -1180, 216, 764,
	403, -116, -267, -114, 43, 66, 20, -10
};

static const int16_t alpf3[A_FILTER_LENGTH] =
{
	19, -22, -44, 96, 65, -267, -8, 563, -261,
	-964, 969, 1389, -2685, -1718, 10141, 18223,
	10141, -1718, -2685, 1389, 969, -964, -261,
	563, -8, -267, 65, 96, -44, -22, 19
};

static const int16_t alpf4[A_FILTER_LENGTH] =
{
	-21, 24, -1, -73, 178, -227, 101, 259, -744,
	1037, -721, -500, 2555, -4934, 6848, 25203,
	6848, -4934, 2555, -500, -721, 1037, -744,
	259, 101, -227, 178, -73, -1, 24, -21
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
