#include <iostream>

#define BUCKET_SIZE 1024
#define DEFAULT_SIZE 16
#define hash_func SDBMHash

using namespace std;

typedef struct _ListNode {
	struct _ListNode* next;
	const void* key;
	const void* data;
}ListNode;
typedef ListNode* List;
typedef ListNode* Element;

typedef struct _HashTable {
	int TableSize;
	List* Thelists;
}HashTable;

unsigned int SDBMHash(const void* key) {
	unsigned  int hash = 0;
	char* str = (char*)key;

	while (*str) {
		hash = (*str++) + (hash << 6) + (hash << 16) - hash;
	}
	return (hash & 0xFFFFFFF);
}

/*根据 key 计算索引，定位 Hash 桶的位置*/
int Hash(const void* key, int TableSize) {
	return hash_func(key) % TableSize;
}

/*初始化哈希表*/
HashTable* InitHash(int TableSize) {
	int i = 0;
	HashTable* hTable = nullptr;

	if (TableSize < 0) TableSize = DEFAULT_SIZE;

	hTable = (HashTable*)malloc(sizeof(HashTable));
	if (hTable == nullptr) {
		cerr << "HashTable malloc error!" << endl;
		return nullptr;
	}

	hTable->TableSize = TableSize;

	//为 Hash 桶分配内存空间，其为一个指针数组
	hTable->Thelists = (List*)malloc(sizeof(List) * TableSize);
	if (hTable->Thelists == nullptr) {
		cerr << "HashTable malloc error!" << endl;
		free(hTable);
		return nullptr;
	}

	//为 Hash 桶对应的指针数组初始化链表节点
	for (i = 0; i < TableSize; i++) {
		hTable->Thelists[i] = (List)malloc(sizeof(ListNode));
		if (hTable->Thelists[i] == nullptr) {
			cerr << "HashTable malloc error!" << endl;
			free(hTable->Thelists);
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
Element Find(HashTable* HashTable, const void* key) {
	int i = 0;
	List L = nullptr;
	Element e = nullptr;
	i = Hash(key, HashTable->TableSize);
	L = HashTable->Thelists[i];
	e = L->next;
	while (e != nullptr && e->key != key)  e = e->next;
	return e;
}

/*哈希表插入元素，元素为键值对*/
void Insert(HashTable* HashTable,const void* key, const void* value) {
	Element tmp = nullptr, e = nullptr;
	List L = nullptr;
	e = Find(HashTable, key);

	if (e == nullptr) {
		tmp = (Element)malloc(sizeof(ListNode));
		if (tmp == nullptr) {
			printf("malloc error!\n");
			return;
		}
		int code = Hash(key, HashTable->TableSize);
		L = HashTable->Thelists[code];
		tmp->data = value;
		tmp->key;
		tmp->next = L->next;
		L->next = tmp;
	}
	else {
		printf("the key already exit\n");
	}
}

/*哈希表删除元素，元素为键值对*/
void Deleete(HashTable* HashTable, void* key) {
	Element e = nullptr, last = nullptr;
	List L = nullptr;
	int i = Hash(key, HashTable->TableSize);
	L = HashTable->Thelists[i];

	last = L;
	e = L->next;
	while (e != nullptr && e->key != key) {
		last = e;
		e = e ->next;
	}
	if (e) {
		last->next = e->next;
		free(e);
	}
}

/*哈希表元素中提取数据*/
const void* Retrieve(Element e) {
	return e ? e->data : nullptr;
}

/*销毁哈希表*/
void Destory(HashTable* HashTable) {
	int i = 0;
	List L = nullptr;
	Element cur = nullptr,next = nullptr;
	for (i = 0; i < HashTable->TableSize; i++) {
		L = HashTable->Thelists[i];
		cur = L->next;
		while (cur != nullptr) {
			next = cur->next;
			free(cur);
			cur = next;
		}
		free(L);
	}
	free(HashTable->Thelists);
	free(HashTable);
}

void  main(void) {
	const char* elems[] = { "ADBB","BDDC","CDBC","BDBB" };
	const char* tester = "ABDBBBAC";

	char cur[5] = { '\0' };
	int i = 0;
	HashTable* HashTable = NULL;
	HashTable = InitHash(BUCKET_SIZE);
	Insert(HashTable, elems[0], elems[0]);
	Insert(HashTable, elems[1], elems[1]);
	Insert(HashTable, elems[2], elems[2]);
	Insert(HashTable, elems[3], elems[3]);
	//Delete(HashTable, elems[0]);

	strncpy_s(cur, tester + 1, 4);//ADBB'\0'
	Element e = Find(HashTable, cur);
	if (e) {
		printf("%s\n", (const char*)Retrieve(e));
	}
	else {
		printf("Not found [key:%s]\n", cur);
	}
}
