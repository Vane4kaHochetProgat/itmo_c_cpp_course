#include "return_codes.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int matrix_builder(float **matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		matrix[i] = malloc(sizeof(float) * (size + 1));
		if (matrix[i] == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(matrix[j]);
			}
			return -1;
		}
	}
	return 1;
}

// void print_matrix(float **matrix, int size)
//{
//	printf("\n");
//	for (int i = 0; i < size; ++i)
//	{
//		for (int j = 0; j < size + 1; ++j)
//		{
//			printf("%15.2f ", matrix[i][j]);
//		}
//		printf("\n");
//	}
// }

int equals(float a, float b)
{
	const float delta = (float)2e-38;
	return fabsf(b - a) <= delta * fabsf(b);
}

void free_matrix(float **matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (matrix[i] != NULL)
		{
			free(matrix[i]);
		}
	}
	free(matrix);
}

void swap_lines(float **matrix, int size, int a, int b)
{
	for (int v = 0; v < size + 1; v++)
	{
		float temp = matrix[a][v];
		matrix[a][v] = matrix[b][v];
		matrix[b][v] = temp;
	}
}

void diagonalize_line(float **matrix, int size, int line_index, int variable_index, float mult)
{
	for (int l = line_index + 1; l < size; l++)
	{
		if (equals(matrix[l][variable_index], 0))
		{
			continue;
		}
		float k = -matrix[l][variable_index] / mult;
		for (int v = variable_index; v < size + 1; v++)
		{
			matrix[l][v] += k * matrix[line_index][v];
		}
	}
}

int gauss_method(float **matrix, int size)
{
	int line_index = 0;
	for (int j = 0; j < size; j++)
	{
		if (equals(matrix[line_index][j], 0))
		{
			int nonzero_index = line_index + 1 - size;
			while (nonzero_index < 0 && equals(matrix[nonzero_index + size][line_index], 0))
			{
				nonzero_index++;
			}
			if (nonzero_index)
			{
				swap_lines(matrix, size, nonzero_index + size, line_index);
			}
			else
			{
				continue;
			}
		}
		diagonalize_line(matrix, size, line_index, j, matrix[line_index][j]);
		line_index++;
	}
	if (line_index < size)
	{
		for (; line_index < size; line_index++)
		{
			if (!equals(matrix[line_index][size], 0))
			{
				return 1;
			}
		}
		return 2;
	}

	for (int i = size - 1; i >= 0; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			matrix[j][size] -= matrix[j][i] / matrix[i][i] * matrix[i][size];
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
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

	int N = 0;

	// The correctness of the input data is guaranteed, N is a natural number
	fscanf(input_file, "%d", &N);
	float **matrix = malloc(sizeof(float *) * N);
	if (matrix == NULL)
	{
		fprintf(stderr, "Error occurs while allocating memory");
		fclose(input_file);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	if (matrix_builder(matrix, N) == -1)
	{
		fclose(input_file);
		free(matrix);
		fprintf(stderr, "Error occurs while allocating memory");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N + 1; j++)
		{
			// The correctness of the input data is guaranteed, matrix element is a float number
			fscanf(input_file, "%f", &matrix[i][j]);
		}
	}

	fclose(input_file);

	int result = gauss_method(matrix, N);
	FILE *output_file = fopen(argv[2], "w");

	if (output_file == NULL)
	{
		fprintf(stderr, "The program cannot write in output file %s", argv[2]);
		free_matrix(matrix, N);
		return ERROR_FILE_NOT_FOUND;
	}
	//	print_matrix(matrix, N);
	if (result == 0)
	{
		for (int i = 0; i < N; i++)
		{
			fprintf(output_file, "%g %s", matrix[i][N] / matrix[i][i], "\n");
		}
	}
	else if (result == 1)
	{
		fprintf(output_file, "no solution");
	}
	else
	{
		fprintf(output_file, "many solutions");
	}

	free_matrix(matrix, N);
	fclose(output_file);
	return ERROR_SUCCESS;
}