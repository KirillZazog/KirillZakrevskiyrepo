#include "PipelineSystem.h"
#include "Validation.h"
#include "Logger.h"
#include <fstream>
#include <sstream>

PipelineSystem::PipelineSystem() : manager(pipes, stations), nextPipeId(1), nextStationId(1) {}

void PipelineSystem::addPipe() {
    std::cout << "\n=== ДОБАВЛЕНИЕ ТРУБЫ ===\n";

    std::string name = getValidString("Введите название трубы: ");
    float length = getValidFloat("Введите длину трубы (км, положительное число): ", 0.0f);
    int diameter = getValidInt("Введите диаметр трубы (мм, положительное число): ", 1);
    bool repair = getValidBool("Статус трубы (0-не в ремонте/1-в ремонте)");

    Pipe newPipe(name, length, diameter, repair);
    pipes[nextPipeId] = newPipe;

    Logger::log("Добавлена труба ID " + std::to_string(nextPipeId) + ": " + name);
    std::cout << "Труба успешно добавлена с ID: " << nextPipeId << "\n";
    nextPipeId++;
}

void PipelineSystem::addStation() {
    std::cout << "\n=== ДОБАВЛЕНИЕ КС ===\n";

    std::string name = getValidString("Введите название КС: ");
    int total = getValidInt("Введите количество цехов (целое >= 0): ", 0);
    int active = getValidInt("Введите количество рабочих цехов (0.." + std::to_string(total) + "): ", 0, total);
    std::string efficiency = getValidString("Введите класс станции: ");

    CompressorStation newCS(name, total, active, efficiency);
    stations[nextStationId] = newCS;

    Logger::log("Добавлена КС ID " + std::to_string(nextStationId) + ": " + name);
    std::cout << "КС успешно добавлена с ID: " << nextStationId << "\n";
    nextStationId++;
}

void PipelineSystem::displayAllPipes() const {
    if (pipes.empty()) {
        std::cout << "\nТруб нет в системе.\n";
        return;
    }
    std::cout << "\n=== СПИСОК ВСЕХ ТРУБ ===\n";
    for (const auto& [id, pipe] : pipes) {
        std::cout << "\nТруба ID: " << id << "\n"
            << pipe;
        std::cout << "\n---\n";
    }
}

void PipelineSystem::displayAllStations() const {
    if (stations.empty()) {
        std::cout << "\nКС нет в системе.\n";
        return;
    }
    std::cout << "\n=== СПИСОК ВСЕХ КС ===\n";
    for (const auto& [id, station] : stations) {
        std::cout << "КС ID: " << id << "\n"
            << station;
        std::cout << "\n---\n";
    }
}

void PipelineSystem::editPipe() {
    if (pipes.empty()) {
        std::cout << "\nОшибка: труб нет в системе.\n";
        return;
    }

    displayAllPipes();
    std::cout << "Введите ID трубы для редактирования: ";
    int id = getValidInt("", 1);

    auto it = pipes.find(id);
    if (it != pipes.end()) {
        it->second.edit();
        Logger::log("Отредактирована труба ID " + std::to_string(id));
    }
    else {
        std::cout << "Труба с ID " << id << " не найдена.\n";
    }
}

void PipelineSystem::editStation() {
    if (stations.empty()) {
        std::cout << "\nОшибка: КС нет в системе.\n";
        return;
    }

    displayAllStations();
    std::cout << "Введите ID КС для редактирования: ";
    int id = getValidInt("", 1);

    auto it = stations.find(id);
    if (it != stations.end()) {
        it->second.edit();
        Logger::log("Отредактирована КС ID " + std::to_string(id));
    }
    else {
        std::cout << "КС с ID " << id << " не найдена.\n";
    }
}

void PipelineSystem::deletePipe() {
    if (pipes.empty()) {
        std::cout << "\nОшибка: труб нет в системе.\n";
        return;
    }

    displayAllPipes();
    int id = getValidInt("Введите ID трубы для удаления: ", 1);

    if (!pipes.count(id)) {
        std::cout << "Ошибка: труба с таким ID не существует.\n";
        return;
    }

    if (manager.connections.count(id)) {
        std::cout << "Ошибка: труба участвует в соединении и не может быть удалена.\n";
        return;
    }

    pipes.erase(id);
    Logger::log("Удалена труба ID " + std::to_string(id));

    std::cout << "Труба успешно удалена.\n";
}

