//
// Created by vane4ka on 10.04.2022.
//

#include "endian_format_utils.h"

uint32_t endian_swap(uint32_t x)
{
	return (x >> 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | (x << 24);
}