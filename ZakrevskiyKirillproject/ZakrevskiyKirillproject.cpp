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
	cout << "\nВведите название трубы:";
	cin >> t.name;
	cout << "Введите длину трубы:";
	cin >> t.length;
	cout << "Введите диаметр трубы:";
	cin >> t.diameter;
	cout << "Введите статус трубы (0-не в ремонте/1-в ремонте):";
	cin >> t.status;
}

void AddCS(CS &c) {
	cout << "\nВведите название КС:";
	cin >> c.name;
	cout << "Введите количество цехов:";
	cin >> c.num_work;
	cout << "Введите количество рабочих цехов:";
	cin >> c.num_work_online;
	cout << "Введите класс станциии:";
	cin >> c.class_cs;
}

void ShowAll(Pipe &t, CS &c) {
	cout << "\nНазвание трубы: " << t.name << endl
		<< "Длина трубы: " << t.length << endl
		<< "Диаметр трубы: " << t.diameter << endl
		<< "Статус трубы (0-не в ремонте/1-в ремонте): " << t.status << endl
		<< "\nНазвание КС: " << c.name << endl
		<< "Количество цехов: " << c.num_work << endl
		<< "Количество рабочих цехов: " << c.num_work_online << endl
		<< "Класс станции: " << c.class_cs << endl;
}

void EditPipe(Pipe &t) {
	cout << "\nВведите новый статус трубы (0-не в ремонте/1-в ремонте): ";
	cin >> t.status;
}

void EditCS(CS &c) {
	int opt;
	cout << "\nЧтобы запустить цех, введите: 1" << endl
		<< "Чтобы остановить цех, введите: 0\n";
	cin >> opt;
	if (opt == 1) {c.num_work_online++; }
	else { c.num_work_online--; }
}

void Show_menu(Pipe &t, CS &c) {
	int option;
	while (true) {
		cout << "\nВыберите опцию:\n"
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
		case 1: AddPipe(t); break;
		case 2: AddCS(c); break;
		case 3: ShowAll(t, c); break;
		case 4: EditPipe(t); break;
		case 5: EditCS(c); break;
		case 0: return;
		default: cout << "\nВыбрана несуществующая опция, введите число от 0 до 7!\n\n";
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