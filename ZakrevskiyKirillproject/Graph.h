#pragma once
#include <unordered_map>
#include <vector>
#include <queue>
#include <stdexcept>

class Graph {
private:
    std::unordered_map<int, std::vector<int>> adj;
    std::unordered_map<int, int> indegree;

public:
    void addNode(int id);
    void addEdge(int from, int to);

    const std::unordered_map<int, std::vector<int>>& getAdj() const { return adj; }
    const std::unordered_map<int, int>& getIndegree() const { return indegree; }

    std::vector<int> topologicalSort() const;
};
