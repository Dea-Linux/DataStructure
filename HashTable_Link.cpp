#include <iostream>

#define DEFAULT_SIZE 16

using namespace std;

typedef struct _ListNode {
	struct _ListNode* next;
	int key;
	void* data;
}ListNode;

typedef ListNode* List;
typedef ListNode* Element;

typedef struct _HashTable {
	int TableSize;
	List* Thelists;
}HashTable;

/*根据 key 计算索引，定位 Hash 桶的位置*/
int Hash(int key, int TableSize) {
	return (key % TableSize);
}

/*初始化哈希表*/
HashTable* InitHash(int TableSize) {
	int i = 0;
	HashTable* hTable = nullptr;

	if (TableSize < 0) TableSize = DEFAULT_SIZE;

	hTable = (HashTable*)malloc(sizeof(HashTable));

	if (hTable == nullptr) {
		printf("HashTable malloc error.\n");
		return nullptr;
	}

	hTable->TableSize = TableSize;

	//为 Hash 桶分配内存空间，其为一个指针数组
	hTable->Thelists = (List*)malloc(sizeof(List) * TableSize);
	if (hTable->Thelists == nullptr) {
		printf("HashTable malloc error.\n");
		free(hTable);
		return nullptr;
	}

	//为 Hash 桶对应的指针数组初始化链表节点
	for (int i = 0; i < TableSize; i++) {
		hTable->Thelists[i] = (ListNode*)malloc(sizeof(ListNode));
		if (hTable->Thelists == nullptr) {
			printf("hTable molloc error.\n");
			free(hTable->Thelists[i]);
			free(hTable);
			return nullptr;
		}
		else {
			memset(hTable->Thelists[i], 0, sizeof(ListNode));
		}
	}
	return hTable;
}

/*从哈希表中根据键值查找元素*/
Element Find(HashTable* HashTable, int key) {
	int i = 0;
	List L = nullptr;
	Element e = nullptr;
	i = Hash(key,HashTable->TableSize);
	L = HashTable->Thelists[i];
	e = L->next;
	while (e != nullptr && e->key != key)  e = e->next;
	return e;
}

/*哈希表插入元素，元素为键值对*/
void Insert(HashTable* HashTable, int key, void *value) {
	Element e = nullptr, tmp = nullptr;
	List L = nullptr;
	e = Find(HashTable, key);

	if (e == nullptr) {
		tmp = (Element)malloc(sizeof(ListNode));
		if (tmp == nullptr) {
			printf("tmp malloc error.\n");
			return ;
		}
		L = HashTable->Thelists[Hash(key, HashTable->TableSize)];
		tmp->data = value;
		tmp->key = key;
		tmp->next = L->next;
		L->next = tmp;
	}
	else {
		printf("the key already exist\n");
	}
}

/*哈希表删除元素，元素为键值对*/
void Delete(HashTable* HashTable, int key) {
	Element e = nullptr, last = nullptr;
	List L = nullptr;
	int i = Hash(key, HashTable->TableSize);
	L = HashTable->Thelists[i];

	last = L;
	e = L->next;
	while (e != nullptr && e->key != key) {
		last = e;
		e = e->next;
	}
	if (e) {
		last->next = e->next;
		free(e);
	}
}

/*哈希表元素中提取数据*/
void* Retrieve(Element e)
{
	return e ? e->data : NULL;
}

void main(void) {
	char* elems[] = { "翠花" ,"小芳" ,"苍老师" };
	int i = 0;

	HashTable* HashTable;
	HashTable = InitHash(31);
	Insert(HashTable, 1, elems[0]);
	Insert(HashTable, 2, elems[1]);
	Insert(HashTable, 3, elems[2]);
	Delete(HashTable, 1);

	for (i = 0; i < 4; i++) {
		Element e = Find(HashTable, i);
		if (e) {
			printf("%s\n", (const char*)Retrieve(e));
		}
		else {
			printf("Not found [key:%d]\n", i);
		}
	}
}
