#pragma once
#include <unordered_map>
#include "Pipe.h"
#include "CompressorStation.h"
#include "Graph.h"

class NetworkEngine {
private:
    std::unordered_map<int, Pipe>& pipes;
    std::unordered_map<int, CompressorStation>& stations;
    Graph graph;
    std::unordered_map<int, std::pair<int, int>> connections;

    std::vector<int> dijkstra(int start, int end);
    double fordFulkerson(int source, int sink);
public:
    NetworkEngine(std::unordered_map<int, Pipe>& pipes,
        std::unordered_map<int, CompressorStation>& stations)
        : pipes(pipes), stations(stations) {
    }

    bool isPipeFree(int pipeId) const;
    bool isStationConnected(int stationId) const;

    int findFreePipe(int diameter) const;

    int connectStations(int cs_in, int cs_out, int pipe_id);
    void disconnectPipe(int pipe_id);
    void disconnectStation(int station_id);

    const std::unordered_map<int, std::pair<int, int>>& getConnections() const {
        return connections;
    }

    std::pair<int, int> getConnection(int pipe_id) const;
    bool hasConnection(int pipe_id) const { return connections.count(pipe_id) > 0; }

    const Graph& getGraph() const { return graph; }
    void displayConnections() const;

    std::vector<int> getShortestPath(int start, int end);
    double getMaxFlow(int source, int sink);

    void clear();
};