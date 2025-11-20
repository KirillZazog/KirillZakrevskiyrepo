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
public:
    std::unordered_map<int, std::pair<int, int>> connections;
    PipelineManager(std::unordered_map<int, Pipe>& pipes,
        std::unordered_map<int, CompressorStation>& stations)
        : pipes(pipes), stations(stations) {
    }

    bool isPipeFree(int pipeId) const;
    int findFreePipe(int diameter) const;

    int connectStations(int cs_in, int cs_out, int pipe_id);

    const Graph& getGraph() const { return graph; }
};
