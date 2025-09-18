#include <fstream>
#include <iostream>

using namespace std;

struct Pipe
{
	string name;
	float length=0.0f;
	int diameter=0;
	bool status=false;
};

struct CS
{
	string name;
	int num_work=0;
	int num_work_online=0;
	string class_cs;
};

void AddPipe(Pipe &t) {
	system("cls");
	cout << "Введите название трубы:";
	cin >> t.name;
	cout << "Введите длину трубы:";
	cin >> t.length;
	cout << "Введите диаметр трубы:";
	cin >> t.diameter;
	cout << "Введите статус трубы (0-не в ремонте/1-в ремонте):";
	cin >> t.status;
}

void AddCS(CS& c) {
	system("cls");
	cout << "Введите название КС:";
	cin >> c.name;
	cout << "Введите количество цехов:";
	cin >> c.num_work;
	cout << "Введите количество рабочих цехов:";
	cin >> c.num_work_online;
	cout << "Введите класс станциии:";
	cin >> c.class_cs;
}

void ShowAll(Pipe& t, CS& c) {
	system("cls");
	cout << "Название трубы: " << t.name << endl
		<< "Длина трубы: " << t.length << endl
		<< "Диаметр трубы: " << t.diameter << endl
		<< "Статус трубы (0-не в ремонте/1-в ремонте): " << t.status << endl
		<< "\nНазвание КС: " << c.name << endl
		<< "Количество цехов: " << c.num_work << endl
		<< "Количество рабочих цехов: " << c.num_work_online << endl
		<< "Класс станции: " << c.class_cs << endl
		<< "\nНажмите Enter, чтобы вернуться в меню...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}

void Show_menu(Pipe &t, CS &c) {
	int option;
	while (true) {
		cout << "Выберите опцию:\n"
			<< "1. Добавить трубу\n"
			<< "2. Добавить КС\n"
			<< "3. Посмотреть все объекты\n"
			<< "4. Редактировать трубу\n"
			<< "5. Редактировать КС\n"
			<< "6. Сохранить\n"
			<< "7. Загрузить\n"
			<< "0. Выход\n"
			<< "Ваш выбор: ";
		cin >> option;
		switch (option) {
		case 1: AddPipe(t); system("cls"); break;
		case 2: AddCS(c); system("cls"); break;
		case 3: ShowAll(t, c); system("cls"); break;
		case 0: return;
		default: system("cls"); cout << "Выбрана несуществующая опция, введите число от 0 до 7!\n\n";
		}
	}
}

int main()
{
	Pipe truba;
	CS comp_station;
	Show_menu(truba, comp_station);
	return 0;
}