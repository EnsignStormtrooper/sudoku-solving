// sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"

struct sRow {
	int dat[9];
	int empty[9];
};

struct sCol {
	int dat[9];
	int empty[9];
};

struct sSect {
	int dat[3][3];
};

struct possible {
	int pos[9][10];
};

struct sudokuGrid {
	sRow row[9];
	int rowT[9];
	sCol col[9];
	int colT[9];
	sSect sect[3][3];
	int sectT[3][3];
	possible posN[9];
};

sudokuGrid initGrid() {
	sudokuGrid out;
	int dat[81] = { 9,0,0,2,0,0,6,1,0,
									0,7,3,0,0,0,0,0,0,
									0,0,0,4,0,0,2,5,0,
									0,4,7,0,1,2,0,6,5,
									0,2,6,5,0,9,3,7,0,
									5,3,0,7,8,0,1,4,0,
									0,6,5,0,0,4,0,0,0,
									0,0,0,0,0,0,7,9,0,
									0,9,8,0,0,7,0,0,4 };
	int i, j = 0;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			out.row[i].dat[j] = dat[9 * i + j];
			if (out.row[i].dat[j] == 0) {
				out.row[i].empty[j] = 1;
			}
			else {
				out.row[i].empty[j] = 0;
			}
			out.col[i].dat[j] = dat[9 * j + i];
			if (out.col[i].dat[j] == 0) {
				out.col[i].empty[j] = 1;
			}
			else {
				out.col[i].empty[j] = 0;
			}
		}
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					out.sect[i][j].dat[k][l] = out.row[j * 3 + l].dat[3 * i + k];
				}
			}
		}
	}
	return out;
}

void rowTotal(sudokuGrid *mat, int pos) {
	int T = 0;
	for (int i = 0; i < 9; i++) {
		T += mat->row[pos].dat[i];
	}
	mat->rowT[pos] = T;
}

void colTotal(sudokuGrid *mat, int pos) {
	int T = 0;
	for (int i = 0; i < 9; i++) {
		T += mat->col[pos].dat[i];
	}
	mat->colT[pos] = T;
}

void sectTotal(sudokuGrid *mat, int x, int y) {
	int T = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			T += mat->sect[x][y].dat[i][j];
		}
	}
	mat->sectT[x][y] = T;
}

void checkRow(sudokuGrid *mat, int x, int y) {
	mat->posN[y].pos[x][0] = 900;
	for (int i = 1; i < 10; i++) {
		mat->posN[y].pos[x][i] = 1;
	}
	for (int i = 0; i < 9; i++) {
		if (mat->row[y].empty[i] == 0 && mat->posN[y].pos[x][mat->row[y].dat[i]] == 1) {
			mat->posN[y].pos[x][mat->row[y].dat[i]] = 0;
			mat->posN[y].pos[x][0] -= 100;
		}
		/*if (mat->row[y].empty[i] == 1) {
			mat->posN[y].pos[x][9] -= 100;
		}*/
		if (mat->col[x].empty[i] == 0 && mat->posN[y].pos[x][mat->col[x].dat[i]] == 1) {
			mat->posN[y].pos[x][mat->col[x].dat[i]] = 0;
			mat->posN[y].pos[x][0] -= 100;
		}
		/*if (mat->col[x].empty[i] == 1) {
			mat->posN[y].pos[x][9] -= 10;
		}*/
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (mat->sect[x / 3][y / 3].dat[i][j] != 0 && mat->posN[y].pos[x][mat->sect[x / 3][y / 3].dat[i][j]] == 1) {
				mat->posN[y].pos[x][mat->sect[x/3][y/3].dat[i][j]] = 0;
				mat->posN[y].pos[x][0] -= 100;
			}
			/*if (mat->sect[x/3][y/3].dat[i][j] == 0) {
				mat->posN[y].pos[x][9] -= 1;
			}*/
		}
	}
}

void setSo(sudokuGrid *mat, int x, int y) {
	int j = 9;
	for (int i = 1; i < 9; i++) {
		if (mat->posN[y].pos[x][i] == 1) {
			j = i;
		}
	}
	mat->row[y].dat[x] = j;
	mat->row[y].empty[x] = 0;
	mat->col[x].dat[y] = j;
	mat->col[x].empty[y] = 0;
	mat->sect[x / 3][y / 3].dat[x % 3][y % 3] = j;
}

void solve(sudokuGrid *mat) {
	//int remainderR[9], remainderC[9];
	//int remainderS[3][3];
	//for (int i = 0; i < 9; i++) {
	//	rowTotal(mat, i);
	//	remainderR[i] = 45 - mat->rowT[i];
	//	colTotal(mat, i);
	//	remainderC[i] = 45 - mat->colT[i];
	//	sectTotal(mat, i % 3, i / 3);
	//	remainderS[i % 3][i / 3] = 45 - mat->sectT[i % 3][i / 3];
	//}
	//for (int i = 0; i < 9; i++) {
	//	for (int j = 0; j < 9; j++) {

	//	}
	//}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++){
			//if (remainderR[i] > 0 && remainderC[j] > 0 && remainderS[j/3][i/3] > 0 && mat->row[i].empty[j] == 1) {
			//	mat->row[i].dat[j] += 1;
			//	mat->col[j].dat[i] += 1;
			//	mat->sect[j / 3][i / 3].dat[j%3][i%3] += 1;
			//	rowTotal(mat, i);
			//	colTotal(mat, j);
			//	sectTotal(mat, j / 3, i / 3);
			//	remainderR[i] -= 1;
			//	remainderC[j] -= 1;
			//	remainderS[j / 3][i / 3] -= 1;
			if (mat->row[j].empty[i] == 1) {
				checkRow(mat, i, j);
				if (mat->posN[j].pos[i][0] == 100) {
					setSo(mat, i, j);
				}
			}
		}
	}
	bool remaining = true;
	//for (int i = 0; i < 9; i++) {
	//	if (remainderR[i] > 0 || remainderC[i] > 0) {
	//		remaining = true;
	//	}
	//}
	if (remaining == true) {
		solve(mat);
	}
}

int main()
{
	sudokuGrid matrix;
	matrix = initGrid();
	solve(&matrix);
	getchar();
    return 0;
}
