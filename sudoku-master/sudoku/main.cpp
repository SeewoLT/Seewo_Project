#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include<cstdio>

#include<iostream>
#include<fstream>

#include<cstdlib>
#include <tchar.h>

#include<algorithm>

#include"getopt.h"

using namespace std;

int Plate[9][9];

bool Flag;

FILE* INPUT_SOLVE, *OUTPUT_SOLVE;



bool CHECK(int x, int m, int n)
{
	int i, j;
	for (i = 0; i < 9; i++)
		if (Plate[m][i] == x) return false;
	for (i = 0; i < 9; i++)
		if (Plate[i][n] == x) return false;

	int begin_i = m / 3 * 3;
	int begin_j = n / 3 * 3;

	for (i = begin_i; i < begin_i + 3; i++)
		for (j = begin_j; j < begin_j + 3; j++)
			if (Plate[i][j] == x) return false;

	return true;
}

void WriteFILE() {
	int i, j;

	for (i = 0; i < 9; i++)
	{
		fputc(Plate[i][0]+ '0', OUTPUT_SOLVE);
		for (j = 1; j < 9; j++)
		{
			fputc(' ', OUTPUT_SOLVE);
			fputc(Plate[i][j]+ '0', OUTPUT_SOLVE);
		}
		fputc('\n', OUTPUT_SOLVE);
	}

	fputc('\n', OUTPUT_SOLVE);

	return;
}

void SOLVE(int count)
{
	if (Flag) return;

	if (count == 81)
	{
		Flag = true;

		WriteFILE();

		return;
	}

	int Hang = count / 9;
	int Lie = count % 9;

	if (Plate[Hang][Lie] == 0)
	{
		for(int i = 1; i <= 9; i++)
			if (CHECK(i, Hang, Lie))
			{
				Plate[Hang][Lie] = i;
				SOLVE(count + 1);
			}

		Plate[Hang][Lie] = 0;
	}
	else SOLVE(count + 1);

	return;
}

inline int TO_INTEGER(char* arg)
{
	int ret = 0;
	while (*arg)
	{
		if (!isdigit(*arg)) return -1;
		ret = ret * 10 + *arg - '0';
		arg++;
	}
	return ret;
}

void Create_Sudoku(int NUM)
{
	FILE* OUTPUT_CREATE;
	OUTPUT_CREATE = fopen("sudoku.txt", "w");

	if (!OUTPUT_CREATE)
	{
		printf("CANNOT open the output file!\n");
		exit(1);
	}

	int SHIFT[9] = { 0,3,6,1,4,7,2,5,8 };

	for (int i = 0; i < 6 && NUM; i++)
	{
		if (i)
		{			
			next_permutation(SHIFT + 3, SHIFT + 6);
		}


		for (int j = 0; j < 6 && NUM; j++)
		{
			if (j)
			{
				next_permutation(SHIFT + 6, SHIFT + 9);
			}

			char row[10] = "712345689";
			for (int k = 0; k < 40320 && NUM; k++)
			{
				if (k)
				{
					next_permutation(row + 1, row + 9);
				}

				for (int r = 0; r < 9; r++)
				{
					fputc(row[SHIFT[r] % 9], OUTPUT_CREATE);

					for (int c = 1; c < 9; c++)
					{
						fputc(' ', OUTPUT_CREATE);
						fputc(row[(c + SHIFT[r]) % 9], OUTPUT_CREATE);
					}

					fputc('\n', OUTPUT_CREATE);
				}

				if (--NUM)
				{
					fputc('\n', OUTPUT_CREATE);
				}

			}
		}
		SHIFT[6] = 2, SHIFT[7] = 5, SHIFT[8] = 8;
	}

	fclose(OUTPUT_CREATE);

	return;
}

int main(int argc, char* argv[])
{
	int opt = getopt(argc, argv, "c:s:");

	if (opt == 'c')
	{
		int n = TO_INTEGER(optarg);

		if (n > 0 && n <= 1000000)
			Create_Sudoku(n);
		else 
			printf("Please input create number from 1 to 1000000!\n");

	}

	else if (opt == 's')
	{
		INPUT_SOLVE = fopen(optarg, "r");
		OUTPUT_SOLVE = fopen("sudoku.txt", "w");

		if (!INPUT_SOLVE)
		{
			printf("CANNOT find the input file!\n");
			exit(1);
		}

		if (!OUTPUT_SOLVE)
		{
			printf("CANNOT open the output file!\n");
			exit(1);
		}

		while (fscanf(INPUT_SOLVE, "%d", &Plate[0][0]) != EOF)
		{
			for (int j = 1; j < 9; j++)
				fscanf(INPUT_SOLVE, "%d", &Plate[0][j]);

			for (int i = 1; i < 9; i++)
				for (int j = 0; j < 9; j++)
					fscanf(INPUT_SOLVE, "%d", &Plate[i][j]);

			Flag = false;
			SOLVE(0);
		}

		fclose(INPUT_SOLVE);
		fclose(OUTPUT_SOLVE);
	}

	else printf("INPUT ERROR!\n");

    return 0;
}