#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <locale.h>

using namespace std;
//поиск пути queue
void BFSD(const vector < vector < int>>& G, int v, vector<int>& DIST) {
	queue<int> Q;
	Q.push(v);
	DIST[v] = 0;

	while (!Q.empty()) {
		int current = Q.front();
		Q.pop();

		for (int i = 0; i < G[current].size(); ++i) {
			if (G[current][i] == 1 && DIST[i] == -1) {
				Q.push(i);
				DIST[i] = DIST[current] + 1;
			}
		}
	}
}

//поиск пути список смежности
void BFSDList(const vector < vector < int>>& G, int v, vector<int>& DIST) {
	queue<int> Q;
	Q.push(v);
	DIST[v] = 0;

	while (!Q.empty()) {
		int current = Q.front();
		Q.pop();

		for (const int& neighbor : G[current]) {
			if (DIST[neighbor] == -1) {
				Q.push(neighbor);
				DIST[neighbor] = DIST[current] + 1;
			}
		}
	}
}


//поиск пути матрицы смежности в глубину
void DFSDMatrix(const vector < vector < int>>& G, int v, vector<int>& DIST, int distance = 0) {
	DIST[v] = distance;
	int num_vertices = G.size() - 1;

	for (int neighbor = 1; neighbor <= num_vertices; ++neighbor) {
		if (G[v][neighbor] == 1 && DIST[neighbor] == -1) {
			DFSDMatrix(G, neighbor, DIST, distance + 1);
		}
	}
}

//поиск пути список смежности в глубину
void DFSDList(const vector < vector < int>>& G, int v, vector<int>& DIST, int distance = 0) {
	DIST[v] = distance;

	for (const int& neighbor : G[v]) {
		if (DIST[neighbor] == -1) {
			DFSDList(G, neighbor, DIST, distance + 1);
		}
	}
}

void printList(const vector<vector<int>>& list) {
	printf("Список смежности:\n");
	for (int i = 1; i < list.size(); ++i) {
		printf("%d: ", i);
		for (const int& j : list[i]) {
			printf("%d ", j);
		}
		printf("\n");
	}
}

vector<vector<int>> generateAndPrintMatrix(int num_vertices, double edge_probability) {
	vector<vector<int>> adjacency_matrix(num_vertices + 1, vector<int>(num_vertices + 1, 0));
	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = i + 1; j <= num_vertices; ++j) {
			if (static_cast<double>(rand()) / RAND_MAX < edge_probability) {
				adjacency_matrix[i][j] = 1;
				adjacency_matrix[j][i] = 1;
			}
		}
	}
	printf("Матрица смежности:\n");
	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = 1; j <= num_vertices; ++j) {
			printf("%d ", adjacency_matrix[i][j]);
		}
		printf("\n");
	}
	return adjacency_matrix;
}

vector<vector<int>> matrixToList(const vector<vector<int>>& matrix) {
	int num_vertices = matrix.size() - 1;
	vector<vector<int>> adjacency_list(num_vertices + 1);
	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = 1; j <= num_vertices; ++j) {
			if (matrix[i][j] == 1) {
				adjacency_list[i].push_back(j);
			}
		}
	}
	return adjacency_list;
}

int main() {
	setlocale(LC_ALL, "RUS");
	int num_vertices;
	double edge_probability;
	clock_t start, end;
	double cpu_time_used;


	printf("Введите количество вершин в графе: ");
	scanf("%d", &num_vertices);

	printf("Введите вероятность наличия ребра в графе (0 - 1): ");
	scanf("%lf", &edge_probability);

	srand(static_cast<unsigned int>(time(nullptr)));

	auto matrix = generateAndPrintMatrix(num_vertices, edge_probability);
	auto adjacency_list = matrixToList(matrix);
	printList(adjacency_list);

	vector<int> DIST(num_vertices + 1, -1); // Вектор расстояний

	int start_vertex = 1; // начальная вершина всегда 1
	int target_vertex; // вершина, до которой нужно найти путь



	printf("Введите конечную вершину: ");
	scanf("%d", &target_vertex);


	start = clock();
	BFSD(matrix, start_vertex, DIST); 
	end = clock();
	// Проверка наличия пути до целевой вершины
	if (DIST[target_vertex] == -1) {
		printf("Предупреждение: Путь до заданной вершины не существует.\n");
	}
	else {
		printf("Расстояние до заданной вершины(Матрица смежности): %d\n", DIST[target_vertex]);
	}
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Время выполнения: %f секунд\n", cpu_time_used);
	vector<int> PREV(num_vertices + 1, -1);

	//Сброс DIST и PREV
	std::fill(DIST.begin(), DIST.end(), -1);
	std::fill(PREV.begin(), PREV.end(), -1);

	start = clock();
	BFSDList(adjacency_list, start_vertex, DIST);
	end = clock();
	if (DIST[target_vertex] == -1) {
		printf("Предупреждение: Путь до заданной вершины не существует.\n");
	}
	else {
		printf("Расстояние до заданной вершины(Список смежности): %d\n", DIST[target_vertex]);
	}
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Время выполнения: %f секунд\n", cpu_time_used);

	//Сброс DIST и PREV
	std::fill(DIST.begin(), DIST.end(), -1);
	std::fill(PREV.begin(), PREV.end(), -1);
	
	start = clock();
	DFSDMatrix(matrix, start_vertex, DIST);
	end = clock();
	if (DIST[target_vertex] == -1) {
		printf("Предупреждение: Путь до заданной вершины не существует.\n");
	}
	else {
		printf("Расстояние до заданной вершины в глубину(Матрица смежности): %d\n", DIST[target_vertex]);
	}
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Время выполнения: %f секунд\n", cpu_time_used);

	//Сброс DIST и PREV
	std::fill(DIST.begin(), DIST.end(), -1);
	std::fill(PREV.begin(), PREV.end(), -1);
	
	start = clock();
	DFSDList(adjacency_list, start_vertex, DIST);
	end = clock();
	if (DIST[target_vertex] == -1) {
		printf("Предупреждение: Путь до заданной вершины не существует.\n");
	}
	else {
		printf("Расстояние до заданной вершины в глубину(Список смежности): %d\n", DIST[target_vertex]);
	}
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Время выполнения: %f секунд\n", cpu_time_used);
	return 0;
}


