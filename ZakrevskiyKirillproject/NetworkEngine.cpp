#include "NetworkEngine.h"
#include <stdexcept>
#include <iostream>
#include <functional>

bool NetworkEngine::isPipeFree(int pipeId) const {
    return connections.find(pipeId) == connections.end();
}

bool NetworkEngine::isStationConnected(int stationId) const {
    for (const auto& [pipeId, conn] : connections) {
        if (conn.first == stationId || conn.second == stationId) {
            return true;
        }
    }
    return false;
}

int NetworkEngine::findFreePipe(int diameter) const {
    for (const auto& [id, pipe] : pipes) {
        if (pipe.getDiameter() == diameter &&
            !pipe.isInRepair() &&
            isPipeFree(id))
        {
            return id;
        }
    }
    return -1;
}

int NetworkEngine::connectStations(int cs_in, int cs_out, int pipe_id) {
    if (!pipes.count(pipe_id)) {
        throw std::runtime_error("Труба с ID " + std::to_string(pipe_id) + " не существует");
    }

    if (!stations.count(cs_in)) {
        throw std::runtime_error("КС с ID " + std::to_string(cs_in) + " не существует");
    }

    if (!stations.count(cs_out)) {
        throw std::runtime_error("КС с ID " + std::to_string(cs_out) + " не существует");
    }

    if (!isPipeFree(pipe_id)) {
        throw std::runtime_error("Труба ID " + std::to_string(pipe_id) + " уже используется");
    }

    if (cs_in == cs_out) {
        throw std::runtime_error("Невозможно соединить станцию саму с собой (петля запрещена)");
    }

    graph.addEdge(cs_in, cs_out);
    if (graph.hasCycle()) {
        graph.removeEdge(cs_in, cs_out);
        throw std::runtime_error("Соединение создаст цикл в графе! Операция отменена.");
    }

    connections[pipe_id] = { cs_in, cs_out };

    return pipe_id;
}

void NetworkEngine::disconnectPipe(int pipe_id) {
    auto it = connections.find(pipe_id);
    if (it != connections.end()) {
        auto [cs_from, cs_to] = it->second;
        graph.removeEdge(cs_from, cs_to);

        connections.erase(it);
    }
}

void NetworkEngine::disconnectStation(int station_id) {
    std::vector<int> pipesToDisconnect;

    for (const auto& [pipeId, conn] : connections) {
        if (conn.first == station_id || conn.second == station_id) {
            pipesToDisconnect.push_back(pipeId);
        }
    }

    for (int pipeId : pipesToDisconnect) {
        disconnectPipe(pipeId);
    }

    graph.removeNode(station_id);
}

std::pair<int, int> NetworkEngine::getConnection(int pipe_id) const {
    auto it = connections.find(pipe_id);
    if (it != connections.end()) {
        return it->second;
    }
    return { -1, -1 };
}

void NetworkEngine::displayConnections() const {
    if (connections.empty()) {
        std::cout << "\nНет соединений в системе.\n";
        return;
    }

    std::cout << "\nГРАФ СОЕДИНЕНИЙ ТРУБОПРОВОДА\n\n";

    std::cout << "Всего соединений: " << connections.size() << "\n";
    std::cout << "Вершин в графе: " << graph.nodeCount() << "\n";
    std::cout << "Рёбер в графе: " << graph.edgeCount() << "\n\n";

    std::cout << "Список соединений:\n";
    std::cout << std::string(60, '-') << "\n";

    for (const auto& [pipeId, conn] : connections) {
        auto [cs_from, cs_to] = conn;

        std::string fromName = stations.count(cs_from) ?
            stations.at(cs_from).getName() : "?";
        std::string toName = stations.count(cs_to) ?
            stations.at(cs_to).getName() : "?";
        std::string pipeName = pipes.count(pipeId) ?
            pipes.at(pipeId).getName() : "?";

        std::cout << "КС[" << cs_from << "] \"" << fromName << "\"  -->  ";
        std::cout << "Труба[" << pipeId << "] \"" << pipeName << "\"  -->  ";
        std::cout << "КС[" << cs_to << "] \"" << toName << "\"\n";
    }

    std::cout << std::string(60, '-') << "\n";

    auto isolated = graph.getIsolatedNodes();
    if (!isolated.empty()) {
        std::cout << "\nИзолированные станции (без соединений): ";
        for (int id : isolated) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }

    if (graph.hasCycle()) {
        std::cout << "\nВНИМАНИЕ: В графе обнаружен цикл!\n";
        auto cycle = graph.findCycle();
        std::cout << "Цикл: ";
        for (int id : cycle) {
            std::cout << id << " -> ";
        }
        std::cout << "\n";
    }
    else {
        std::cout << "\nГраф ацикличен (возможна топологическая сортировка)\n";
    }
}

std::vector<int> NetworkEngine::dijkstra(int start, int end) {
    std::unordered_map<int, std::vector<std::pair<int, double>>> adjW;

    for (auto& [pipeId, c] : connections) {
        int u = c.first;
        int v = c.second;

        const Pipe& p = pipes.at(pipeId);

        double w;
        if (p.isInRepair()) {
            w = std::numeric_limits<double>::infinity();
        }
        else {
            w = p.getLength();
        }
        adjW[u].push_back({ v, w });
    }

    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;

    for (auto& [id, _] : stations)
        dist[id] = 1e18;

    dist[start] = 0;

    using P = std::pair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({ 0, start });

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        if (u == end) break;

        for (auto& [v, w] : adjW[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                parent[v] = u;
                pq.push({ dist[v], v });
            }
        }
    }

    if (dist[end] == 1e18) return {};

    std::vector<int> path;
    for (int v = end; v != start; v = parent[v])
        path.push_back(v);
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<int> NetworkEngine::getShortestPath(int start, int end) {
    return dijkstra(start, end);
}

double NetworkEngine::fordFulkerson(int source, int sink) {
    std::unordered_map<int, std::unordered_map<int, double>> residual;

    for (auto& [pipeId, c] : connections) {
        int u = c.first;
        int v = c.second;

        const Pipe& p = pipes.at(pipeId);
        double diam = p.getDiameter();
        double len = p.getLength();

        double cap;
        if (p.isInRepair()) {
            cap = 0.0;
        }
        else {
            double diam = p.getDiameter();
            double len = p.getLength();
            cap = std::sqrt(std::pow(diam, 5) / (len * 10e12));
        }

        residual[u][v] += cap;
        residual[v][u] += 0.0;
    }

    std::function<double(int, double, std::unordered_map<int, bool>&)> dfs =
        [&](int u, double flow, std::unordered_map<int, bool>& vis) -> double {

        if (u == sink) return flow;
        vis[u] = true;

        for (auto& [v, cap] : residual[u]) {
            if (cap > 1e-12 && !vis[v]) {
                double pushed = dfs(v, std::min(flow, cap), vis);
                if (pushed > 0) {
                    residual[u][v] -= pushed;
                    residual[v][u] += pushed;
                    return pushed;
                }
            }
        }
        return 0.0;
        };

    double maxFlow = 0.0;

    while (true) {
        std::unordered_map<int, bool> vis;
        double pushed = dfs(source, 1e18, vis);
        if (pushed <= 0) break;
        maxFlow += pushed;
    }

    return maxFlow;
}

double NetworkEngine::getMaxFlow(int source, int sink) {
    return fordFulkerson(source, sink);
}

void NetworkEngine::clear() {
    connections.clear();
    graph.clear();
}