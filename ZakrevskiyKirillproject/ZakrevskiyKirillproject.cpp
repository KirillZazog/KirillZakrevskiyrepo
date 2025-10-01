#include <fstream>
#include <iostream>
#include <string>

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

void clear_stdin() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void AddPipe(Pipe &t) {
	cout << "\nВведите название трубы:";
	clear_stdin();
	getline(cin, t.name);

	cout << "Введите длину трубы (км, положительное число):";
	while (!(cin >> t.length) || t.length < 0.0f) {
		cout << "Неверный ввод. Введите положительное число для длины: ";
		clear_stdin();
	}

	cout << "Введите диаметр трубы (м, положительное число):";
	while (!(cin >> t.diameter) || t.diameter <= 0) {
		cout << "Неверный ввод. Введите положительное целое для диаметра: ";
		clear_stdin();
	}
	cout << "Введите статус трубы (0-не в ремонте/1-в ремонте):";
	int temp_status;
	while (!(cin >> temp_status) || (temp_status != 0 && temp_status != 1)) {
		cout << "Неверный ввод. Введите 0 или 1: ";
		clear_stdin();
	}
	t.status = (temp_status == 1);
	clear_stdin();
}

void AddCS(CS &c) {
	cout << "\nВведите название КС:";
	clear_stdin();
	getline(cin, c.name);
	cout << "Введите количество цехов (целое >= 0):";
	while (!(cin >> c.num_work) || c.num_work < 0) {
		cout << "Неверный ввод. Введите целое число >= 0:";
		clear_stdin();
	}
	cout << "Введите количество рабочих цехов (целое, 0..n):";
	while (!(cin >> c.num_work_online) || c.num_work_online < 0 || c.num_work_online > c.num_work) {
		cout << "Неверный ввод. Введите целое в диапазоне 0.." << c.num_work << ";";
		clear_stdin();
	}
	cout << "Введите класс станциии:";
	getline(cin, c.class_cs);
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
	cout << "Текущий статус трубы: " << (t.status ? "в ремонте" : "не в ремонте") << endl;
	cout << "Введите новый статус трубы (0-не в ремонте/1-в ремонте): ";
	int temp_status;
	while (!(cin >> temp_status) || (temp_status != 0 && temp_status != 1)) {
		cout << "Неверный ввод. Введите 0 или 1: ";
		clear_stdin();
	}
	t.status = (temp_status == 1);
	clear_stdin();
}

void EditCS(CS &c) {
	cout << "Текущее количество рабочих цехов: " << c.num_work_online << " из " << c.num_work << endl;
	cout << "Чтобы запустить цех, введите: 1\nЧтобы остановить цех, введите: 0\nВаш выбор: ";
	int opt;
	while (!(cin >> opt) || (opt != 0 && opt != 1)) {
		cout << "Неверный ввод. Введите 1 для запуска, 0 для остановки: ";
		clear_stdin();
	}
	if (opt == 1) {
		if (c.num_work_online < c.num_work) {
			c.num_work_online++;
			cout << "Цех запущен. Рабочих цехов: " << c.num_work_online << "\n";
		}
		else { cout << "Все цеха уже в работе, запуск невозможен.\n"; }
	}
	else {
		if (c.num_work_online > 0) {
			c.num_work_online--;
			cout << "Цех остановлен. Рабочих цехов: " << c.num_work_online << "\n";
		}
		else {
			cout << "Нет рабочих цехов для остановки.\n";
		}
	}
	clear_stdin();
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