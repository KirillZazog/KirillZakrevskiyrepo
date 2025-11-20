#include "PipelineManager.h"
#include <stdexcept>

bool PipelineManager::isPipeFree(int pipeId) const {
    return connections.find(pipeId) == connections.end();
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

int PipelineManager::connectStations(int cs_in, int cs_out, int pipe_id)
{
    if (!pipes.count(pipe_id))
        throw std::runtime_error("Pipe ID does not exist");

    if (!stations.count(cs_in) || !stations.count(cs_out))
        throw std::runtime_error("Station ID does not exist");

    if (!isPipeFree(pipe_id))
        throw std::runtime_error("Pipe already used");

    connections[pipe_id] = { cs_in, cs_out };

    graph.addEdge(cs_in, cs_out);

    return pipe_id;
}