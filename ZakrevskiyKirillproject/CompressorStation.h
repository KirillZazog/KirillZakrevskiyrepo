#ifndef COMPRESSOR_STATION_H
#define COMPRESSOR_STATION_H

#include <string>
#include <iostream>

class CompressorStation {
private:
    std::string name;
    int totalWorkshops;
    int activeWorkshops;
    std::string efficiency;

public:
    CompressorStation();
    CompressorStation(const std::string& name, int total, int active, const std::string& eff);

    std::string getName() const { return name; }
    int getTotalWorkshops() const { return totalWorkshops; }
    int getActiveWorkshops() const { return activeWorkshops; }
    std::string getEfficiency() const { return efficiency; }
    double getUnusedPercent() const;

    void setName(const std::string& newName) { name = newName; }
    void setTotalWorkshops(int total) { totalWorkshops = total; }
    void setActiveWorkshops(int active);
    void setEfficiency(const std::string& eff) { efficiency = eff; }

    void edit();

    void save(std::ostream& out, int id) const;
    static CompressorStation load(std::istream& in, int& id);

    friend std::ostream& operator<<(std::ostream& os, const CompressorStation& cs);
};

#endif