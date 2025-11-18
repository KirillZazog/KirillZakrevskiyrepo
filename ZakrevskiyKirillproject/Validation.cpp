#include "Validation.h"

using namespace std;

float getValidFloat(const string& prompt, float min, float max) {
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

int getValidInt(const string& prompt, int min, int max) {
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

string getValidString(const string& prompt, bool allowEmpty) {
    string input;
    cout << prompt;

    while (true) {
        getline(cin, input);
        size_t start = input.find_first_not_of(" \t");
        size_t end = input.find_last_not_of(" \t");

        if (start == string::npos) {
            if (allowEmpty) return "";
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
        for (char& c : input) c = tolower(c);

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