#include "Graph.h"
#include <algorithm>
#include <unordered_set>
#include <functional>

void Graph::addNode(int id) {
    adj.try_emplace(id);
    indegree.try_emplace(id, 0);
    reverse.try_emplace(id);
}

void Graph::addEdge(int from, int to) {
    addNode(from);
    addNode(to);

    if (!hasEdge(from, to)) {
        adj[from].push_back(to);
        reverse[to].push_back(from);
        indegree[to]++;
    }
}

void Graph::removeEdge(int from, int to) {
    if (!hasNode(from) || !hasNode(to)) return;

    auto& edges = adj[from];
    edges.erase(std::remove(edges.begin(), edges.end(), to), edges.end());

    auto& rev = reverse[to];
    rev.erase(std::remove(rev.begin(), rev.end(), from), rev.end());

    if (indegree.count(to)) {
        indegree[to]--;
    }
}

void Graph::removeNode(int id) {
    if (!hasNode(id)) return;

    for (int to : adj[id]) {
        removeEdge(id, to);
    }

    for (int from : reverse[id]) {
        removeEdge(from, id);
    }

    adj.erase(id);
    indegree.erase(id);
    reverse.erase(id);
}

bool Graph::hasEdge(int from, int to) const {
    if (!hasNode(from)) return false;
    const auto& edges = adj.at(from);
    return std::find(edges.begin(), edges.end(), to) != edges.end();
}

std::vector<int> Graph::topologicalSort() const {
    if (hasCycle()) {
        throw std::runtime_error("Граф содержит цикл! Топологическая сортировка невозможна.");
    }

    std::queue<int> q;
    std::unordered_map<int, int> deg = indegree;

    for (const auto& [v, d] : deg) {
        if (d == 0) {
            q.push(v);
        }
    }

    std::vector<int> result;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);

        for (int next : adj.at(u)) {
            deg[next]--;
            if (deg[next] == 0) {
                q.push(next);
            }
        }
    }

    if (result.size() != adj.size()) {
        throw std::runtime_error("Топологическая сортировка не включила все вершины.");
    }

    return result;
}

bool Graph::hasCycle() const {
    std::unordered_set<int> visited;
    std::unordered_set<int> recStack;

    std::function<bool(int)> dfs = [&](int v) -> bool {
        visited.insert(v);
        recStack.insert(v);

        for (int next : adj.at(v)) {
            if (recStack.count(next)) {
                return true;
            }
            if (!visited.count(next) && dfs(next)) {
                return true;
            }
        }

        recStack.erase(v);
        return false;
        };

    for (const auto& [v, _] : adj) {
        if (!visited.count(v)) {
            if (dfs(v)) {
                return true;
            }
        }
    }

    return false;
}

std::vector<int> Graph::findCycle() const {
    std::unordered_set<int> visited;
    std::unordered_set<int> recStack;
    std::vector<int> path;
    std::vector<int> cycle;

    std::function<bool(int)> dfs = [&](int v) -> bool {
        visited.insert(v);
        recStack.insert(v);
        path.push_back(v);

        for (int next : adj.at(v)) {
            if (recStack.count(next)) {
                auto it = std::find(path.begin(), path.end(), next);
                cycle.assign(it, path.end());
                cycle.push_back(next);
                return true;
            }
            if (!visited.count(next) && dfs(next)) {
                return true;
            }
        }

        recStack.erase(v);
        path.pop_back();
        return false;
        };

    for (const auto& [v, _] : adj) {
        if (!visited.count(v)) {
            if (dfs(v)) {
                return cycle;
            }
        }
    }

    return {};
}

std::set<int> Graph::getIsolatedNodes() const {
    std::set<int> isolated;

    for (const auto& [v, edges] : adj) {
        if (edges.empty() && indegree.at(v) == 0) {
            isolated.insert(v);
        }
    }

    return isolated;
}

void Graph::clear() {
    adj.clear();
    indegree.clear();
    reverse.clear();
}

size_t Graph::edgeCount() const {
    size_t count = 0;
    for (const auto& [_, edges] : adj) {
        count += edges.size();
    }
    return count;
}