#pragma once
#include <unordered_map>
#include "Pipe.h"
#include "CompressorStation.h"
#include "Graph.h"

class PipelineManager {
private:
    std::unordered_map<int, Pipe>& pipes;
    std::unordered_map<int, CompressorStation>& stations;
    Graph graph;
    std::unordered_map<int, std::pair<int, int>> connections;

public:
    PipelineManager(std::unordered_map<int, Pipe>& pipes,
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

    void clear();
};