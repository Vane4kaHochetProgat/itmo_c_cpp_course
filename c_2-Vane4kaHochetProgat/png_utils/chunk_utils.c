//
// Created by vane4ka on 10.04.2022.
//

#include "chunk_utils.h"

int init_v(chunks_vector_t *chunks_v)
{
	chunks_v->capacity = 3;
	chunks_v->count = 0;
	chunks_v->chunks = malloc(sizeof(chunk_t *) * chunks_v->capacity);
	if (chunks_v->chunks == NULL)
	{
		fprintf(stderr, "Not enough memory to allocate");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	return ERROR_SUCCESS;
}

void destruct_v(chunks_vector_t *chunks_v)
{
	for (int j = 0; j < chunks_v->count; ++j)
	{
		free(chunks_v->chunks[j]->data);
		free(chunks_v->chunks[j]);
	}
	free(chunks_v->chunks);
}

void ensure_capacity_v(chunks_vector_t *chunks_v)
{
	if (chunks_v->count >= chunks_v->capacity)
	{
		chunks_v->capacity *= 2;
		chunks_v->chunks = realloc(chunks_v->chunks, chunks_v->capacity * sizeof(chunk_t));
	}
}

int read_chunk(FILE *input_file, chunk_t *chunk)
{
	fread(chunk, sizeof(int32_t), 2, input_file);
	chunk->length = endian_swap(chunk->length);

	if (chunk->length != 0)
	{
		chunk->data = malloc(sizeof(char) * chunk->length);
		if (chunk->data == NULL)
		{
			fprintf(stderr, "Not enough memory to allocate");
			return ERROR_OUTOFMEMORY;
		}
		fread(chunk->data, sizeof(char), chunk->length, input_file);
	}
	else
	{
		chunk->data = NULL;
	}

	int32_t CRC;
	fread(&CRC, sizeof(int32_t), 1, input_file);
	return ERROR_SUCCESS;
}

int read_chunks(FILE *input_file, chunks_vector_t *chunks_v)
{
	char *IEND_name = "IEND";
	chunk_t *tmp_chunk = NULL;
	do
	{
		ensure_capacity_v(chunks_v);

		tmp_chunk = malloc(sizeof(chunk_t));
		if (tmp_chunk == NULL)
		{
			fprintf(stderr, "Not enough memory to allocate");
			destruct_v(chunks_v);
			return ERROR_NOT_ENOUGH_MEMORY;
		}

		int ret = read_chunk(input_file, tmp_chunk);
		if (ret != ERROR_SUCCESS)
		{
			destruct_v(chunks_v);
			fprintf(stderr, "Error while reading chunk");
			return ret;
		}
		chunks_v->chunks[chunks_v->count++] = tmp_chunk;
	} while (strncmp((char *)tmp_chunk->chunk_type, IEND_name, 4) != 0);

	return ERROR_SUCCESS;
}