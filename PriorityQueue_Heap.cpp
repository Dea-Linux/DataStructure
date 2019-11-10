#include <iostream>

#define DEFAULT_CAPCITY 128
#define isLess(a,b) (a < b)

using namespace std;

typedef int DataType;
typedef struct _PriorityQueue {
	DataType* arr;
	int size;
	int capacity;
}PriorityQueue;

bool init(PriorityQueue& pq, DataType* orginal, int size);
bool push(PriorityQueue& pq, DataType value);
bool pop(PriorityQueue& pq, DataType& value);
bool isEmpty(PriorityQueue& pq);
bool isFull(PriorityQueue& pq);
void destory(PriorityQueue& pq);

static void build(PriorityQueue& pq);
static void adjustDown(PriorityQueue& pq, int index);
static void adjustUp(PriorityQueue& pq, int  index);


bool init(PriorityQueue& pq, DataType* orginal, int size) {
	int capacity = DEFAULT_CAPCITY > size ? DEFAULT_CAPCITY : size;

	pq.arr = new DataType[capacity];
	if (!pq.arr) return false;

	pq.capacity = capacity;
	
	if (size > 0) {
		memcpy(pq.arr, orginal, sizeof(DataType) * size);
		pq.size = size;
		build(pq);
	}
	else {
		pq.size = 0;
	}
	return true;
}

bool isEmpty(PriorityQueue& pq) {
	if (pq.size < 1) return true;
	return false;
}

bool isFull(PriorityQueue& pq) {
	if (pq.size < pq.capacity) return false;
	return true;
}

void destory(PriorityQueue& pq) {
	if (pq.arr) delete[] pq.arr;
}

int size(PriorityQueue& pq) {
	return pq.size;
}

/* 从最后一个父节点(size/2-1 的位置)逐个往前调整所有父节点（直到根节 点）,
确保每一个父节点都是一个最大堆，最后整体上形成一个最大89堆*/
void build(PriorityQueue& pq) {
	for (int i = (pq.size / 2) - 1; i >= 0; i--) {
		adjustDown(pq, i);
	}
}
/*将当前的节点和子节点调整成最大堆*/
void adjustDown(PriorityQueue& pq, int index) {
	DataType cur = pq.arr[index];
	int parent  = 0, child = 0;

	/*判断否存在大于当前节点子节点，如果不存在 ，则堆本身是平衡的，不 需要调整；
	如果存在，则将最大的子节点与之交换，交换后，如果这个子节点还有子节 点，则要继续 
	按照同样的步骤对这个子节点进行调整 */
	for (parent = index; (parent * 2 + 1) < pq.size; parent = child) {
		child = parent * 2 + 1;

		//假如两个节点都存在的话,取两个子节点中最大的节点
		if ((child + 1) < pq.size && isLess(pq.arr[child] ,pq.arr[child+1])) child++;

		//判断最大的节点是否大于父节点
		if (isLess(pq.arr[child],cur)) {
			break;
		}
		else {
			pq.arr[parent] = pq.arr[child];
			pq.arr[child] = cur;
		}
	}
}

/*将当前节点和父节点调成最大堆*/
void adjustUp(PriorityQueue& pq, int index) {
	if (index < 0 || index >= pq.size) return;

	while (index > 0) {
		DataType temp = pq.arr[index];
		int parent = (index - 1) / 2;

		if (parent >= 0) {
			if (isLess(pq.arr[parent],temp)) {
				pq.arr[index] = pq.arr[parent];
				pq.arr[parent] = temp;
				index = parent;
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}
}

/* 删除优先队列中最大的节点，并获得节点的值*/ 
bool pop(PriorityQueue& pq, DataType& value) {
	if (isEmpty(pq)) return false;
	value = pq.arr[0];
	pq.arr[0] = pq.arr[--pq.size]; //heap.arr[0] = heap.arr[heap.size-1]; //heap.size--;
	adjustDown(pq, 0);// 向下执行堆调整
	return true;
}

/*优先队列中插入节点*/ 
bool push(PriorityQueue& pq, DataType value) {
	if (isFull(pq)) { 
		fprintf(stderr, "优先队列空间耗尽！\n");
		return false;
	}
	int index = pq.size;
	pq.arr[pq.size++] = value;
	adjustUp(pq, index);
	return true;
}


int main(void) {
	PriorityQueue pq;
	int task[] = { 1, 2, 3, 87, 93, 82, 92, 86, 95 }; 
	int i = 0;
	if (!init(pq, task, sizeof(task) / sizeof(task[0]))) { 
		fprintf(stderr, "初始化优先队列失败！\n"); 
		exit(-1); 
	}

	for (i = 0; i < pq.size; i++) {
		printf("the %dth task:%d\n", i, pq.arr[i]);
	}

	//堆中插入优先级为 88 的任务
	push(pq, 88);

	//堆中元素出列
	printf("按照优先级出列：\n"); 
	DataType value;
	while( pop(pq, value)){
		printf(" %d\n", value);
	}
	destory(pq);
	return 0;
}
