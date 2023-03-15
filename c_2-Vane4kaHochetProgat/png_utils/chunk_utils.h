//
// Created by vane4ka on 10.04.2022.
//

#ifndef C_2_VANE4KAHOCHETPROGAT_CHUNK_UTILS_H
#define C_2_VANE4KAHOCHETPROGAT_CHUNK_UTILS_H

#include "png_utils/endian_format_utils.h"
#include "return_codes.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	uint32_t length;
	unsigned char chunk_type[4];
	unsigned char *data;
} chunk_t;

typedef struct
{
	size_t capacity;
	size_t count;
	chunk_t **chunks;
} chunks_vector_t;

typedef struct
{
	uint32_t width;
	uint32_t height;
	unsigned char bit_depth;
	unsigned char color_type;
	unsigned char compression_method;
	unsigned char filter_method;
	unsigned char interlace_method;
} header_chunk_t;

int init_v(chunks_vector_t *chunks_v);

void destruct_v(chunks_vector_t *chunks_v);

void ensure_capacity_v(chunks_vector_t *chunks_v);

int read_chunk(FILE *input_file, chunk_t *chunk);

int read_chunks(FILE *input_file, chunks_vector_t *chunks_v);

#endif