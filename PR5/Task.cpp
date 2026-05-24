#include "task.h"
#include <algorithm>
#include <sstream>
#include <iomanip>


using namespace std;


task::task() : vertexCount(0), edgeCount(0) {}


task::task(const string& filename) : vertexCount(0), edgeCount(0) {
    loadFromFile(filename);
}


task::task(const task& other) {
    incidenceMatrix = other.incidenceMatrix;
    adjacencyMatrix = other.adjacencyMatrix;
    edges = other.edges;
    vertexCount = other.vertexCount;
    edgeCount = other.edgeCount;
}


task::~task() {}


task& task::operator=(const task& other) {
    if (this != &other) {
        incidenceMatrix = other.incidenceMatrix;
        adjacencyMatrix = other.adjacencyMatrix;
        edges = other.edges;
        vertexCount = other.vertexCount;
        edgeCount = other.edgeCount;
    }
    return *this;
}


void task::convertIncidenceToAdjacency() {
    if (vertexCount == 0 || edgeCount == 0) return;


    adjacencyMatrix.assign(vertexCount, vector<int>(vertexCount, 0));


    for (int j = 0; j < edgeCount; j++) {
        int vertex1 = -1, vertex2 = -1;
        int weight = 0;


        for (int i = 0; i < vertexCount; i++) {
            if (incidenceMatrix[i][j] != 0) {
                if (vertex1 == -1) {
                    vertex1 = i;
                    weight = abs(incidenceMatrix[i][j]);
                }
                else {
                    vertex2 = i;

                }
            }
        }

        if (vertex1 != -1 && vertex2 != -1) {
            adjacencyMatrix[vertex1][vertex2] = weight;
            adjacencyMatrix[vertex2][vertex1] = weight;
        }
    }
}


void task::convertIncidenceToEdges() {
    if (vertexCount == 0 || edgeCount == 0) return;

    edges.clear();


    for (int j = 0; j < edgeCount; j++) {
        int vertex1 = -1, vertex2 = -1;
        int weight = 0;


        for (int i = 0; i < vertexCount; i++) {
            if (incidenceMatrix[i][j] != 0) {
                if (vertex1 == -1) {
                    vertex1 = i;
                    weight = abs(incidenceMatrix[i][j]);
                }
                else {
                    vertex2 = i;
                }
            }
        }

        if (vertex1 != -1 && vertex2 != -1) {
            edges.push_back({vertex1, vertex2 });
        }
    }
}


bool task::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return false;
    }


    file >> vertexCount >> edgeCount;

    if (vertexCount <= 0 || edgeCount <= 0) {
        cerr << "Ошибка: некорректные размеры графа" << endl;
        return false;
    }


    incidenceMatrix.assign(vertexCount, vector<int>(edgeCount, 0));


    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < edgeCount; j++) {
            file >> incidenceMatrix[i][j];
        }
    }

    file.close();


    convertIncidenceToAdjacency();
    convertIncidenceToEdges();

    return true;
}


void task::printIncidenceMatrix() const {
    cout << "Матрица инцидентности:" << endl;
    cout << "Вершины\\Ребра";
    for (int j = 0; j < edgeCount; j++) {
        cout << setw(4) << "E" << j + 1;
    }
    cout << endl;

    for (int i = 0; i < vertexCount; i++) {
        cout << "   V" << i + 1 << "     ";
        for (int j = 0; j < edgeCount; j++) {
            cout << setw(3) << incidenceMatrix[i][j];
        }
        cout << endl;
    }
}


void task::printAdjacencyMatrix() const {
    cout << "Матрица смежности:" << endl;
    cout << "     ";
    for (int i = 0; i < vertexCount; i++) {
        cout << setw(4) << "V" << i + 1;
    }
    cout << endl;

    for (int i = 0; i < vertexCount; i++) {
        cout << "V" << i + 1 << "   ";
        for (int j = 0; j < vertexCount; j++) {
            cout << setw(4) << adjacencyMatrix[i][j];
        }
        cout << endl;
    }
}


