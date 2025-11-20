#ifndef PIPELINE_SYSTEM_H
#define PIPELINE_SYSTEM_H

#include "Pipe.h"
#include "CompressorStation.h"
#include "PipelineManager.h"
#include <unordered_map>
#include <vector>
#include <functional>

class PipelineSystem {
private:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CompressorStation> stations;
    int nextPipeId;
    int nextStationId;
    PipelineManager manager;

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

    void connectStationsMenu();
    void topoSortMenu();

private:
    template<typename Object>
    std::vector<int> findObjectsByFilter(const std::unordered_map<int, Object>& objects, std::function<bool(const Object&)> filter) const {
        std::vector<int> results;
        for (const auto& [id, obj] : objects) {
            if (filter(obj)) {
                results.push_back(id);
            }
        }
        return results;
    }

    std::vector<int> findPipesByFilter(std::function<bool(const Pipe&)> filter) const;
    std::vector<int> findStationsByFilter(std::function<bool(const CompressorStation&)> filter) const;

    void displaySearchResults(const std::vector<int>& ids, bool isPipe) const;
};

#endif