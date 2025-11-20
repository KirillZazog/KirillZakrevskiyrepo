#include "Graph.h"

void Graph::addNode(int id) {
    adj.try_emplace(id);
    indegree.try_emplace(id, 0);
}

void Graph::addEdge(int from, int to) {
    addNode(from);
    addNode(to);
    adj[from].push_back(to);
    indegree[to]++;
}

std::vector<int> Graph::topologicalSort() const {
    std::queue<int> q;
    std::unordered_map<int, int> deg = indegree;

    for (const auto& [v, d] : deg)
        if (d == 0)
            q.push(v);

    std::vector<int> result;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        result.push_back(u);

        for (int next : adj.at(u)) {
            deg[next]--;
            if (deg[next] == 0)
                q.push(next);
        }
    }

    if (result.size() != adj.size())
        throw std::runtime_error("Graph contains a cycle! Topological sort impossible.");

    return result;
}
