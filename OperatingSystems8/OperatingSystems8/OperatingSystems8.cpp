#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>

int find_bits_in_bitmap(unsigned char* bm_arr, int bmarr_size, int b_count)
{
	unsigned char* navigate = bm_arr;
	int i, j, found = 0, control, flag = 0;

	for (i = 0; i < bmarr_size; i++)
	{
		control = 1;
		for (j = 0; j < 8; j++)
		{
			if ((*navigate & control) == 0) 
				found++;
			else
				found = 0;
			if (found == b_count)
			{
				for (; ; )
				{
					for (; j >= 0 && found >= 0; j--)
					{
						*navigate = *navigate | control;
						control = control >> 1;
						found--;
						if (found == 0) {
							flag = 1;
							break;
						}
					}
					if (flag) break;
					j = 7;
					navigate--;
					i--;
					control = 128;
				}
				return i * 8 + j;
			}
			control = control << 1;
		}
		navigate++;
	}
	return -1;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int i, result, x, BMSIZE;
	unsigned char* bm_arrg;
	printf("Input the bitmap size:");
	scanf_s("%d", &BMSIZE);
	bm_arrg = (unsigned char*)malloc(sizeof(unsigned char*) * BMSIZE);
	for (i = 0; i < BMSIZE; i++)
		bm_arrg[i] = rand() % 128 + 1;
	for (i = 0; i < 5; i++)
	{
		x = rand() % 13 + 1;
		result = find_bits_in_bitmap(bm_arrg, BMSIZE, x);
		if (result == -1)
		{
			printf("There is not enough memory for your request (%d bits)\n", x);
		}
		else
		{
			printf("Memory allocated started from address: %d(%dbits)\n", result, x);
		}
	}
	system("pause");

}