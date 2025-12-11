#include "PipelineManager.h"
#include <stdexcept>
#include <iostream>

bool PipelineManager::isPipeFree(int pipeId) const {
    return connections.find(pipeId) == connections.end();
}

bool PipelineManager::isStationConnected(int stationId) const {
    for (const auto& [pipeId, conn] : connections) {
        if (conn.first == stationId || conn.second == stationId) {
            return true;
        }
    }
    return false;
}

int PipelineManager::findFreePipe(int diameter) const {
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

int PipelineManager::connectStations(int cs_in, int cs_out, int pipe_id) {
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

void PipelineManager::disconnectPipe(int pipe_id) {
    auto it = connections.find(pipe_id);
    if (it != connections.end()) {
        auto [cs_from, cs_to] = it->second;
        graph.removeEdge(cs_from, cs_to);

        connections.erase(it);
    }
}

void PipelineManager::disconnectStation(int station_id) {
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

std::pair<int, int> PipelineManager::getConnection(int pipe_id) const {
    auto it = connections.find(pipe_id);
    if (it != connections.end()) {
        return it->second;
    }
    return { -1, -1 };
}

void PipelineManager::displayConnections() const {
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

void PipelineManager::clear() {
    connections.clear();
    graph.clear();
}