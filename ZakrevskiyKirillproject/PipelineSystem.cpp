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
    std::cout << "Введите ID трубы для удаления: ";
    int id = getValidInt("", 1);

    if (!pipes.count(id)) {
        std::cout << "Труба с ID " << id << " не найдена.\n";
        return;
    }

    if (manager.hasConnection(id)) {
        std::cout << "\nТруба ID " << id << " участвует в соединении.\n";
        auto [cs_from, cs_to] = manager.getConnection(id);

        std::cout << "Соединение: КС[" << cs_from << "]";
        if (stations.count(cs_from)) {
            std::cout << " \"" << stations.at(cs_from).getName() << "\"";
        }
        std::cout << " -> КС[" << cs_to << "]";
        if (stations.count(cs_to)) {
            std::cout << " \"" << stations.at(cs_to).getName() << "\"";
        }
        std::cout << "\n\n";

        bool confirm = getValidBool("Отключить трубу из графа и удалить?");
        if (confirm) {
            manager.disconnectPipe(id);
            std::cout << "Соединение разорвано, труба отключена из графа.\n";
        }
        else {
            std::cout << "Удаление отменено.\n";
            return;
        }
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
    std::cout << "Введите ID КС для удаления: ";
    int id = getValidInt("", 1);

    if (!stations.count(id)) {
        std::cout << "КС с ID " << id << " не найдена.\n";
        return;
    }

    if (manager.isStationConnected(id)) {
        std::cout << "\nКС ID " << id << " участвует в соединениях.\n\n";

        std::cout << "Связанные соединения:\n";
        int count = 0;
        for (const auto& [pipeId, conn] : manager.getConnections()) {
            if (conn.first == id || conn.second == id) {
                std::cout << "  " << (++count) << ". Труба[" << pipeId << "]: ";
                std::cout << "КС[" << conn.first << "]";
                if (stations.count(conn.first)) {
                    std::cout << " \"" << stations.at(conn.first).getName() << "\"";
                }
                std::cout << " -> КС[" << conn.second << "]";
                if (stations.count(conn.second)) {
                    std::cout << " \"" << stations.at(conn.second).getName() << "\"";
                }
                std::cout << "\n";
            }
        }
        std::cout << "Всего соединений: " << count << "\n\n";

        bool confirm = getValidBool("Отключить все связи из графа и удалить КС?");
        if (confirm) {
            manager.disconnectStation(id);
            std::cout << "Все соединения с КС разорваны, станция отключена из графа.\n";
        }
        else {
            std::cout << "Удаление отменено.\n";
            return;
        }
    }

    stations.erase(id);
    Logger::log("Удалена КС ID " + std::to_string(id));
    std::cout << "✓ КС успешно удалена.\n";
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
        std::cout << "\nНет станций в системе!\n";
        std::cout << "Сначала добавьте компрессорные станции (опция 2).\n";
        return;
    }

    if (pipes.empty()) {
        std::cout << "\nНет труб в системе!\n";
        std::cout << "Сначала добавьте трубы (опция 1).\n";
        return;
    }

    std::cout << "\nСОЕДИНЕНИЕ СТАНЦИЙ ЧЕРЕЗ ТРУБУ\n\n";
    std::cout << "Доступные станции:\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto& [id, station] : stations) {
        std::cout << "  КС[" << id << "]: " << station.getName() << "\n";
    }
    std::cout << std::string(60, '-') << "\n\n";

    int in = getValidInt("Введите ID станции ВХОДА: ", 1, nextStationId);

    if (!stations.count(in)) {
        std::cout << "Станции с ID " << in << " не существует!\n";
        return;
    }

    int out = getValidInt("Введите ID станции ВЫХОДА: ", 1, nextStationId);

    if (!stations.count(out)) {
        std::cout << "Станции с ID " << out << " не существует!\n";
        return;
    }

    std::cout << "\nВыбрано соединение:\n";
    std::cout << "  " << stations.at(in).getName() << " [" << in << "]  -->  ";
    std::cout << stations.at(out).getName() << " [" << out << "]\n\n";

    std::cout << "Стандартные диаметры: 500, 700, 1000, 1400 мм\n";
    int diameter = getValidInt("Введите требуемый диаметр трубы (мм): ", 100, 2000);

    int pipe_id = manager.findFreePipe(diameter);

    if (pipe_id == -1) {
        std::cout << "\nНет свободной трубы с диаметром " << diameter << " мм.\n";
        std::cout << "Создать новую трубу? ";

        bool createNew = getValidBool("");

        if (createNew) {
            addPipe();
        }
        else {
            std::cout << "Операция отменена.\n";
            return;
        }
    }
    else {
        std::cout << "\nНайдена свободная труба:\n";
        std::cout << "ID: " << pipe_id << "\n";
        std::cout << "Название: " << pipes.at(pipe_id).getName() << "\n";
        std::cout << "Диаметр: " << pipes.at(pipe_id).getDiameter() << " мм\n";
        std::cout << "Длина: " << pipes.at(pipe_id).getLength() << " км\n\n";
    }

    try {
        manager.connectStations(in, out, pipe_id);

        std::cout << "\nСОЕДИНЕНИЕ УСПЕШНО СОЗДАНО\n\n";

        std::cout << "Детали соединения:\n";
        std::cout << "КС входа:  [" << in << "] " << stations.at(in).getName() << "\n";
        std::cout << "Труба:     [" << pipe_id << "] " << pipes.at(pipe_id).getName() << "\n";
        std::cout << "КС выхода: [" << out << "] " << stations.at(out).getName() << "\n";

        Logger::log("Соединены КС[" + std::to_string(in) + "] -> КС[" +
            std::to_string(out) + "] через трубу[" + std::to_string(pipe_id) + "]");
    }
    catch (const std::exception& e) {
        std::cout << "\nОШИБКА ПРИ СОЗДАНИИ СОЕДИНЕНИЯ\n\n";
        std::cout << e.what() << "\n\n";
        Logger::log("Ошибка соединения: " + std::string(e.what()));
    }
}