void PipelineSystem::deleteStation() {
    if (stations.empty()) {
        std::cout << "\nОшибка: КС нет в системе.\n";
        return;
    }

    displayAllStations();
    int id = getValidInt("Введите ID КС для удаления: ", 1);

    if (!stations.count(id)) {
        std::cout << "Ошибка: КС с таким ID не существует.\n";
        return;
    }

    for (const auto& [pipeId, conn] : manager.connections) {
        if (conn.first == id || conn.second == id) {
            std::cout << "Ошибка: КС участвует в соединении и не может быть удалена.\n";
            return;
        }
    }

    stations.erase(id);
    Logger::log("Удалена КС ID " + std::to_string(id));

    std::cout << "КС успешно удалена.\n";
}

std::vector<int> PipelineSystem::findPipesByFilter(std::function<bool(const Pipe&)> filter) const {
    return findObjectsByFilter<Pipe>(pipes, filter);
}

std::vector<int> PipelineSystem::findStationsByFilter(std::function<bool(const CompressorStation&)> filter) const {
    return findObjectsByFilter<CompressorStation>(stations, filter);
}

void PipelineSystem::displaySearchResults(const std::vector<int>& ids, bool isPipe) const {
    if (ids.empty()) {
        std::cout << "Ничего не найдено.\n";
        return;
    }

    std::cout << "\nНайдено: " << ids.size() << "\n";
    for (int id : ids) {
        if (isPipe) {
            std::cout << "\nТруба ID: " << id << "\n"
                << pipes.at(id);
        }
        else {
            std::cout << "КС ID: " << id << "\n"
                << stations.at(id);
        }
        std::cout << "\n---\n";
    }
}

void PipelineSystem::searchPipes() {
    if (pipes.empty()) {
        std::cout << "\nОшибка: труб нет в системе.\n";
        return;
    }

    std::cout << "\n=== ПОИСК ТРУБ ===\n";
    std::cout << "1. По названию\n";
    std::cout << "2. По признаку 'в ремонте'\n";

    int choice = getValidInt("Ваш выбор: ", 1, 2);
    std::vector<int> results;

    if (choice == 1) {
        std::string searchName = getValidString("Введите название (частичное совпадение): ");
        results = findPipesByFilter([&searchName](const Pipe& p) {
            return p.getName().find(searchName) != std::string::npos;
            });
        Logger::log("Поиск труб по названию: " + searchName);
    }
    else {
        bool inRepair = getValidBool("Искать трубы в ремонте?");
        results = findPipesByFilter([inRepair](const Pipe& p) {
            return p.isInRepair() == inRepair;
            });
        Logger::log("Поиск труб по статусу ремонта: " + std::string(inRepair ? "в ремонте" : "не в ремонте"));
    }

    displaySearchResults(results, true);
}

void PipelineSystem::searchStations() {
    if (stations.empty()) {
        std::cout << "\nОшибка: КС нет в системе.\n";
        return;
    }

    std::cout << "\n=== ПОИСК КС ===\n";
    std::cout << "1. По названию\n";
    std::cout << "2. По проценту незадействованных цехов\n";

    int choice = getValidInt("Ваш выбор: ", 1, 2);
    std::vector<int> results;

    if (choice == 1) {
        std::string searchName = getValidString("Введите название (частичное совпадение): ");
        results = findStationsByFilter([&searchName](const CompressorStation& cs) {
            return cs.getName().find(searchName) != std::string::npos;
            });
        Logger::log("Поиск КС по названию: " + searchName);
    }
    else {
        float minPercent = getValidFloat("Введите минимальный процент незадействованных цехов (0-100): ", 0.0f, 100.0f);
        results = findStationsByFilter([minPercent](const CompressorStation& cs) {
            return cs.getUnusedPercent() >= minPercent;
            });
        Logger::log("Поиск КС по проценту незадействованных цехов >= " + std::to_string(minPercent));
    }

    displaySearchResults(results, false);
}

