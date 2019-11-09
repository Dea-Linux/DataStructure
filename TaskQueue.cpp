#include <iostream>
#include <assert.h>
#include <iomanip>

#define MAXSIZE 100		//队列的最大容量

using namespace std;

typedef struct _QNode {		//节点的结构
	int id;
	void (*handler)(void);
	struct _QNode* next;
}QNode;

typedef QNode* QueuePtr;

typedef struct Queue {
	int length;
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

	//分配线程执行的任务节点

QueuePtr thread_task_alloc() {
	QNode* task = NULL;
	task = (QNode*)calloc(1, sizeof(QNode));
	if (task == NULL) return NULL;
	return task;
}

	//初始化队列,将队列初始化为空
void InitQueue(LinkQueue* LQ) {
	if (!LQ) return;
	LQ->length = 0;
	LQ->rear = LQ->front = NULL;
}

int IsEmpty(LinkQueue* LQ) {
	if (!LQ) return 0;
	if (LQ->front == NULL) return 1;
	return 0;
}

int IsFull(LinkQueue* LQ) {
	if (!LQ) return 0;
	if (LQ->length == MAXSIZE) return 1;
	return 0;
}

int EnterQueue(LinkQueue* LQ, QNode* node) {
	if (!LQ || !node) return 0;
	if (IsFull(LQ)) {
		cout << "队列已满,无法插入任务:" << node->id << endl;
		return 0;
	}
	node->next = NULL;
	if (IsEmpty(LQ)) LQ->front = LQ->rear = node;
	else {
		LQ->rear->next = node;
		LQ->rear = node;
	}
	return 1;
}

QNode* PopQueue(LinkQueue* LQ) {
	if (!LQ || IsEmpty(LQ)) {
		cout << "队列为空,无法出队!" << endl;
		return NULL;
	}
	QNode* tmp = NULL;
	tmp = LQ->front;
	LQ->front = tmp->next;
	if (!LQ->front) LQ->rear = NULL;
	LQ->length--;
	return tmp;
}

void PrintQueue(LinkQueue* LQ) {
	if (!LQ) return; 
	if (LQ->front == NULL) { 
		cout << "队列为空！"; return;
	}
	QNode* tmp = NULL;
	tmp = LQ->front;
	while (tmp) {
		cout << setw(4) << tmp->id;
		tmp = tmp->next;
	}
	cout << endl;
}

int GetLength(LinkQueue* LQ) {
	if (!LQ) return 0;
	return LQ->length;
}

void task1() { 
	printf("我是任务 1 ...\n");
}

void task2() { 
	printf("我是任务 2 ...\n");
}

int main(void) {
	LinkQueue* LQ = new LinkQueue;
	QNode* task = NULL;

	InitQueue(LQ);

	//任务1入队
	task = thread_task_alloc();
	task->id = 1;
	task->handler = &task1;
	EnterQueue(LQ, task);

	//任务2入队
	task = thread_task_alloc();
	task->id = 2;
	task->handler = &task2;
	EnterQueue(LQ, task);

	//打印队列中的元素
	printf("队列中的元素(总共%d 个)：", GetLength(LQ));
	PrintQueue(LQ); 
	cout << endl;

	while (task = PopQueue(LQ)) {
		task->handler();
		delete task;
	}

	delete LQ;
	return 0;
}
