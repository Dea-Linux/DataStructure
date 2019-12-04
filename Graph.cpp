#include <iostream>
#include <queue>

#define MaxSize 1024

using namespace std;

typedef struct _EdgeNode {	//与节点邻接的边的定义
	int adjvex;	//邻接的顶点
	int weight;	//权重
	struct _EdgeNode* next;		//下一条边
}EdgeNode;

typedef struct _VertexNode {	//顶点节点
	char data;
	struct _EdgeNode* first;	//邻接的第一条边
}VertexNode,AdjList;

typedef struct _AdjListGraph {
	AdjList* adjlist;
	int vex;		//顶点数
	int edge;		//边数
}AdjListGraph;

bool visited[MaxSize];		//全局数组,用来标记节点是否被访问

/*图的初始化*/
void Init(AdjListGraph& G) {
	G.adjlist = new AdjList[MaxSize];
	if (G.adjlist == nullptr) {
		cerr << "malloc error!" << endl;
		return;
	}
	G.edge = 0;
	G.vex = 0;

	for (int i = 0; i < MaxSize; i++) visited[i] = false;
}

/*通过顶点对应的字符寻找顶点在图中的邻接点*/
int Location(AdjListGraph& G, char c) {
	for (int i = 0; i < G.vex; i++) {
		if (G.adjlist[i].data == c) return i;
	}
	return -1;
}

/*图的创建*/
void Create(AdjListGraph& G) {
	cout << "请输入该图的顶点数以及边数" << endl;
	cin >> G.vex >> G.edge;

	cout << "请依次输入所有的顶点" << endl;
	for (int i = 0; i < G.vex; i++) {
		cin >> G.adjlist[i].data;
		G.adjlist[i].first = nullptr;
	}

	char v1 = 0, v2 = 0;
	int i1 = 0, i2 = 0;
	int weight = 0;

	cout << "请输入相关联的顶点和它们之间的权重(先输入起始节点,再输入终节点)" << endl;
	for (int i = 0; i < G.edge; i++) {
		cin >> v1 >> v2 >> weight;
		i1 = Location(G, v1);
		i2 = Location(G, v2);

		if (i1 != -1 && i2 != -1) {		//寻找位置
			EdgeNode* temp = new EdgeNode;
			temp->adjvex = i2;
			temp->weight = weight;
			temp->next = G.adjlist[i1].first;
			G.adjlist[i1].first = temp;
		}
	}
}

/*对图上的节点进行深度遍历*/
void DFS(AdjListGraph& G, int v) {
	int next = -1;

	if (visited[v]) return;

	cout << G.adjlist[v].data << "  ";
	visited[v] = true;

	EdgeNode* temp = G.adjlist[v].first;

	while (temp) {
		next = temp->adjvex;
		temp = temp->next;
		if (visited[next] == false) DFS(G, next);
	}
}

/*对所有的顶点进行深度遍历*/
void DFS_Main(AdjListGraph& G) {
	for (int i = 0; i < G.vex; i++) {
		if (visited[i] == false) DFS(G, i);
	}
}

/*对图上的顶点进行广度遍历*/
void BFS(AdjListGraph& G, int v) {
	queue<int> q;
	int cur = 0, next = 0;

	q.push(v);

	while (!q.empty()) {
		cur = q.front();
		if (visited[cur] == false) {
			cout << G.adjlist[cur].data << "  ";
			visited[cur] = true;
		}
		q.pop();	//出队

		EdgeNode* temp = G.adjlist[cur].first;
		while (temp != nullptr) {
			next = temp->adjvex;
			temp = temp->next;
			q.push(next);	//将第一个邻接点入队
		}
	}
}

/*对所有顶点进行广度遍历*/
void BFS_Main(AdjListGraph& G) {
	for (int i = 0; i < G.vex; i++) {
		if (visited[i] == false) BFS(G, i);
	}
}

int min_weights = 0x7FFFFFFF;	//最大整数2的32次方 - 1
int steps = 0;
int path[MaxSize] = { 0 };		//保存走过的路径
int shortest_path[MaxSize] = { 0 };		//保存最短路径

/*对图上的顶点进行深度遍历同时计算最短路径*/
void DFS_Min(AdjListGraph& G, int start, int end, int weights) {
	int cur = -1;

	cur = start;

	if (start == end) {		//已找到终点,不用继续遍历
		for (int i = 0; i < steps; i++) {		//输出所有可能路径
			cout << G.adjlist[i].data << "  ";
		}
		printf("\t\t该路径对应的长度是: %d\n", weights);
		
		if (min_weights > weights) {		//更新最小路径
			min_weights = weights;
			memcpy(shortest_path, path, steps * sizeof(int));
		}
	}

	visited[start] = true;
	EdgeNode* temp = G.adjlist[start].first;

	while (temp) {
		int weight = temp->weight;
		cur = temp->adjvex;
		if (visited[cur] == false) {
			visited[cur] = true;
			path[steps++] = cur;
			DFS_Min(G, cur, end, weight + weights);
			visited[cur] = false;
			path[--steps] = 0;
		}
		temp = temp->next;
	}
}

int main(void) {
	AdjListGraph G;

	/*图的初始化*/
	Init(G);

	/*创建图*/
	Create(G);

	/*广度遍历图*/
	//DFS_Main(G);

	/*深度遍历图*/
	//BFS_Main(G);

	char start, end;
	cout << "请输入要查询的最短路径的起点和终点：" << endl;
	cin >> start >> end;

	//求两点间的最短路径
	DFS_Min(G, Location(G, start), Location(G, end), 0);
	cout << "最小路径长度为：" << min_weights << "\t\t路径";
	int i = 0;
	while (i < MaxSize && shortest_path[i]>0) {
		cout << G.adjlist[shortest_path[i]].data << " ";
		i++;
	}

	return 0;
}
