#ifndef PIPELINE_SYSTEM_H
#define PIPELINE_SYSTEM_H

#include "Pipe.h"
#include "CompressorStation.h"
#include <unordered_map>
#include <vector>
#include <functional>

class PipelineSystem {
private:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CompressorStation> stations;
    int nextPipeId;
    int nextStationId;

public:
    PipelineSystem();

    void addPipe();
    void addStation();

    void displayAllPipes() const;
    void displayAllStations() const;

    void editPipe();
    void editStation();

    void deletePipe();
    void deleteStation();

    void searchPipes();
    void searchStations();

    void batchEditPipes();
    void batchDeletePipes();

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

private:
    std::vector<int> findPipesByFilter(std::function<bool(const Pipe&)> filter) const;
    std::vector<int> findStationsByFilter(std::function<bool(const CompressorStation&)> filter) const;
    void displaySearchResults(const std::vector<int>& ids, bool isPipe) const;
};

#endif