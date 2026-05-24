#ifndef TASK
#define TASK

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <queue>
#include <limits>

class task {
private:
    std::vector<std::vector<int>> incidenceMatrix; 
    std::vector<std::vector<int>> adjacencyMatrix; 
    std::vector<std::pair<int, int>> edges; 
    int vertexCount; 
    int edgeCount;   

    
    void convertIncidenceToAdjacency();

    void convertIncidenceToEdges();

public:

    task();
    explicit task(const std::string& filename);
    task(const task& other);
    ~task();


    task& operator=(const task& other);
    friend std::ostream& operator<<(std::ostream& os, task& graph);


    bool loadFromFile(const std::string& filename);
    void printIncidenceMatrix() const;
    void printAdjacencyMatrix() const;


    void dfs(int startVertex, std::vector<int>& traversalOrder) const;


    bool dijkstra(int startVertex, const std::string& outputFilename);

    int getVertexCount() const { return vertexCount; }
    int getEdgeCount() const { return edgeCount; }
    int getWeight(int vertex1, int vertex2) const;
};

#endif 