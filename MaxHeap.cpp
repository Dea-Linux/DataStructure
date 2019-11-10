#include <iostream>

#define DEFAULT_CAPCITY 128

using namespace std;

typedef struct _Heap {
	int* arr;
	int size;
	int capacity;
}Heap;

bool initHeap(Heap& heap, int* orginal, int size);
bool insert(Heap& heap, int value);
static void buildHeap(Heap& heap);
static void adjustDown(Heap& heap, int index);
static void adjustUp(Heap& heap, int  index);


bool initHeap(Heap& heap, int* orginal, int size) {
	int capacity = DEFAULT_CAPCITY > size ? DEFAULT_CAPCITY : size;

	heap.arr = new int[capacity];
	if (!heap.arr) return false;

	heap.capacity = capacity;
	
	if (size > 0) {
		memcpy(heap.arr, orginal, sizeof(int) * size);
		heap.size = size;
		buildHeap(heap);
	}
	else {
		heap.size = 0;
	}
	return true;
}

/* 从最后一个父节点(size/2-1 的位置)逐个往前调整所有父节点（直到根节 点）,
确保每一个父节点都是一个最大堆，最后整体上形成一个最大89堆*/
void buildHeap(Heap& heap) {
	for (int i = (heap.size / 2) - 1; i >= 0; i--) {
		adjustDown(heap, i);
	}
}
/*将当前的节点和子节点调整成最大堆*/
void adjustDown(Heap& heap, int index) {
	int cur = heap.arr[index];
	int parent  = 0, child = 0;

	/*判断否存在大于当前节点子节点，如果不存在 ，则堆本身是平衡的，不 需要调整；
	如果存在，则将最大的子节点与之交换，交换后，如果这个子节点还有子节 点，则要继续 
	按照同样的步骤对这个子节点进行调整 */
	for (parent = index; (parent * 2 + 1) < heap.size; parent = child) {
		child = parent * 2 + 1;

		//取两个子节点中最大的节点
		if ((child + 1) < heap.size && (heap.arr[child + 1] > heap.arr[child])) child++;

		//判断最大的节点是否大于父节点
		if (cur >= heap.arr[child]) {
			break;
		}
		else {
			heap.arr[parent] = heap.arr[child];
			heap.arr[child] = cur;
		}
	}
}

/*将当前节点和父节点调成最大堆*/
void adjustUp(Heap& heap, int index) {
	if (index < 0 || index >= heap.size) return;

	while (index > 0) {
		int temp = heap.arr[index];
		int parent = (index - 1) / 2;

		if (parent >= 0) {
			if (temp > heap.arr[parent]) {
				heap.arr[index] = heap.arr[parent];
				heap.arr[parent] = temp;
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

bool insert(Heap& heap, int value) {
	if (heap.size == heap.capacity) {
		fprintf(stderr, "栈空间耗尽");
		return false;
	}

	int index = heap.size;
	heap.arr[heap.size++] = value;
	adjustUp(heap, index);
	return true;
}

int main(void) {
	Heap hp;
	int origVals[] = { 1, 2, 3, 87, 93, 82, 92, 86, 95 };

	if (!initHeap(hp, origVals, sizeof(origVals) / sizeof(origVals[0]))) { 
		fprintf(stderr, "初始化堆失败！\n"); 
		exit(-1);
	}

	for (int i = 0; i < hp.size; i++) {
		printf("the %dth element:%d\n", i, hp.arr[i]);
	}

	insert(hp, 99);
	printf("在堆中插入新的元素 99，插入结果:\n");
	for (int i = 0; i < hp.size; i++) {
		printf("the %dth element:%d\n", i, hp.arr[i]);
	}
	return 0;
}


