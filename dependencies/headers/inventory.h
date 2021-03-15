#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>

#include "aircraft.h"

class Inventory{
  private:
    std::vector<Aircraft> embarkedAircraft;
    std::vector<int> hangarlist{};
    std::vector<int> fdlist{};
    std::vector<int> transitlist{};
    std::vector<int> onmission{};
    std::vector<std::string> embarkedSquadrons{};
  public:
    void addAircraft();
    void removeAircraft();
    void transferAircraft();
    std::string viewSquadrons();
    std::string getSquadron(int i);
    void addSquadron();
    std::string outputData(int i);
    Aircraft getAircraft(int i);

    Inventory();
    ~Inventory();
};

#endif
