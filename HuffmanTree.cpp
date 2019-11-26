#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <iomanip>

#define MaxSize 1024

using namespace std;

typedef struct _Bnode {
	char value;
	int weight;
	struct _Bnode* parent;
	struct _Bnode* rchild;
	struct _Bnode* lchild;
}Bnode,Btree;
typedef Bnode* DataType;			//任务队列中元素的类型

typedef struct _Qnode {
	int priority;
	DataType data;
	struct _Qnode* next;
}Qnode;
typedef Qnode* QueuePtr;

typedef struct Queue {
	int length;
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

void InitQueue(LinkQueue* LQ) {
	if (!LQ) return;
	LQ->length = 0;
	LQ->front = LQ->rear = nullptr;
}

int IsEmpty(LinkQueue LQ) {
	if (LQ.front == nullptr) return 1;
	return 0;
}

int IsFull(LinkQueue LQ) {
	if (LQ.length == MaxSize) return 1;
	return 0;
}

int EnterQueue(LinkQueue* LQ, DataType data, int priority) {
	if (!LQ) return 0;

	if (IsFull(*LQ)) {
		cout << "无法插入元素 " << data << ", 队列已满!" << endl;
		return 0;
	}
	Qnode* qNode = new Qnode;
	qNode->data = data;
	qNode->priority = priority;
	qNode->next = nullptr;

	if (IsEmpty(*LQ)) LQ->front = LQ->rear = qNode;
	else {
		qNode->next = LQ->front;
		LQ->front = qNode;
	}
	LQ->length++;
	return 1;
}

int PopQueue(LinkQueue* LQ, DataType* data) {
	Qnode** prev = nullptr, * prev_node = nullptr;
	Qnode* last = nullptr, * tmp = nullptr;

	if (!LQ || IsEmpty(*LQ)) {
		cout << "队列为空!" << endl;
		return 0;
	}

	if (!data) return 0;
	prev = &(LQ->front);
	last = LQ->front;
	tmp = last->next;

	while (tmp) {
		if (tmp->priority < (*prev)->priority) {
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

void PrintQueue(LinkQueue LQ) {
	QueuePtr tmp = nullptr;

	if (IsEmpty(LQ)) {
		cout << "队列为空!" << endl;
		return;
	}

	tmp = LQ.front;
	while (tmp) {
		cout << setw(4) << tmp->data->value << "[" << tmp->priority << "]";
		tmp = tmp->next;
	}
	cout << endl;
}

int GetHead(LinkQueue LQ,DataType *data) {
	if (IsEmpty(LQ)) {
		cout << "队列为空!" << endl;
		return 0;
	}
	if (*data) return 0;
	*data = LQ.front->data;
	return 1;
}

void ClearQueue(LinkQueue* LQ) {
	if (!LQ) return;
	while (LQ->front) {
		Qnode *tmp = LQ->front->next;
		delete LQ->front;
		LQ->front = tmp;
	}
	LQ->front = LQ->rear = nullptr;
	LQ->length = 0;
}

int getLength(LinkQueue LQ) {
	return LQ.length;
}

void HuffmanTree(Btree*& huff, int n) {
	LinkQueue* LQ = new LinkQueue;

	InitQueue(LQ);

	/* 初始化存放哈夫曼树数组 HuffNode[] 中的结点 */
	for (int i = 0; i < n;i ++) {
		Bnode* node = new Bnode;

		cout << "请输入第" << i + 1 << "个字符和出现频率: " << endl;
		cin >> node->value >> node->weight;
		node->parent = nullptr;
		node->lchild = nullptr;
		node->rchild = nullptr;

		EnterQueue(LQ, node, node->weight);
	}
	PrintQueue(*LQ);

	do {
		Bnode *node1 = nullptr;
		Bnode *node2 = nullptr;

		if (!IsEmpty(*LQ)) {
			PopQueue(LQ, &node1);
			printf("第一个出队列的数：%c, 优先级: %d\n", node1->value,node1->weight);
		}
		else {
			break;
		}

		if (!IsEmpty(*LQ)) {
			Bnode* node3 = new Bnode;

			PopQueue(LQ, &node2);
			printf("第二个出队列的数：%c, 优先级: %d\n", node2->value, node2->weight);

			node3->lchild = node1;
			node1->parent = node3;
			node3->rchild = node2;
			node2->parent = node3;
			node3->value = ' ';

			node3->weight = node1->weight + node2->weight;
			printf("合并进队列的数：%c, 优先级: %d\n", node3->value,node3->weight);
			EnterQueue(LQ, node3, node3->weight);
		}
		else {
			huff = node1;
			break;
		}
	} while (1);
}

/*采用递归方式实现前序遍历*/
void PreOrderRec(Btree * root) {
	if (root == nullptr) {
		return;
	}
	printf("—— %c ——", root->value);
	PreOrderRec(root->lchild);
	PreOrderRec(root->rchild);
}

int main(void) {
	Btree* tree = nullptr;
	HuffmanTree(tree, 7);
	PreOrderRec(tree);
	return 0;
}