void PipelineSystem::batchEditPipes() {
    if (pipes.empty()) {
        std::cout << "\nОшибка: труб нет в системе.\n";
        return;
    }

    searchPipes();

    std::cout << "\n=== ПАКЕТНОЕ РЕДАКТИРОВАНИЕ ТРУБ ===\n";
    std::cout << "Введите ID труб через пробел (или 'all' для всех): ";
    std::string input = getValidString("", true);

    if (input.empty()) {
        std::cout << "Отмена операции.\n";
        return;
    }

    bool newStatus = getValidBool("Установить статус 'в ремонте'?");

    int count = 0;
    if (input == "all") {
        for (auto& [id, pipe] : pipes) {
            pipe.setInRepair(newStatus);
            count++;
        }
        Logger::log("Пакетное редактирование: все трубы, статус = " + std::string(newStatus ? "в ремонте" : "не в ремонте"));
    }
    else {
        std::istringstream iss(input);
        int id;
        while (iss >> id) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                it->second.setInRepair(newStatus);
                count++;
            }
        }
        Logger::log("Пакетное редактирование труб, изменено: " + std::to_string(count));
    }

    std::cout << "Отредактировано труб: " << count << "\n";
}

void PipelineSystem::batchDeletePipes() {
    if (pipes.empty()) {
        std::cout << "\nОшибка: труб нет в системе.\n";
        return;
    }

    searchPipes();

    std::cout << "\n=== ПАКЕТНОЕ УДАЛЕНИЕ ТРУБ ===\n";
    std::cout << "Введите ID труб для удаления через пробел: ";
    std::string input = getValidString("", true);

    if (input.empty()) {
        std::cout << "Отмена операции.\n";
        return;
    }

    std::istringstream iss(input);
    int id, count = 0;
    while (iss >> id) {
        if (pipes.erase(id)) {
            count++;
        }
    }

    Logger::log("Пакетное удаление труб, удалено: " + std::to_string(count));
    std::cout << "Удалено труб: " << count << "\n";
}

void PipelineSystem::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла для записи.\n";
        return;
    }

    for (const auto& [id, pipe] : pipes) {
        pipe.save(file, id);
    }

    for (const auto& [id, station] : stations) {
        station.save(file, id);
    }

    file.close();
    Logger::log("Данные сохранены в файл: " + filename);
    std::cout << "Данные успешно сохранены в " << filename << "\n";
}

void PipelineSystem::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла для чтения.\n";
        return;
    }

    pipes.clear();
    stations.clear();
    nextPipeId = 1;
    nextStationId = 1;

    std::string line;
    while (std::getline(file, line)) {
        if (line == "<PIPE>") {
            int id;
            Pipe pipe = Pipe::load(file, id);
            pipes[id] = pipe;
            if (id >= nextPipeId) {
                nextPipeId = id + 1;
            }
        }
        else if (line == "<CS>") {
            int id;
            CompressorStation station = CompressorStation::load(file, id);
            stations[id] = station;
            if (id >= nextStationId) {
                nextStationId = id + 1;
            }
        }
    }

    file.close();
    Logger::log("Данные загружены из файла: " + filename);
    std::cout << "Данные успешно загружены из " << filename << "\n";
    std::cout << "Загружено: труб - " << pipes.size() << ", КС - " << stations.size() << "\n";
}

void PipelineSystem::connectStationsMenu() {
    if (stations.empty()) {
        std::cout << "Нет станций!\n";
        return;
    }

    std::cout << "Введите ID станции входа: ";
    int in = getValidInt("", 0, nextStationId);

    if (!stations.count(in)) {
        std::cout << "Станции с таким ID нет!\n";
        return;
    }

    std::cout << "Введите ID станции выхода: ";
    int out = getValidInt("", 0, nextStationId);

    if (!stations.count(out)) {
        std::cout << "Станции с таким ID нет!\n";
        return;
    }

    std::cout << "Введите диаметр трубы (500/700/1000/1400): ";
    int diameter = getValidInt("", 500, 2000);

    int pipe_id = manager.findFreePipe(diameter);

    if (pipe_id == -1) {
        std::cout << "Нет свободной трубы с таким диаметром. Создайте трубу.\n";
        addPipe();

        pipe_id = manager.findFreePipe(diameter);
        if (pipe_id == -1) {
            std::cout << "Созданная труба не подходит.\n";
            return;
        }
    }

    try {
        manager.connectStations(in, out, pipe_id);
        std::cout << "Станции успешно соединены трубой ID = " << pipe_id << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void PipelineSystem::topoSortMenu() {
    try {
        auto order = manager.getGraph().topologicalSort();
        std::cout << "Топологический порядок станций:\n";
        for (int id : order)
            std::cout << id << " ";
        std::cout << "\n";
    }
    catch (const std::exception& ex) {
        std::cout << "Ошибка: " << ex.what() << "\n";
    }
}