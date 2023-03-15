//
// Created by vane4ka on 02.04.2022.
//

#include "png_utils/chunk_utils.h"
#include "return_codes.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(ZLIB)

	#include <zlib.h>

unsigned char *uncompress_idat(size_t size, chunk_t *idat)
{
	unsigned char *full_data = malloc(size);
	if (!full_data)
		return NULL;
	int ret = uncompress(full_data, &size, idat->data, (unsigned long)idat->length);
	if (ret != Z_OK)
	{
		fprintf(stderr, "Uncompression error in zlib function. Probably there are some mistakes in your png \n");
		if (ret == Z_MEM_ERROR)
		{
			fprintf(stderr, "Not enough memory to uncompress\n");
		}

		free(full_data);
		full_data = NULL;
	}

	return full_data;
}

#elif defined(LIBDEFLATE)

	#include <libdeflate.h>

unsigned char *uncompress_idat(size_t size, chunk_t *idat)
{
	unsigned char *full_data = malloc(size);
	if (!full_data)
		return NULL;
	struct libdeflate_decompressor *Compressor = libdeflate_alloc_decompressor();
	if (!Compressor)
	{
		fprintf(stderr, "NO MEM\n");
		return NULL;
	}
	size_t *tmp = 0;
	int ret = libdeflate_zlib_decompress(Compressor, idat->data, (unsigned long)idat->length, full_data, size, tmp);
	if (ret != LIBDEFLATE_SUCCESS)
	{
		fprintf(stderr, "Uncompression error in libdeflate function. Probably there are some mistakes in your png \n");

		if (ret == LIBDEFLATE_INSUFFICIENT_SPACE)
		{
			fprintf(stderr, "Not enough memory  to uncompress\n");
		}
		free(full_data);
		full_data = NULL;
	}
	free(tmp);
	libdeflate_free_decompressor(Compressor);
	return full_data;
}

#elif defined(ISAL)

	#include <include/igzip_lib.h>
//#include <isa-l.h>

unsigned char *uncompress_idat(size_t size, chunk_t *idat)
{
	struct inflate_state state;
	uint8_t *full_data = (malloc(size));

	isal_inflate_init(&state);
	state.crc_flag = IGZIP_ZLIB;
	state.next_in = (uint8_t *)(idat->data);
	state.avail_in = (uint32_t)(idat->length);
	state.next_out = full_data;
	state.avail_out = (uint32_t)size;

	int ret = isal_inflate_stateless(&state);
	if (ret != ISAL_DECOMP_OK)
	{
		fprintf(stderr, "Uncompression error in isa-l function. Probably there are some mistakes in your png \n");

		if (ret == ISAL_OUT_OVERFLOW)
		{
			fprintf(stderr, "Not enough memory to uncompress\n");
		}

		free(full_data);
		full_data = NULL;
	}
	return full_data;
}

#else
	#error "None of supported compilation flags specified - none of supported libraries were included."

#endif

enum
{
	NONE,
	SUB,
	UP,
	AVERAGE,
	PAETH
};

void write_number(FILE *output_file, uint32_t number)
{
	if (number == 0)
	{
		fwrite("0", sizeof(int8_t), 1, output_file);
		return;
	}
	unsigned int symbols[11];
	unsigned int symbols_count = 0;
	while (number > 0)
	{
		symbols[symbols_count] = number % 10;
		number = number / 10;
		symbols_count++;
	}

	for (int i = 0; i < symbols_count; ++i)
	{
		uint8_t symbol = (unsigned int)'0' + symbols[symbols_count - 1 - i];
		fwrite(&symbol, sizeof(int8_t), 1, output_file);
	}
}

