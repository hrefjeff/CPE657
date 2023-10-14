#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

float calculateEntropy(long byte_count[], int length);

int main(int argc, char* argv[])
{
	int length = 0; // length of file
	FILE * file;
	long byte_count[256];
	unsigned char buffer[1024];

	memset(byte_count, 0, sizeof(long) * 256);
	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		return 1; // just don't let this happen lol
	}

	int n;
	while ((n = fread(buffer, 1, 1024, file)) != 0)
	{
		int i;
		for (i = 0; i < n; i++)
		{
			byte_count[(int) buffer[i]]++;
			length++;
		}
	}

	fclose(file);
	float entropy = calculateEntropy(byte_count, length);
	printf("%02.5f \t%s\n", entropy, argv[1]);
	return 0;
}

// derived from Shannon's entropy formula
float calculateEntropy(long byte_count[], int length)
{
	float entropy = 0;
	float count = 0;
	int i;

	for (i = 0; i < 256; i++)
	{
		if (byte_count[i] !=0)
		{
			count = (float) byte_count[i] / (float) length;
			entropy += -count * log2f(count);
		}
	}

	return entropy;
}
