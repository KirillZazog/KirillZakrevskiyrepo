#include <fstream>
#include <iostream>

using namespace std;

struct Pipe
{
	string name;
	float lenth;
	int diametr;
	bool status;
};

struct CS
{
	string name;
	int num_work;
	int num_work_online;
	string class_cs;
};

int main()
{
	Pipe truba;
	CS comp_station;
	cout << "Введите название трубы: ";
	cin >> truba.name;
	cout << "Введите название КС: ";
	cin >> comp_station.name;
	cout << "труба " << truba.name << "\nКС " << comp_station.name;
	return 1;
}