int write_pnm(FILE *output_file, chunks_vector_t *chunks_v)
{
	const char IDAT_name[] = "IDAT";
	int channels;
	int ret = ERROR_SUCCESS;

	header_chunk_t *header = (header_chunk_t *)chunks_v->chunks[0]->data;

	char *pnm_str;

	if (header->color_type == 0)
	{
		pnm_str = "P5\n";
		channels = 1;
	}
	else if (header->color_type == 2)
	{
		pnm_str = "P6\n";
		channels = 3;
	}
	else
	{
		return ERROR_CALL_NOT_IMPLEMENTED;
	}

	size_t row = header->width * channels;
	size_t idat_row = row + 1;
	size_t size = header->height * idat_row;

	uint8_t(*data)[idat_row] = NULL;
	int(*pixels)[row] = malloc((header->height) * row * sizeof(int));

	fwrite(pnm_str, sizeof(int8_t), 3, output_file);
	write_number(output_file, header->width);
	fwrite(" ", sizeof(int8_t), 1, output_file);
	write_number(output_file, header->height);
	fwrite("\n255\n", sizeof(int8_t), 5, output_file);

	for (int i = 0; i < chunks_v->count; ++i)
	{
		if (strncmp((char *)chunks_v->chunks[i]->chunk_type, IDAT_name, 4) == 0)
		{
			data = uncompress_idat(size, chunks_v->chunks[i]);

			if (data == NULL)
			{
				ret = ERROR_INVALID_DATA;
				goto ERR;
			}

			for (int x = 0; x < header->height; ++x)
			{
				uint8_t type;
				type = data[x][0];

				for (int y = 0; y < row; ++y)
				{
					switch (type)
					{
					case NONE:
						pixels[x][y] = (int)data[x][y + 1];
						break;
					case SUB:
						pixels[x][y] = (int)data[x][y + 1] + (y >= channels ? pixels[x][y - channels] : 0);
						break;
					case UP:
						pixels[x][y] = (int)data[x][y + 1] + (x > 0 ? pixels[x - 1][y] : 0);
						break;
					case AVERAGE:
						pixels[x][y] =
							(int)data[x][y + 1] +
							(((y >= channels ? pixels[x][y - channels] : 0) + (x > 0 ? pixels[x - 1][y] : 0)) / 2);
						break;
					case PAETH:
					{
						int a = (y >= channels ? pixels[x][y - channels] : 0);
						int b = (x > 0 ? pixels[x - 1][y] : 0);
						int c = (x > 0 && y >= channels ? (pixels[x - 1][y - channels]) : 0);
						int p = a + b - c;
						int pa = abs((p - a));
						int pb = abs((p - b));
						int pc = abs((p - c));

						if (pa <= pb && pa <= pc)
							pixels[x][y] = (int)data[x][y + 1] + a;
						else if (pb <= pc)
							pixels[x][y] = (int)data[x][y + 1] + b;
						else
							pixels[x][y] = (int)data[x][y + 1] + c;
						break;
					}
					default:
						ret = ERROR_CALL_NOT_IMPLEMENTED;
						goto ERR;
					}
					pixels[x][y] = pixels[x][y] & 0xff;
				}
			}

			for (int x = 0; x < header->height; ++x)
			{
				for (int y = 0; y < row; ++y)
				{
					fwrite((int8_t *)&pixels[x][y], sizeof(int8_t), 1, output_file);
				}
			}
		}
	}

ERR:
	free(data);
	free(pixels);
	return ret;
}

int main(int argc, char *argv[])
{
	const char HEADER[] = "\211PNG\r\n\032\n";
	int ret = ERROR_SUCCESS;
	if (argc != 3)
	{
		fprintf(stderr, "Program needs two file names as arguments instead of %d\n", argc - 1);
		return ERROR_INVALID_PARAMETER;
	}

	FILE *input_file = fopen(argv[1], "rb");

	if (input_file == NULL)
	{
		fprintf(stderr, "The system cannot find the input file %s", argv[1]);
		return ERROR_FILE_NOT_FOUND;
	}

	char sig[8];
	fread(sig, sizeof(int8_t), 8, input_file);
	if (strncmp(sig, HEADER, 8) != 0)
	{
		fprintf(stderr, "The input file %s %s\n", argv[1], " is not a png file");
		ret = ERROR_INVALID_DATA;
		goto RETURN;
	}

	chunks_vector_t chunks_v;

	ret = init_v(&chunks_v);
	if (ret != ERROR_SUCCESS)
	{
		goto RETURN;
	}

	ret = read_chunks(input_file, &chunks_v);
	if (ret != ERROR_SUCCESS)
	{
		goto RETURN;
	}

	header_chunk_t *header = (header_chunk_t *)chunks_v.chunks[0]->data;

	if (header->compression_method != 0)
	{
		fprintf(stderr, "Files with compression are not supported yet");
		ret = ERROR_CALL_NOT_IMPLEMENTED;
		goto RETURN;
	}

	header->width = endian_swap(header->width);
	header->height = endian_swap(header->height);

	if (header->filter_method != 0)
	{
		fprintf(stderr, "Filter method is not implemented");
		ret = ERROR_CALL_NOT_IMPLEMENTED;
		goto RETURN;
	}

	if (header->color_type == 0 || header->color_type == 2)
	{
		FILE *output_file = fopen(argv[2], "wb");
		ret = write_pnm(output_file, &chunks_v);
		fclose(output_file);
	}
	else
	{
		fprintf(stderr, "Color type %d %s", header->color_type, "is not supported.");
		ret = ERROR_CALL_NOT_IMPLEMENTED;
	}

RETURN:
	fclose(input_file);
	destruct_v(&chunks_v);

	return ret;
}