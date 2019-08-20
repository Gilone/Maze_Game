#include"acllib.h"
#include <iostream>  
#include <stdlib.h>
#include <time.h>
#include<windows.h>
#define MAXCLO 24
using namespace std;
const int mWindowWidth = 1200, mWindowHeight = 1000;
const int w1Width = 4, w1Height = 16;
const int w2Width = 16, w2Height = 4;
const int zhuWidth = 16, zhuHeight = 16;
const int pathWidth = 14, pathHeight = 14;
const int blockw = 16, blockh = 16;
int map[2 * MAXCLO + 1][2 * MAXCLO + 1];  //标记通路状态
int mark[(2 * MAXCLO + 1)*(2 * MAXCLO + 1)];  //标记到达过
int mark2[(2 * MAXCLO + 1)*(2 * MAXCLO + 1)];  //标记到达过
const char *name = "wall1.jpg";
const char *name2 = "zhu.jpg";
const char *name3 = "path1.jpg";
const char *name4 = "path2.jpg";
char mapp[MAXCLO * 2 + 1][MAXCLO * 2 + 1];


typedef struct SqStack {
	int data[(2 * MAXCLO + 1)*(2 * MAXCLO + 1)];
	int top;
}SqStack;

SqStack mppp, mppp2;
SqStack*mp = &mppp;
SqStack*mp2 = &mppp2;


void initSqStack(SqStack *st) {
	st->top = -1;
}

int SqStackEmpty(SqStack *st) {
	return (st->top == -1 ? 1 : 0);
}

int push(SqStack *st, int x) {
	if (st->top == (2 * MAXCLO + 1)*(2 * MAXCLO + 1) - 1) {
		return 0;
	}
	st->data[++st->top] = x;
	return 1;
}

int pop(SqStack *st) {
	if (st->top == -1) {
		return 0;
	}
	int x = st->data[st->top--];
	return 1;
}

void printStack(SqStack *st) {
	while (st->top != -1) {
		printf("栈元素：%d\n", st->data[st->top--]);
	}
}

class pic {
protected:
	ACL_Image img;
	int x, y;
	int width, height;
public:
	pic(const char *name, int w, int h, int ax, int ay) {
		loadImage(name, &img);
		width = w;
		height = h;
		x = ax;
		y = ay;
	}
	void drawImageScale()
	{
		putImageScale(&img, x, y, width, height);
	}
};

pic *wall1[(2 * MAXCLO + 1)*(2 * MAXCLO + 1)] = { NULL };
pic *wall2[(2 * MAXCLO + 1)*(2 * MAXCLO + 1)] = { NULL };
pic *zhu[(2 * MAXCLO + 1)*(2 * MAXCLO + 1)] = { NULL };
pic *path1[(2 * MAXCLO + 1)*(2 * MAXCLO + 1)] = { NULL };
pic *path2[(2 * MAXCLO + 1)*(2 * MAXCLO + 1)] = { NULL };

