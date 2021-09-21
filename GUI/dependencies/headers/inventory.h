#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>

#include "aircraft.h"
#include "shape.h"

class Inventory{
  private:
    std::vector<Aircraft> embarkedAircraft;
    std::vector<int> hangarlist{};
    std::vector<int> fdlist{};
    std::vector<int> transitlist{};
    std::vector<int> onmission{};
    std::vector<std::string> embarkedSquadrons{};
    std::vector<std::string> squadronColours{};
    std::vector<Shape> diagramShapes;
  public:
    void addAircraft();
    void addAircraft(Aircraft*);
    void editAircraft(int, bool, int, int, int);
    void setAircraftRotation(int, int);
    void removeAircraft(int);
    void transferAircraft(int, int, int=3, int=-1,int=-1);
    void setLocation(int, int, int, int);
    int getACNumber();
    std::string viewSquadrons();
    std::string getSquadron(int i);
    void addSquadron();
    std::string outputData(int i);
    Aircraft* getAircraft(int i);
    int searchAircraft(std::string, int i=-1, bool=0);
    Shape* getShape(int i);

    Inventory();
    ~Inventory();
};

#endif
