#include "Pipe.h"
#include "Validation.h"
#include <iomanip>

Pipe::Pipe() : name(""), length(0), diameter(0), inRepair(false) {}

Pipe::Pipe(const std::string& name, float length, int diameter, bool inRepair)
    : name(name), length(length), diameter(diameter), inRepair(inRepair) {
}

void Pipe::display(int id) const {
    std::cout << "Труба ID: " << id << "\n"
        << "  Название: " << name << "\n"
        << "  Длина: " << length << " км\n"
        << "  Диаметр: " << diameter << " мм\n"
        << "  Статус: " << (inRepair ? "в ремонте" : "не в ремонте") << "\n";
}

void Pipe::edit() {
    std::cout << "Текущий статус: " << (inRepair ? "в ремонте" : "не в ремонте") << "\n";
    inRepair = getValidBool("Введите новый статус (0-не в ремонте/1-в ремонте)");
    std::cout << "Статус обновлён!\n";
}

void Pipe::save(std::ostream& out, int id) const {
    out << "<PIPE>\n"
        << id << "\n"
        << name << "\n"
        << length << "\n"
        << diameter << "\n"
        << (inRepair ? 1 : 0) << "\n"
        << "</PIPE>\n";
}

Pipe Pipe::load(std::istream& in, int& id) {
    std::string line;
    std::getline(in, line);
    id = std::stoi(line);

    std::string name;
    std::getline(in, name);

    float length;
    in >> length;

    int diameter;
    in >> diameter;

    int repairStatus;
    in >> repairStatus;
    in.ignore();

    std::getline(in, line);

    return Pipe(name, length, diameter, repairStatus == 1);
}