class UF
{
public:
	int *id; // access to component id (site indexed)  
	int count; // number of components  
	int *sz;
	UF(int N)
	{			// Initialize component id array.  
		count = N;
		id = new int[N];
		sz = new int[N];
		for (int i = 0; i < N; i++) {
			id[i] = i;
			sz[i] = i;
		}
	}
	~UF() {
		delete[] id;
	}
	int count1()
	{
		return count;
	}
	bool connected(int p, int q)
	{
		return find(p) == find(q);
	}
	int find(int p)
	{
		while (p != id[p]) {
			id[p] = id[id[p]];
			p = id[p];
		}
		return p;
	}
	void merge(int p, int q)
	{
		int i = find(p);
		int j = find(q);
		if (i == j) return;
		// 将小树作为大树的子树  
		if (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
		else { id[j] = i; sz[i] += sz[j]; }
		count--;
	}
};

bool isvalidpoint(int row, int col)
{
	if (map[row][col] == 0 || mark[col + row*(2 * MAXCLO + 1)] == 1 || row<0 || col<0 || row == 2 * MAXCLO + 1 || col == 2 * MAXCLO + 1)return false;
	else return true;
}
bool isvalidpoint2(int row, int col)
{
	if (map[row][col] == 0 || mark2[col + row*(2 * MAXCLO + 1)] == 1 || row<0 || col<0 || row == 2 * MAXCLO + 1 || col == 2 * MAXCLO + 1)return false;
	else return true;
}

bool findnext(int row, int col) {
	push(mp, col + row*(2 * MAXCLO + 1));
	if (row == (2 * MAXCLO - 1) && col == (2 * MAXCLO - 1))return true;
	else if (isvalidpoint(row, col) == false)
	{
		pop(mp);
		return false;
	}
	mark[col + row*(2 * MAXCLO + 1)] = 1;
	if (findnext(row + 1, col) || findnext(row, col + 1) || findnext(row - 1, col) || findnext(row, col - 1))return true;
	else {
		pop(mp);
		return false;
	}
}

bool findnext2(int row, int col) {
	push(mp2, col + row*(2 * MAXCLO + 1));
	if (row == (1) && col == (2 * MAXCLO - 1))return true;
	else if (isvalidpoint2(row, col) == false)
	{
		pop(mp2);
		return false;
	}
	mark2[col + row*(2 * MAXCLO + 1)] = 1;
	if (findnext2(row , col+1) || findnext2(row-1, col ) || findnext2(row + 1, col) || findnext2(row, col - 1))return true;
	else {
		pop(mp2);
		return false;
	}
}

void keyevent(int key, int event) {
	if (event != KEY_DOWN)return;
	char tishi1[200] = { " 你做对了吗？" };
	switch (key)
	{
	case VK_SPACE:
		beginPaint();
		clearDevice();
		setPenColor(BLACK);
		setBrushColor(WHITE);
		setPenWidth(20);
		paintText(900, 200, tishi1);
		for (int i = 0; i<2 * MAXCLO + 1; ++i) {
			for (int j = 0; j < 2 * MAXCLO + 1; ++j) {
				if (mapp[i][j] == '+') {
					zhu[i*(2 * MAXCLO + 1) + j] = new pic(name2, zhuWidth, zhuHeight, 10 + i*blockh,  j*blockh);
					zhu[i*(2 * MAXCLO + 1) + j]->drawImageScale();
				}
				else if (mapp[i][j] == '-') {
					wall1[i*(2 * MAXCLO + 1) + j] = new pic(name, w1Width, w1Height, 16 + i*blockh, j*blockh);
					wall1[i*(2 * MAXCLO + 1) + j]->drawImageScale();
				}
				else if (mapp[i][j] == '|') {
					wall2[i*(2 * MAXCLO + 1) + j] = new pic(name, w2Width, w2Height, 10 + i*blockh, 6+ j*blockh);
					wall2[i*(2 * MAXCLO + 1) + j]->drawImageScale();
				}
				else if (mapp[i][j] == '*') {
					path1[i*(2 * MAXCLO + 1) + j] = new pic(name3, pathWidth, pathHeight, 10 + i*blockh,  j*blockh);
					path1[i*(2 * MAXCLO + 1) + j]->drawImageScale();
				}
				else if (mapp[i][j] == 'o') {
					path2[i*(2 * MAXCLO + 1) + j] = new pic(name4, pathWidth, pathHeight, 10 + i*blockh,  j*blockh);
					path2[i*(2 * MAXCLO + 1) + j]->drawImageScale();
				}
			}
		}
		endPaint();
		break;
	default:
		break;
	}
}

int Setup()
{
	srand((unsigned)time(NULL));
	initWindow("maze", 0, 0, mWindowWidth, mWindowHeight);
	char wall_row[MAXCLO + 1][MAXCLO];
	char wall_col[MAXCLO][MAXCLO + 1];
	memset(wall_row, '-', (MAXCLO + 1)*MAXCLO * sizeof(char));
	memset(wall_col, '|', MAXCLO*(MAXCLO + 1) * sizeof(char));
	memset(map, 0, (2 * MAXCLO + 1)*(2 * MAXCLO + 1) * sizeof(int)); //全部不通
	memset(mark, 0, (2 * MAXCLO + 1)*(2 * MAXCLO + 1) * sizeof(int));
	memset(mark2, 0, (2 * MAXCLO + 1)*(2 * MAXCLO + 1) * sizeof(int));
	registerKeyboardEvent(keyevent);
	initSqStack(mp);
	initSqStack(mp2);
	UF MAZE(MAXCLO*MAXCLO);


	while (!(MAZE.connected(0, (MAXCLO*MAXCLO) - 1) && MAZE.connected(MAXCLO*MAXCLO - MAXCLO + 1, MAXCLO - 1))) {                    //build a maze,two start,two end
		int a = rand() % 2;
		if (a == 0) {
			int i = rand() % MAXCLO;
			int j = 1 + rand() % (MAXCLO - 1);
			wall_col[i][j] = ' ';
			MAZE.merge(((i)*MAXCLO) + (j)-1, ((i)*MAXCLO) + (j));
		}
		else {
			int i = rand() % MAXCLO;
			int j = 1 + rand() % (MAXCLO - 1);
			wall_row[j][i] = ' ';
			MAZE.merge((j - 1)*MAXCLO + i, j*MAXCLO + i);
		}
	}

	for (int i = 1; i < (2 * MAXCLO + 1); i += 2) {
		for (int j = 1; j < (2 * MAXCLO + 1); j += 2) {
			map[i][j] = 1;
		}
	}

	for (int i = 1; i < (2 * MAXCLO + 1); i += 2) {
		for (int j = 2; j < (2 * MAXCLO - 1); j += 2) {
			if (wall_col[(i - 1) / 2][j / 2] == ' ')map[i][j] = 1;
			if (wall_row[j / 2][(i - 1) / 2] == ' ')map[j][i] = 1;
		}
	}



	for (int i = 0; i < 2 * MAXCLO + 1; ++i) {
		if (i % 2 == 0) {
			for (int j = 0; j < 2 * MAXCLO + 1; ++j) {
				if (j % 2 == 0) {
					mapp[i][j] = '+';
				}
				else {
					mapp[i][j] = wall_row[i / 2][(j - 1) / 2];
				}
			}
		}
		else {
			for (int j = 0; j < 2 * MAXCLO + 1; ++j) {
				if (j % 2 == 0) {
					mapp[i][j] = wall_col[(i - 1) / 2][j / 2];
				}
				else {
					mapp[i][j] = ' ';
				}

			}
		}
	}

	/*		cout << "\n\t"<<"生成的迷宫:\n" << endl;
	for (int i = 0; i < 2 * MAXCLO + 1; ++i) {
	cout << "\t\t\t";
	for (int j = 0; j < 2 * MAXCLO + 1; ++j) {
	cout << mapp[i][j];
	}
	cout << endl;
	}
	cout << "\n\n\n" << endl;*/



	beginPaint();
	clearDevice();
	setPenColor(BLACK);
	setBrushColor(WHITE);
	char tishi1[200] = { " 迷宫 请找出连通两条对角线的路径" };
	char tishi2[200] = { "按空格键查看答案" };
	paintText(900, 200, tishi1);
	paintText(960, 250, tishi2);

	for (int i = 0; i<2 * MAXCLO + 1; ++i) {
		for (int j = 0; j < 2 * MAXCLO + 1; ++j) {
			if (mapp[i][j] == '+') {
				zhu[i*(2 * MAXCLO + 1) + j] = new pic(name2, zhuWidth, zhuHeight, 10 + i*blockh, j*blockh);
				zhu[i*(2 * MAXCLO + 1) + j]->drawImageScale();
			}
			else if (mapp[i][j] == '-') {
				wall1[i*(2 * MAXCLO + 1) + j] = new pic(name, w1Width, w1Height, 16 + i*blockh,  j*blockh);
				wall1[i*(2 * MAXCLO + 1) + j]->drawImageScale();
			}
			else if (mapp[i][j] == '|') {
				wall2[i*(2 * MAXCLO + 1) + j] = new pic(name, w2Width, w2Height, 10 + i*blockh, 6+ j*blockh);
				wall2[i*(2 * MAXCLO + 1) + j]->drawImageScale();
			}
			else if (mapp[i][j] == '*') {
				path1[i*(2 * MAXCLO + 1) + j] = new pic(name3, pathWidth, pathHeight, 10 + i*blockh, j*blockh);
				path1[i*(2 * MAXCLO + 1) + j]->drawImageScale();
			}
			else if (mapp[i][j] == 'o') {
				path2[i*(2 * MAXCLO + 1) + j] = new pic(name4, pathWidth, pathHeight, 10 + i*blockh,  j*blockh);
				path2[i*(2 * MAXCLO + 1) + j]->drawImageScale();
			}
		}
	}



	/*	cout << "\t" << "迷宫的解:" << endl;
	for (int i = 0; i < 2 * MAXCLO + 1; ++i) {
	cout << "\t\t\t";
	for (int j = 0; j < 2 * MAXCLO + 1; ++j) {
	cout << mapp[i][j];     //记住你的所有解都在这里，把这个画出来，计算好坐标
	}
	cout << endl;
	}
	cout << "\n\n\n" << endl;*/
	endPaint();

	bool a = findnext(1, 1); //1,1起点寻路
	bool b = findnext2(2 * MAXCLO - 1, 1);

	while (mp->top != -1) {
		mapp[(mp->data[mp->top] / (2 * MAXCLO + 1))][(mp->data[mp->top]) % (2 * MAXCLO + 1)] = '*';
		--mp->top;
	}
	while (mp2->top != -1) {
		mapp[(mp2->data[mp2->top] / (2 * MAXCLO + 1))][(mp2->data[mp2->top]) % (2 * MAXCLO + 1)] = 'o';
		--mp2->top;
	}
	return 0;
}