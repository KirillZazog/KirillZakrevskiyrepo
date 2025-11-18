#include "PipelineSystem.h"
#include "Logger.h"
#include "Validation.h"
#include <iostream>

void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "           ГЛАВНОЕ МЕНЮ\n";
    std::cout << "========================================\n";
    std::cout << "1. Добавить трубу\n";
    std::cout << "2. Добавить КС\n";
    std::cout << "3. Просмотреть все трубы\n";
    std::cout << "4. Просмотреть все КС\n";
    std::cout << "5. Редактировать трубу\n";
    std::cout << "6. Редактировать КС\n";
    std::cout << "7. Удалить трубу\n";
    std::cout << "8. Удалить КС\n";
    std::cout << "9. Поиск труб\n";
    std::cout << "10. Поиск КС\n";
    std::cout << "11. Пакетное редактирование труб\n";
    std::cout << "12. Пакетное удаление труб\n";
    std::cout << "13. Сохранить в файл\n";
    std::cout << "14. Загрузить из файла\n";
    std::cout << "0. Выход\n";
    std::cout << "========================================\n";
}

int main() {
    Logger::init();
    PipelineSystem system;

    while (true) {
        displayMenu();
        int choice = getValidInt("Ваш выбор: ", 0, 14);

        switch (choice) {
        case 1:
            system.addPipe();
            break;
        case 2:
            system.addStation();
            break;
        case 3:
            system.displayAllPipes();
            break;
        case 4:
            system.displayAllStations();
            break;
        case 5:
            system.editPipe();
            break;
        case 6:
            system.editStation();
            break;
        case 7:
            system.deletePipe();
            break;
        case 8:
            system.deleteStation();
            break;
        case 9:
            system.searchPipes();
            break;
        case 10:
            system.searchStations();
            break;
        case 11:
            system.batchEditPipes();
            break;
        case 12:
            system.batchDeletePipes();
            break;
        case 13: {
            std::string filename = getValidString("Введите имя файла для сохранения: ");
            system.saveToFile(filename);
            break;
        }
        case 14: {
            std::string filename = getValidString("Введите имя файла для загрузки: ");
            system.loadFromFile(filename);
            break;
        }
        case 0:
            Logger::close();
            std::cout << "\nВыход из программы. До свидания!\n";
            return 0;
        }
    }

    return 0;
}