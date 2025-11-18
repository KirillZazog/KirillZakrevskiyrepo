#pragma once
#include <string>
#include <iostream>

class Pipe {
private:
    std::string name;
    float length;
    int diameter;
    bool inRepair;

public:
    Pipe();
    Pipe(const std::string& name, float length, int diameter, bool inRepair = false);

    std::string getName() const { return name; }
    float getLength() const { return length; }
    int getDiameter() const { return diameter; }
    bool isInRepair() const { return inRepair; }

    void setName(const std::string& newName) { name = newName; }
    void setLength(float newLength) { length = newLength; }
    void setDiameter(int newDiameter) { diameter = newDiameter; }
    void setInRepair(bool repair) { inRepair = repair; }

    void display(int id) const;
    void edit();
    void save(std::ostream& out, int id) const;
    static Pipe load(std::istream& in, int& id);
};