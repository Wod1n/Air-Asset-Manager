#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include "location.h"

class Aircraft{
  private:
    bool stowed;
    std::string serialNumber;
    Location currentLocation{0,0,0};
    int fuelLevel;
    int type;
    int squadron;

  public:
    Aircraft();
    Aircraft(int, std::string, int, bool, int, Location);
    void setRegion(int newRegion);
    void setLocation(int, std::string);
    int getRegionCode();
    std::string getNumber();
    std::string getRegion();
    std::string getType();
    std::string getSquadron();
    std::string getSpread();
    std::string getDetails(bool ex=0);
};

#endif