void PipelineSystem::displayGraphMenu() {
    const auto& connections = manager.getConnections();
    const Graph& g = manager.getGraph();

    std::cout << "\nСОЕДИНЕНИЯ ГРАФА ТРУБОПРОВОДНОЙ СЕТИ\n\n";

    if (connections.empty()) {
        std::cout << "Граф пуст. Соединений нет.\n\n";
        std::cout << "Используйте опцию 15 для создания соединений между станциями.\n";
        return;
    }

    int counter = 1;
    for (const auto& [pipeId, conn] : connections) {
        auto [cs_from, cs_to] = conn;

        std::string fromName = stations.count(cs_from) ?
            stations.at(cs_from).getName() : "Неизвестная";
        std::string toName = stations.count(cs_to) ?
            stations.at(cs_to).getName() : "Неизвестная";
        std::string pipeName = pipes.count(pipeId) ?
            pipes.at(pipeId).getName() : "Неизвестная";

        std::cout << counter++ << ". ";
        std::cout << "КС[" << cs_from << "] \"" << fromName << "\"\n";
        std::cout << "     ↓ Труба[" << pipeId << "] \"" << pipeName << "\"\n";
        std::cout << "   КС[" << cs_to << "] \"" << toName << "\"\n\n";
    }

    auto isolated = g.getIsolatedNodes();
    if (!isolated.empty()) {
        std::cout << "изолированные станции (без соединений):\n";
        std::cout << "   ";
        for (int id : isolated) {
            std::string name = stations.count(id) ?
                stations.at(id).getName() : "Неизвестная";
            std::cout << "КС[" << id << "] \"" << name << "\"  ";
        }
    }

    if (g.hasCycle()) {
        auto cycle = g.findCycle();
        std::cout << "Обнаруженный цикл:\n   ";
        for (size_t i = 0; i < cycle.size(); ++i) {
            std::cout << "КС[" << cycle[i] << "]";
            if (i < cycle.size() - 1) {
                std::cout << " → ";
            }
        }

    }
    else {
        std::cout << "Граф ацикличен.\nТопологическая сортировка возможна (опция 17)\n";
    }
}

void PipelineSystem::topoSortMenu() {
    const Graph& g = manager.getGraph();

    std::cout << "\nТОПОЛОГИЧЕСКАЯ СОРТИРОВКА ГРАФА СТАНЦИЙ\n\n";

    if (g.nodeCount() == 0) {
        std::cout << "Граф пуст.\n\n";
        std::cout << "Сначала создайте соединения между станциями (опция 15).\n";
        return;
    }

    std::cout << "   Информация о графе:\n";
    std::cout << "   Вершин (станций): " << g.nodeCount() << "\n";
    std::cout << "   Рёбер (труб):     " << g.edgeCount() << "\n\n";

    auto isolated = g.getIsolatedNodes();
    if (!isolated.empty()) {
        std::cout << "    Обнаружены изолированные станции: ";
        for (int id : isolated) {
            std::cout << "КС[" << id << "] ";
        }
        std::cout << "\n";
        std::cout << "   Они будут включены в сортировку, но порядок их\n";
        std::cout << "   относительно других станций произволен.\n\n";
    }

    if (g.hasCycle()) {
        std::cout << "ОШИБКА: ГРАФ СОДЕРЖИТ ЦИКЛ\n\n";

        std::cout << "Топологическая сортировка невозможна для графа с циклами.\n\n";

        auto cycle = g.findCycle();

        std::cout << "  Обнаруженный цикл:\n\n";
        std::cout << "   ";
        for (size_t i = 0; i < cycle.size(); ++i) {
            int id = cycle[i];
            std::string name = stations.count(id) ?
                stations.at(id).getName() : "Неизвестная";

            std::cout << "КС[" << id << "] \"" << name << "\"";

            if (i < cycle.size() - 1) {
                std::cout << "\n   ↓\n   ";
            }
        }
        Logger::log("Топосортировка: обнаружен цикл, операция невозможна");
        return;
    }

    try {
        auto order = g.topologicalSort();

        std::cout << " ТОПОЛОГИЧЕСКИЙ ПОРЯДОК СТАНЦИЙ:\n";
        std::cout << std::string(60, '-') << "\n";

        for (size_t i = 0; i < order.size(); ++i) {
            int id = order[i];
            std::string name = stations.count(id) ?
                stations.at(id).getName() : "Неизвестная КС";

            std::cout << "  " << (i + 1) << ". КС[" << id << "] \"" << name << "\"";

            if (stations.count(id)) {
                const auto& cs = stations.at(id);
                std::cout << " (" << cs.getActiveWorkshops() << "/"
                    << cs.getTotalWorkshops() << " цехов)";
            }
            std::cout << "\n";
        }
        std::cout << std::string(60, '-') << "\n";

        std::cout << "  Порядок обработки (ID):\n   ";
        for (size_t i = 0; i < order.size(); ++i) {
            std::cout << order[i];
            if (i < order.size() - 1) {
                std::cout << " → ";
            }
        }
        std::cout << "\n";
        Logger::log("Топологическая сортировка выполнена успешно, станций: " +
            std::to_string(order.size()));

    }
    catch (const std::exception& ex) {
        std::cout << "НЕПРЕДВИДЕННАЯ ОШИБКА\n\n";
        std::cout << "Сообщение об ошибке: " << ex.what() << "\n\n";

        Logger::log("Ошибка топосортировки: " + std::string(ex.what()));
    }
}