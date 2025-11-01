#include <fstream>
#include <iostream>
#include <string>
#include <limits>

using namespace std;

struct Pipe
{
	string name;
	float length = 0.0f;
	int diameter = 0;
	bool status = false;
};

struct CS
{
	string name;
	int num_work = 0;
	int num_work_online = 0;
	string class_cs;
};

float getValidFloat(const string& prompt,
	float min = -numeric_limits<float>::infinity(),
	float max = numeric_limits<float>::infinity()) {
	string input;
	float value;

	cout << prompt;

	while (true) {
		getline(cin, input);

		size_t start = input.find_first_not_of(" \t");
		size_t end = input.find_last_not_of(" \t");

		if (start == string::npos) {
			cout << "Ошибка: пустой ввод. Попробуйте снова: ";
			continue;
		}

		input = input.substr(start, end - start + 1);

		try {
			size_t pos = 0;
			value = stof(input, &pos);

			if (pos != input.length()) {
				cout << "Ошибка: введите корректное число. Попробуйте снова: ";
				continue;
			}

			if (value < min || value > max) {
				if (min == -numeric_limits<float>::infinity()) {
					cout << "Ошибка: число должно быть <= " << max << ". Попробуйте снова: ";
				}
				else if (max == numeric_limits<float>::infinity()) {
					cout << "Ошибка: число должно быть >= " << min << ". Попробуйте снова: ";
				}
				else {
					cout << "Ошибка: число должно быть в диапазоне [" << min << ", " << max << "]. Попробуйте снова: ";
				}
				continue;
			}

			return value;
		}
		catch (const invalid_argument&) {
			cout << "Ошибка: введите корректное число. Попробуйте снова: ";
		}
		catch (const out_of_range&) {
			cout << "Ошибка: число слишком большое. Попробуйте снова: ";
		}
	}
}

int getValidInt(const string& prompt,
	int min = numeric_limits<int>::min(),
	int max = numeric_limits<int>::max()) {
	string input;
	int value;

	cout << prompt;

	while (true) {
		getline(cin, input);

		size_t start = input.find_first_not_of(" \t");
		size_t end = input.find_last_not_of(" \t");

		if (start == string::npos) {
			cout << "Ошибка: пустой ввод. Попробуйте снова: ";
			continue;
		}

		input = input.substr(start, end - start + 1);

		try {
			size_t pos = 0;
			value = stoi(input, &pos);

			if (pos != input.length()) {
				cout << "Ошибка: введите корректное целое число. Попробуйте снова: ";
				continue;
			}

			if (value < min || value > max) {
				if (min == numeric_limits<int>::min()) {
					cout << "Ошибка: число должно быть <= " << max << ". Попробуйте снова: ";
				}
				else if (max == numeric_limits<int>::max()) {
					cout << "Ошибка: число должно быть >= " << min << ". Попробуйте снова: ";
				}
				else {
					cout << "Ошибка: число должно быть в диапазоне [" << min << ", " << max << "]. Попробуйте снова: ";
				}
				continue;
			}

			return value;
		}
		catch (const invalid_argument&) {
			cout << "Ошибка: введите корректное целое число. Попробуйте снова: ";
		}
		catch (const out_of_range&) {
			cout << "Ошибка: число слишком большое. Попробуйте снова: ";
		}
	}
}

string getValidString(const string& prompt,
	bool allowEmpty = false) {
	string input;

	cout << prompt;

	while (true) {
		getline(cin, input);

		size_t start = input.find_first_not_of(" \t");
		size_t end = input.find_last_not_of(" \t");

		if (start == string::npos) {
			if (allowEmpty) {
				return "";
			}
			cout << "Ошибка: пустая строка недопустима. Попробуйте снова: ";
			continue;
		}

		input = input.substr(start, end - start + 1);
		return input;
	}
}

bool getValidBool(const string& prompt) {
	string input;

	cout << prompt << " (0-нет/1-да): ";

	while (true) {
		getline(cin, input);

		size_t start = input.find_first_not_of(" \t");
		size_t end = input.find_last_not_of(" \t");

		if (start == string::npos) {
			cout << "Ошибка: пустой ввод. Попробуйте снова: ";
			continue;
		}

		input = input.substr(start, end - start + 1);

		for (char& c : input) {
			c = tolower(c);
		}

		if (input == "да" || input == "yes" || input == "y" ||
			input == "1" || input == "true" || input == "т") {
			return true;
		}

		if (input == "нет" || input == "no" || input == "n" ||
			input == "0" || input == "false" || input == "н") {
			return false;
		}

		cout << "Ошибка: введите 0 или 1. Попробуйте снова: ";
	}
}

