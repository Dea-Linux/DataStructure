#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXSIZE 100
#define MaxSize 128
#define ROW 6
#define COL 6
using namespace std;

typedef struct _Position {
	int _x;
	int _y;
}Position;

typedef struct _Maze {
	int map[ROW][COL];
}Maze;

typedef Position ElemType;

typedef struct _SqStack {
	ElemType* base;
	ElemType* top;
}SqStack;

bool InitStack(SqStack* S) {
	S->base = new ElemType[MaxSize];
	if (!S->base) return false;
	S->top = S->base;
	return true;
}

bool PushStack(SqStack* S, ElemType e) {
	if ((S->top - S->base) == MaxSize) {
		cout << "栈以满,无法插入!" << endl;
		return false;
	}
	*(S->top++) = e;
	return true;
}

bool PopStack(SqStack* S, ElemType* e) {
	if (S->base == S->top) {
		cout << "栈为空,无法出栈!" << endl;
		return false;
	}
	*e = *(--S->top);
	return true;
}

ElemType* GetTop(SqStack* S) {
	if (S->base != S->top) {
		return S->top - 1;
	}
	else {
		return nullptr;
	}
}

bool IsEmpty(SqStack* S) {
	if (S->base == S->top) return true;
	else return false;
}

void DestoryStack(SqStack* S) {
	if (!S->base) {
		free(S->base);
		S->base = nullptr;
		S->top = nullptr;
	}
}

void InitMaze(Maze* m, int map[ROW][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			m->map[i][j] = map[i][j];
		}
	}
}

void PrintMaze(Maze m) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%4d", m.map[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int IsValidEnter(Maze* m, Position cur) {
	assert(m);
	if ((cur._x == 0 || cur._x == ROW - 1) || (cur._y == 0 || cur._y == COL - 1) && (m->map[cur._x][cur._y] == 1)) {
		return 1;
	}
	else {
		return 0;
	}
}

int IsNextPass(Maze* m, Position cur, Position next) {
	assert(m);
	if (((next._x == cur._x) && ((next._y == cur._y + 1) || (next._y == cur._y - 1))) //在同一行上并且相邻
		|| ((next._y == cur._y) && ((next._x == cur._x + 1) || (next._x == cur._x - 1)))) {//或在同一列上并且相邻
		//判断下一个节点是否在迷宫里面
		if (((next._x >= 0 && next._x < ROW) || (next._y >= 0 && next._y < COL))
			&& (m->map[next._x][next._y] == 1)) {
			return 1;
		}
	}
	return 0;
}

int IsValidExit(Maze* m, Position cur, Position enter) {
	assert(m);
	if ((cur._x != enter._x || cur._y != enter._y) &&
		((cur._x == 0 || cur._x == ROW - 1) || (cur._y == 0 || cur._y == COL - 1))) {
		return 1;
	}
	else {
		return 0;
	}
}

int PassMaze(Maze* m, Position enter, SqStack* S) {
	assert(m && IsValidEnter(m, enter) == 1);

	Position cur = enter;
	Position next;

	PushStack(S, cur);
	m->map[cur._x][cur._y] = 2;

	while (!IsEmpty(S)) {
		cur = *GetTop(S);
		if (IsValidExit(m, cur, enter) == 1)  return 1;//判断当前位置是否出口

		//尝试向左一步：看当前节点的左一个节点能不能走通
		next = cur;
		next._y = next._y - 1;
		if (IsNextPass(m, cur, next) == 1) {
			PushStack(S, next);
			m->map[next._x][next._y] = m->map[cur._x][cur._y] + 1;
			continue;
		}

		//尝试向上一步:看当前节点的上一个节点能不能走通
		next = cur;
		next._x = cur._x - 1;
		if (IsNextPass(m, cur, next) == 1) //next 节点能够走通时，将其压入栈中
		{
			PushStack(S, next);
			m->map[next._x][next._y] = m->map[cur._x][cur._y] + 1; //将next 节点的值等于 cur 节点的值加 1
				//PrintMaze(m);
			continue;
		}

		//右：看当前节点的向右的一个节点能不能走通
		next = cur;
		next._y = cur._y + 1;
		if (IsNextPass(m, cur, next) == 1)
		{
			PushStack(S, next);
			m->map[next._x][next._y] = m->map[cur._x][cur._y] + 1;
			//PrintMaze(m);
			continue;
		}

		//下：看当前节点的下一个节点能不能走通
		next = cur;
		next._x = cur._x + 1;
		if (IsNextPass(m, cur, next) == 1)
		{
			PushStack(S, next);
			m->map[next._x][next._y] = m->map[cur._x][cur._y] + 1;
			//PrintMaze(m);
			continue;
		}
		//走到这里说明当前节点的四个方向都走不通，进行回溯，看前一个节点未被遍历的方向是否还能走通
		Position tmp;
		PopStack(S, &tmp);
	}
	return 0;
}

int main()
{
	int map[ROW][COL] = { //用二维数组描绘迷宫：1 代表通路，0 代表墙
	0,0,1,0,0,0,
	0,0,1,1,1,0,
	0,0,1,0,0,0,
	0,1,1,1,1,0,
	0,0,1,0,1,0,
	0,0,0,0,1,0
	};

	Maze m;
	Position enter; //迷宫入口
	enter._x = 0;
	enter._y = 2;
	InitMaze(&m, map);
	PrintMaze(m);

	SqStack s; //定义栈，保存已走过的坐标轨迹，便于回溯
	InitStack(&s); //栈的初始
	int ret = PassMaze(&m, enter, &s); //使用栈和回溯法解开迷宫
	if (ret) {
		printf("恭喜你！终于找到了出口~\n");
	}
	else {
		printf("不是我笨！实在没有出口~\n");
	}
	PrintMaze(m);
	return 0;
}

