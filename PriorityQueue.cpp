/**********************
*优先队列
***********************/
#include <iostream>
#include <iomanip>
#include <assert.h>

#define MAXSIZE 5

using namespace std;

typedef int DataType;

typedef struct _QNode {
	int priority;
	DataType data;
	struct _QNode* next;
}QNode;

typedef QNode* QueuePtr;

typedef struct Queue {
	int length;
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

void InitQueue(LinkQueue* LQ) {
	if (!LQ) return;

	LQ->length = 0;
	LQ->front = LQ->rear = NULL;
}

int IsEmpty(LinkQueue* LQ) {
	if (!LQ) return 0;

	if (LQ->front == NULL) return 1;
	else return 0;
}

int IsFull(LinkQueue* LQ) {
	if (!LQ) return 0;

	if (LQ->length == MAXSIZE) return 1;
	else return 0;
}

int EnterQueue(LinkQueue* LQ, DataType data, int priority) {
	if (!LQ || IsFull(LQ)) {
		cout << "队列已满，数据元素-" << data << "-插入失败！" << endl;
	}

	QNode* node = new QNode;
	node->data = data;
	node->priority = priority;
	node->next = NULL;
	if (IsEmpty(LQ)) {
		LQ->front = LQ->rear = node;
	}
	else {
		LQ->rear->next = node;
		LQ->rear = node;
	}
	LQ->length++;
	return 1;
}

int DeleteQueue(LinkQueue* LQ, DataType* data) {
	if (!LQ || IsEmpty(LQ)) {
		cout << "队列已满!,无法出队!" << endl;
		return 0;
	}
	QNode** prev = NULL, * prev_node = NULL;	//prev即要删除节点的地址,prev即要删除节点的
	QNode* last = NULL, * tmp = NULL;

	if (!data) return 0;

	prev = &(LQ->front);
	printf("第一个节点的优先级: %d \n", (*prev)->priority);
	last = LQ->front;
	tmp = last->next;

	while (tmp) {
		if (tmp->priority > (*prev)->priority) {
			printf("抓到个更大优先级的节点[priority: %d]\n", tmp->priority);
			prev = &(last->next);
			prev_node = last;
		}
		last = tmp;
		tmp = tmp->next;
	}
	*data = (*prev)->data;
	tmp = *prev;
	*prev = (*prev)->next;
	delete tmp;
	LQ->length--;

	//接下来存在 2 种情况需要分别对待 
	//1.删除的是首节点,而且队列长度为零
	if (LQ->length == 0) {
		LQ->rear = NULL;
	}

	//2.删除的是尾部节点
	if (prev_node && prev_node->next == NULL) {
		LQ->rear = prev_node;
	}
	return 1;
}

void PrintQueue(LinkQueue* LQ) {
	QueuePtr tmp = NULL;
	if (!LQ) return;

	if (LQ->front == NULL) {
		cout << "队列为空!" << endl;
		return;
	}

	tmp = LQ->front;
	while (tmp) {
		cout << setw(4) << tmp->data << "[" << tmp->priority << "]";
		tmp = tmp->next;
	}
	cout << endl;
}

int GetHead(LinkQueue* LQ, DataType* data) {
	if (!LQ || IsEmpty(LQ)) {
		cout << "队列为空!" << endl; return 0;
	}

	if (!data) return 0;
	*data = LQ->front->data;
	return 1;
}

void ClearQueue(LinkQueue* LQ) {
	if (!LQ) return;
	while (LQ->front) {
		QueuePtr tmp = LQ->front->next;
		delete LQ->front;
		LQ->front = tmp;
	}
	LQ->rear = LQ->front = NULL;
	LQ->length = 0;
}

int getLength(LinkQueue* LQ) {
	if (!LQ) return 0;
	return LQ->length;
}

int main(void) {
	LinkQueue* LQ = new LinkQueue;
	DataType data = -1;

	InitQueue(LQ);

	for (int i = 0; i < 5; i++) {
		EnterQueue(LQ, i + 10, i);
	}

	printf("队列中的元素(总共%d 个)：", getLength(LQ));
	PrintQueue(LQ);
	cout << endl;

	for (int i = 0; i < 5; i++) {
		if (DeleteQueue(LQ, &data)) {
			cout << "出队的元素是：" << data << endl;
		}
		else {
			cout << "出队失败！" << endl;
		}
	}

	printf("出队五个元素后，队列中剩下的元素[%d]：\n", getLength(LQ));
	PrintQueue(LQ);
	cout << endl; ClearQueue(LQ);
	cout << "清空队列!\n";
	PrintQueue(LQ);

	delete LQ;
	return 0;
}