void AddPipe(Pipe& t) {
	cout << "\n=== ДОБАВЛЕНИЕ ТРУБЫ ===\n";

	t.name = getValidString("Введите название трубы: ");
	t.length = getValidFloat("Введите длину трубы (км, положительное число): ", 0.0f);
	t.diameter = getValidInt("Введите диаметр трубы (мм, положительное число): ", 1);
	t.status = getValidBool("Статус трубы (0-не в ремонте/1-в ремонте)");

	cout << "Труба успешно добавлена!\n";
}

void AddCS(CS& c) {
	cout << "\n=== ДОБАВЛЕНИЕ КОМПРЕССОРНОЙ СТАНЦИИ ===\n";

	c.name = getValidString("Введите название КС: ");
	c.num_work = getValidInt("Введите количество цехов (целое >= 0): ", 0);
	c.num_work_online = getValidInt("Введите количество рабочих цехов (0.." + to_string(c.num_work) + "): ", 0, c.num_work);
	c.class_cs = getValidString("Введите класс станции: ");

	cout << "Компрессорная станция успешно добавлена!\n";
}

void ShowAll(const Pipe& t, const CS& c) {
	cout << "\n=== ВСЕ ОБЪЕКТЫ ===\n";

	if (t.name.empty() && c.name.empty()) {
		cout << "Нет данных для отображения. Добавьте трубу или КС.\n";
		return;
	}

	if (!t.name.empty()) {
		cout << "\n--- ТРУБА ---\n";
		cout << "Название: " << t.name << endl;
		cout << "Длина: " << t.length << " км" << endl;
		cout << "Диаметр: " << t.diameter << " мм" << endl;
		cout << "Статус: " << (t.status ? "в ремонте" : "не в ремонте") << endl;
	}

	if (!c.name.empty()) {
		cout << "\n--- КОМПРЕССОРНАЯ СТАНЦИЯ ---\n";
		cout << "Название: " << c.name << endl;
		cout << "Количество цехов: " << c.num_work << endl;
		cout << "Количество рабочих цехов: " << c.num_work_online << endl;
		cout << "Класс станции: " << c.class_cs << endl;
	}
}

void EditPipe(Pipe& t) {
	if (t.name.empty()) {
		cout << "\nОшибка: труба не создана. Сначала добавьте трубу (опция 1).\n";
		return;
	}

	cout << "\n=== РЕДАКТИРОВАНИЕ ТРУБЫ ===\n";
	cout << "Текущий статус: " << (t.status ? "в ремонте" : "не в ремонте") << endl;

	t.status = getValidBool("Введите новый статус (0-не в ремонте/1-в ремонте)");

	cout << "Статус трубы обновлён!\n";
}

