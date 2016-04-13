/*
 * s_Filters.h
 *
 *  Created on: Apr 13, 2016
 *      Author: tom
 */
#include "defs.h"

#ifndef S_FILTERS_H_
#define S_FILTERS_H_

#define S_FILTER_LENGTH 51

static const int16_t sbpf1[S_FILTER_LENGTH] =
{
		24, 28, 44, 65, 92, 126, 166, 214, 270,
		333, 404, 482, 565, 654, 746, 839, 932,
		1023, 1110, 1190, 1262, 1324, 1374, 1411,
		1433, 1441, 1433, 1411, 1374, 1324, 1262,
		1190, 1110, 1023, 932, 839, 746, 654,
		565, 482, 404, 333, 270, 214, 166, 126,
		92, 65, 44, 28, 24
};

static const int16_t sbpf2[S_FILTER_LENGTH] =
{
		9, 12, 22, 36, 57, 84, 119, 163, 218, 283,
		359, 445, 541, 646, 758, 875, 994, 1112,
		1227, 1334, 1432, 1517, 1585, 1636, 1668,
		1678, 1668, 1636, 1585, 1517, 1432, 1334,
		1227, 1112, 994, 875, 758, 646, 541, 445,
		359, 283, 218, 163, 119, 84, 57, 36, 22,
		12, 9
};

static const int16_t sbpf3[S_FILTER_LENGTH] =
{
		-42, -45, -64, -84, -102, -118, -125, -121,
		-102, -63, 0, 89, 207, 354, 529, 728, 946,
		1177, 1413, 1645, 1862, 2056, 2217, 2338,
		2414, 2439, 2414, 2338, 2217, 2056, 1862,
		1645, 1413, 1177, 946, 728, 529, 354, 207,
		89, 0, -63, -102, -121, -125, -118, -102,
		-84, -64, -45, -42
};

static const int16_t sbpf4[S_FILTER_LENGTH] =
{
		22, 16, 9, -13, -53, -118, -210, -328, -469,
		-622, -774, -906, -996, -1022, -967, -815,
		-561, -208, 227, 719, 1235, 1736, 2179, 2528,
		2750, 2827, 2750, 2528, 2179, 1736, 1235,
		719, 227, -208, -561, -815, -967, -1022,
		-996, -906, -774, -622, -469, -328, -210,
		-118, -53, -13, 9, 16, 22
};

static int16_t sDelayLine1[S_FILTER_LENGTH + 2] = {0};
static int16_t sDelayLine2[S_FILTER_LENGTH + 2] = {0};
static int16_t sDelayLine3[S_FILTER_LENGTH + 2] = {0};
static int16_t sDelayLine4[S_FILTER_LENGTH + 2] = {0};

const int16_t * sFilters[] = {sbpf1, sbpf2, sbpf3, sbpf4 };
static int16_t * sDelayLines[] = { sDelayLine1, sDelayLine2, sDelayLine3, sDelayLine4 };

#endif /* S_FILTERS_H_ */
