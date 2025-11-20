#include "CompressorStation.h"
#include "Validation.h"
#include <iomanip>

CompressorStation::CompressorStation()
    : name(""), totalWorkshops(0), activeWorkshops(0), efficiency("") {
}

CompressorStation::CompressorStation(const std::string& name, int total, int active, const std::string& eff)
    : name(name), totalWorkshops(total), activeWorkshops(active), efficiency(eff) {
}

double CompressorStation::getUnusedPercent() const {
    return totalWorkshops > 0 ?
        ((totalWorkshops - activeWorkshops) * 100.0 / totalWorkshops) : 0.0;
}

void CompressorStation::setActiveWorkshops(int active) {
    activeWorkshops = (active <= totalWorkshops) ? active : totalWorkshops;
}

void CompressorStation::edit() {
    if (totalWorkshops == 0) {
        std::cout << "Невозможно изменить: всего цехов = 0.\n";
        return;
    }

    std::cout << "Текущее количество рабочих цехов: " << activeWorkshops
        << " из " << totalWorkshops << "\n";
    std::cout << "1. Запустить цех\n";
    std::cout << "0. Остановить цех\n";

    int choice = getValidInt("Ваш выбор: ", 0, 1);

    if (choice == 1) {
        if (activeWorkshops < totalWorkshops) {
            activeWorkshops++;
            std::cout << "Цех запущен. Рабочих цехов: " << activeWorkshops << "\n";
        }
        else {
            std::cout << "Все цеха уже в работе.\n";
        }
    }
    else {
        if (activeWorkshops > 0) {
            activeWorkshops--;
            std::cout << "Цех остановлен. Рабочих цехов: " << activeWorkshops << "\n";
        }
        else {
            std::cout << "Нет рабочих цехов для остановки.\n";
        }
    }
}

void CompressorStation::save(std::ostream& out, int id) const {
    out << "<CS>\n"
        << id << "\n"
        << name << "\n"
        << totalWorkshops << "\n"
        << activeWorkshops << "\n"
        << efficiency << "\n"
        << "</CS>\n";
}

CompressorStation CompressorStation::load(std::istream& in, int& id) {
    std::string line;
    std::getline(in, line);
    id = std::stoi(line);

    std::string name;
    std::getline(in, name);

    int total, active;
    in >> total >> active;
    in.ignore();

    std::string eff;
    std::getline(in, eff);

    std::getline(in, line);

    return CompressorStation(name, total, active, eff);
}

std::ostream& operator<<(std::ostream& os, const CompressorStation& cs) {
    os << "КС: " << cs.name
        << " \n| Всего цехов: " << cs.totalWorkshops
        << " \n| Работающих цехов: " << cs.activeWorkshops
        << " \n| Класс: " << cs.efficiency;
    return os;
}