#include "task.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");
    cout << "========================================" << endl;
    cout << "Программа для работы с взвешенным графом" << endl;
    cout << "Вариант 18: обход в глубину (стек)" << endl;
    cout << "========================================" << endl << endl;

    string filename = "data.txt";

    

    task graph;

    cout << "Загрузка графа из файла " << filename << "..." << endl;
    if (!graph.loadFromFile(filename)) {
        cerr << "Ошибка при загрузке графа. Программа завершает работу." << endl;
        return 1;
    }

    cout << "Граф успешно загружен!" << endl << endl;

    cout << "Информация о графе:" << endl;
    cout << "Количество вершин: " << graph.getVertexCount() << endl;
    cout << "Количество ребер: " << graph.getEdgeCount() << endl << endl;

    graph.printIncidenceMatrix();
    cout << endl;

    graph.printAdjacencyMatrix();
    cout << endl;


    cout << "Демонстрация перегруженного оператора вывода:" << endl;
    cout << graph << endl;


    cout << "Демонстрация операции присваивания:" << endl;
    task graph2;
    graph2 = graph;
    cout << "Копия графа создана. Количество вершин в копии: " << graph2.getVertexCount() << endl << endl;

    cout << "Запуск алгоритма Дейкстры..." << endl;
    int startVertex = 0; 

    string outputFilename = "shortest_paths.txt";
    if (graph.dijkstra(startVertex, outputFilename)) {
        cout << "Результаты алгоритма Дейкстры сохранены в файл " << outputFilename << endl;
    }
    else {
        cerr << "Ошибка при выполнении алгоритма Дейкстры" << endl;
    }


    cout << endl << "Кратчайшие расстояния из вершины 1:" << endl;
    for (int i = 0; i < graph.getVertexCount(); i++) {
        if (i != startVertex) {
            int weight = graph.getWeight(startVertex, i);
            if (weight > 0) {
                cout << "  До вершины " << i + 1 << ": вес = " << weight << endl;
            }
            else if (i < graph.getVertexCount()) {

                cout << "  До вершины " << i + 1 << ": путь может проходить через другие вершины" << endl;
            }
        }
    }


    cout << endl << "Обход в глубину с разных начальных вершин:" << endl;
    for (int start = 0; start < min(3, graph.getVertexCount()); start++) {
        vector<int> traversal;
        graph.dfs(start, traversal);
        cout << "  Начиная с вершины " << start + 1 << ": ";
        for (size_t i = 0; i < traversal.size(); i++) {
            cout << traversal[i] + 1;
            if (i < traversal.size() - 1) cout << " -> ";
        }
        cout << endl;
    }

    cout << endl << "Программа успешно завершена!" << endl;

    return 0;
}