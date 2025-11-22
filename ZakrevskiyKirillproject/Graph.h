#pragma once
#include <unordered_map>
#include <vector>
#include <queue>
#include <stdexcept>
#include <set>

class Graph {
private:
    std::unordered_map<int, std::vector<int>> adj;
    std::unordered_map<int, int> indegree;
    std::unordered_map<int, std::vector<int>> reverse;

public:
    void addNode(int id);
    void addEdge(int from, int to);
    void removeEdge(int from, int to);
    void removeNode(int id);

    bool hasNode(int id) const { return adj.count(id) > 0; }
    bool hasEdge(int from, int to) const;

    const std::unordered_map<int, std::vector<int>>& getAdj() const { return adj; }
    const std::unordered_map<int, int>& getIndegree() const { return indegree; }

    std::vector<int> topologicalSort() const;
    bool hasCycle() const;
    std::vector<int> findCycle() const;
    std::set<int> getIsolatedNodes() const;

    void clear();
    size_t nodeCount() const { return adj.size(); }
    size_t edgeCount() const;
};