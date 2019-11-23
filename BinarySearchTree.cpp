/******************
 *二叉搜索树的常规操作
 * ****************/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define MaxSize 128
#define MAX_NODE 1024
#define isLess(a,b) (a < b)
#define isEqual(a,b) (a == b)

using namespace std;

typedef int ElemType;
typedef struct _Bnode {
	ElemType data;
	struct _Bnode* lchild, * rchild;
}Bnode,Btree;

typedef struct _SqStack {
	Bnode* base;
	Bnode* top;
}SqStack;

bool InitStack(SqStack& S) {
	S.base = new Bnode[MaxSize];
	if (!S.base) return false;
	S.top = S.base;
	return true;
}

bool PushStack(SqStack& S, Bnode e) {
	if (S.top - S.base == MaxSize) {
		cout << "栈空间已满,无法入栈!" << endl;
		return false;
	}
	*(S.top++) = e;
	return true;
}

bool PopStack(SqStack& S, Bnode& e) {
	if (S.base == S.top) return false;
	e = *(--S.top);
	return true;
}

Bnode* GetTop(SqStack S) {
	if (S.base == S.top) return nullptr;
	return S.top - 1;
}

int GetSize(SqStack S) {
	return (S.top - S.base);
}

bool IsEmpty(SqStack S) {
	if (S.top == S.base) return true;
	return false;
}

void DestoryStack(SqStack& S) {
	if (!S.base) {
		free(S.base);
		S.base = nullptr;
		S.top = nullptr;
	}
}

bool InsertBtree(Btree** root, Bnode* node) {
	Bnode* tmp = nullptr;
	Bnode* parent = nullptr;
	if (!node) {
		return false;
	} else {		//清空新节点 的左右子树
		node->lchild = nullptr;
		node->rchild = nullptr;
	}

	if (*root) {		//存在根节点
OAOAOA		tmp = *root;
OAOAOA	} else {		//不存在根节点 
		*root = node;
OAOAOA		return true;
	}

	while (tmp != nullptr) {		//退出循环时,parent为将要插入位置的父节点
		parent = tmp;
		if (isLess(node->data, tmp->data)) {
			tmp = tmp->lchild;
		} else {
			tmp = tmp->rchild;
		}
	}

	if (isLess(node->data, parent->data)) {
		parent->lchild = node;
	} else {
		parent->rchild = node;
	}
	return true;
}

int findMax(Btree* root) {
	if (root->rchild == nullptr) {
		return root->data;
	}
	return findMax(root->rchild);
}

Btree* DeleteNode(Btree* root, int key, Bnode*& deletedNode) {
	if (root == nullptr) return nullptr;
	if (root->data > key) {
OAOAOA		root->lchild = DeleteNode(root->lchild, key, deletedNode);
		return root;
	}
OAOAOA	
OAOAOA	if (root->data < key) {
		root->rchild = DeleteNode(root->rchild, key, deletedNode);
		return root;
OAOAOA	}

OAOAOA	deletedNode = root;		//root即要删除的节点

	//删除节点不存在左右子节点,即为叶子结点,直接删除
	if (root->lchild == nullptr && root->rchild == nullptr) return nullptr;
	//删除节点存在右子节点,直接用右子节点取代删除节点
	if (root->lchild == nullptr && root->rchild != nullptr) return root->rchild;
	//删除节点存在左子节点,直接用左子节点取代删除节点
	if (root->lchild != nullptr && root->rchild == nullptr) return root->lchild;
	//删除节点存在左右子节点,直接用左子节点最大值取代删除节点
	int val = findMax(root->lchild);
	root->data = val;
	root->lchild = DeleteNode(root->lchild, val, deletedNode);
	return  root;
}

/*采用递归方式查找节点*/
Bnode* QueryByRec(Btree* root, ElemType e) {
	if (isEqual(root->data, e) || root == nullptr) {
		return root;
	}
	else if (isLess(e, root->data)) {
OAOAOA		return QueryByRec(root->lchild, e);
OAOAOA	}
	else {
		return QueryByRec(root->rchild, e);
OAOAOA	}
}

/*使用非递归的方式查找节点*/
Bnode* QueryByLoop(Bnode* root, int e) {
	while (root != nullptr && !isEqual(root->data, e)) {
		if (isLess(e, root->data)) {
			root = root->lchild;
		} else {
			root = root->rchild;
		}
	}
	return root;
}

/*采用递归方式实现前序遍历*/
void PreOrderRec(Btree* root) {
	if (root == nullptr) {
		return;
	}
	printf("——%d——", root->data);
	PreOrderRec(root->lchild);
	PreOrderRec(root->rchild);
}

/*借助栈实现前序遍历*/
void PreOrder(Btree* root) {
	Bnode cur;
	if (root == nullptr) return;
	SqStack stack;
	InitStack(stack);
	PushStack(stack, *root);
	while (!(IsEmpty(stack))) {
		PopStack(stack, cur);
		printf("——%d——", cur.data);
		if (cur.rchild != nullptr) PushStack(stack, *(cur.rchild));
		if (cur.lchild != nullptr) PushStack(stack, *(cur.lchild));
	}
	DestoryStack(stack);
}

int main(void) {
	int test[] = { 19 , 7 , 25 , 5 , 11 , 15 , 21 , 61 };
	Bnode* root = nullptr, * node = nullptr;
	node = new Bnode;
	node->data = test[0];
	InsertBtree(&root, node);
	for (int i = 1; i < sizeof(test) / sizeof(test[0]); i++) {
		node = new Bnode;
		node->data = test[i];
		if (InsertBtree(&root, node)) {
			printf("节点 %d 插入成功\n", node->data);
		}
		else {
			printf("节点 %d 插入失败\n", node->data);
		}
	}
	printf("前序遍历结果:  \n");
	PreOrderRec(root);
	printf("\n");

	//二叉搜索树删除
	printf("删除节点 15\n");
	Bnode* deletedNode = nullptr;
	root = DeleteNode(root, 15, deletedNode);
	printf("二叉搜索树删除节点 15 , %s\n", deletedNode ? "删除成功" : "删除不成功,节点不存在");
	if (deletedNode) delete deletedNode;
	printf("删除后,在次前序遍历的结果: \n");
	PreOrderRec(root);
	printf("\n");

	//二叉搜索树查找节点
	Bnode* node1 = QueryByLoop(root, 20);
	printf("搜索二叉搜索树,节点 20 %s \n", node1 ? "存在" : "不存在");
	Bnode* node2 = QueryByLoop(root, 21);
	printf("搜索二叉搜索树,节点 21 %s \n", node2 ? "存在" : "不存在");
	return 0;
}