void EditCS(CS& c) {
	if (c.name.empty()) {
		cout << "\nОшибка: КС не создана. Сначала добавьте КС (опция 2).\n";
		return;
	}

	cout << "\n=== РЕДАКТИРОВАНИЕ КС ===\n";
	cout << "Текущее количество рабочих цехов: " << c.num_work_online << " из " << c.num_work << endl;

	if (c.num_work == 0) {
		cout << "Невозможно изменить количество рабочих цехов: всего цехов = 0.\n";
		return;
	}

	cout << "1. Запустить цех\n";
	cout << "0. Остановить цех\n";
	int opt = getValidInt("Ваш выбор: ", 0, 1);

	if (opt == 1) {
		if (c.num_work_online < c.num_work) {
			c.num_work_online++;
			cout << "Цех запущен. Рабочих цехов: " << c.num_work_online << "\n";
		}
		else {
			cout << "Все цеха уже в работе, запуск невозможен.\n";
		}
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
}

int Save(const Pipe& t, const CS& c) {
	cout << "\n=== СОХРАНЕНИЕ ДАННЫХ ===\n";

	if (t.name.empty() && c.name.empty()) {
		cout << "Нет данных для сохранения.\n";
		return 1;
	}

	ofstream fout("data.txt");
	if (!fout) {
		cerr << "Ошибка при создании/перезаписи файла data.txt!\n";
		return 1;
	}

	fout << "Труба:\n"
		<< t.name << '\n'
		<< t.length << '\n'
		<< t.diameter << '\n'
		<< (t.status ? 1 : 0) << '\n'
		<< "Компрессорная станция:\n"
		<< c.name << '\n'
		<< c.num_work << '\n'
		<< c.num_work_online << '\n'
		<< c.class_cs << '\n';

	fout.close();
	cout << "Данные успешно сохранены в файл data.txt!\n";
	return 0;
}

int Download(Pipe& t, CS& c) {
	cout << "\n=== ЗАГРУЗКА ДАННЫХ ===\n";

	ifstream fin("data.txt");
	if (!fin) {
		cerr << "Ошибка при открытии файла data.txt (файл не найден?)\n";
		return 1;
	}

	string line;
	bool gotPipe = false;
	bool gotCS = false;

	try {
		while (getline(fin, line)) {
			if (line == "Труба:") {
				if (!getline(fin, t.name)) throw runtime_error("Ожидалось имя трубы");
				if (!getline(fin, line)) throw runtime_error("Ожидалась длина трубы");
				t.length = stof(line);
				if (!getline(fin, line)) throw runtime_error("Ожидался диаметр трубы");
				t.diameter = stoi(line);
				if (!getline(fin, line)) throw runtime_error("Ожидался статус трубы");
				if (line == "1" || line == "true" || line == "True") t.status = true;
				else t.status = false;
				gotPipe = true;
			}
			else if (line == "Компрессорная станция:") {
				if (!getline(fin, c.name)) throw runtime_error("Ожидалось имя КС");
				if (!getline(fin, line)) throw runtime_error("Ожидалось количество цехов");
				c.num_work = stoi(line);
				if (!getline(fin, line)) throw runtime_error("Ожидалось количество рабочих цехов");
				c.num_work_online = stoi(line);
				if (!getline(fin, c.class_cs)) throw runtime_error("Ожидался класс станции");
				if (c.num_work < 0) c.num_work = 0;
				if (c.num_work_online < 0) c.num_work_online = 0;
				if (c.num_work_online > c.num_work) c.num_work_online = c.num_work;
				gotCS = true;
			}
			else if (line.empty()) {
				continue;
			}
			else {
				continue;
			}
		}
	}
	catch (const invalid_argument& e) {
		cerr << "Ошибка формата числа при чтении файла: " << e.what() << '\n';
		return 2;
	}
	catch (const out_of_range& e) {
		cerr << "Число в файле выходит за допустимый диапазон: " << e.what() << '\n';
		return 3;
	}
	catch (const runtime_error& e) {
		cerr << "Ошибка при чтении файла: " << e.what() << '\n';
		return 4;
	}

	if (!gotPipe && !gotCS) {
		cerr << "Файл прочитан, но в нём не найдено ни секции 'Труба:', ни 'Компрессорная станция:'.\n";
		return 5;
	}

	cout << "Данные успешно загружены из data.txt!\n";
	return 0;
}

void Show_menu(Pipe& t, CS& c) {
	int option = 0;
	while (true) {
		cout << "\n========================================\n";
		cout << "           ГЛАВНОЕ МЕНЮ\n";
		cout << "========================================\n";
		cout << "1. Добавить трубу\n";
		cout << "2. Добавить КС\n";
		cout << "3. Посмотреть все объекты\n";
		cout << "4. Редактировать трубу\n";
		cout << "5. Редактировать КС\n";
		cout << "6. Сохранить\n";
		cout << "7. Загрузить\n";
		cout << "0. Выход\n";
		cout << "========================================\n";

		option = getValidInt("Ваш выбор: ", 0, 7);

		switch (option) {
		case 1: AddPipe(t); break;
		case 2: AddCS(c); break;
		case 3: ShowAll(t, c); break;
		case 4: EditPipe(t); break;
		case 5: EditCS(c); break;
		case 6: Save(t, c); break;
		case 7: Download(t, c); break;
		case 0:
			cout << "\nВыход из программы. До свидания!\n";
			return;
		default:
			cout << "\nНепредвиденная ошибка!\n";
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