void task::dfs(int startVertex, vector<int>& traversalOrder) const {
    if (startVertex < 0 || startVertex >= vertexCount) {
        cerr << "Ошибка: неверная начальная вершина" << endl;
        return;
    }

    vector<bool> visited(vertexCount, false);
    stack<int> st;

    st.push(startVertex);

    while (!st.empty()) {
        int currentVertex = st.top();
        st.pop();

        if (!visited[currentVertex]) {
            visited[currentVertex] = true;
            traversalOrder.push_back(currentVertex);

            for (int i = vertexCount - 1; i >= 0; i--) {
                if (adjacencyMatrix[currentVertex][i] > 0 && !visited[i]) {
                    st.push(i);
                }
            }
        }
    }
}


int task::getWeight(int vertex1, int vertex2) const {
    if (vertex1 >= 0 && vertex1 < vertexCount &&
        vertex2 >= 0 && vertex2 < vertexCount) {
        return adjacencyMatrix[vertex1][vertex2];
    }
    return 0;
}


bool task::dijkstra(int startVertex, const string& outputFilename) {
    if (startVertex < 0 || startVertex >= vertexCount) {
        cerr << "Ошибка: неверная начальная вершина" << endl;
        return false;
    }

    const int INF = numeric_limits<int>::max();
    vector<int> distance(vertexCount, INF);
    vector<int> previous(vertexCount, -1);
    vector<bool> visited(vertexCount, false);

    distance[startVertex] = 0;

    for (int i = 0; i < vertexCount; i++) {
        int minDistance = INF;
        int currentVertex = -1;

        for (int j = 0; j < vertexCount; j++) {
            if (!visited[j] && distance[j] < minDistance) {
                minDistance = distance[j];
                currentVertex = j;
            }
        }

        if (currentVertex == -1) break;

        visited[currentVertex] = true;

        for (int j = 0; j < vertexCount; j++) {
            int weight = adjacencyMatrix[currentVertex][j];
            if (weight > 0 && !visited[j]) {
                int newDistance = distance[currentVertex] + weight;
                if (newDistance < distance[j]) {
                    distance[j] = newDistance;
                    previous[j] = currentVertex;
                }
            }
        }
    }


    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Ошибка: не удалось создать файл " << outputFilename << endl;
        return false;
    }

    outputFile << "Кратчайшие пути из вершины " << startVertex + 1 << ":" << endl;
    outputFile << "=========================================" << endl << endl;

    for (int i = 0; i < vertexCount; i++) {
        if (i != startVertex) {
            outputFile << "Вершина " << i + 1 << ": ";
            if (distance[i] == INF) {
                outputFile << "недостижима" << endl;
            }
            else {
                outputFile << "расстояние = " << distance[i] << ", путь: ";

                vector<int> path;
                for (int v = i; v != -1; v = previous[v]) {
                    path.push_back(v);
                }

                for (int j = path.size() - 1; j >= 0; j--) {
                    outputFile << path[j] + 1;
                    if (j > 0) outputFile << " -> ";
                }
                outputFile << endl;
            }
        }
    }

    outputFile << endl << "Расстояния от вершины " << startVertex + 1 << " до всех вершин:" << endl;
    for (int i = 0; i < vertexCount; i++) {
        if (distance[i] == INF) {
            outputFile << "Вершина " << i + 1 << ": недостижима" << endl;
        }
        else {
            outputFile << "Вершина " << i + 1 << ": " << distance[i] << endl;
        }
    }

    outputFile.close();
    return true;
}


ostream& operator<<(ostream& os, task& graph) {
    vector<int> traversalOrder;
    graph.dfs(0, traversalOrder);

    os << "Обход графа в глубину (начиная с вершины 1):" << endl;
    os << "Порядок обхода: ";
    for (size_t i = 0; i < traversalOrder.size(); i++) {
        os << traversalOrder[i] + 1;
        if (i < traversalOrder.size() - 1) os << " -> ";
    }
    os << endl;

    return os;
}