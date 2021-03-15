#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location{
  private:
    int xrange;
    int yrange;
    int region;
    int fly;
    std::string spot;
  public:
    int getxrange();
    int getRegionCode();
    void setRegion(int newRegion);
    void setLocation(int, std::string);
    std::string getLocation();
    std::string getRegion();
    std::string getSpot();
    std::string saveData();

    Location();
    Location(int, int, char);
};

#endif
