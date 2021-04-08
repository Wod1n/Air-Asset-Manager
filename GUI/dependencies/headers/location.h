#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location{
  private:
    int left{0};
    int top{0};
    int region;
    int fly{-1};
    std::string spot{"Z"};
  public:
    int getWidth();
    int getHeight();
    int getLeft();
    int getTop();
    int getRegionCode();
    void setRegion(int newRegion);
    void setLocation(int, std::string);
    void setLocation(int, int, int);
    std::string getLocation();
    std::string getRegion();
    std::string getSpot();
    std::string saveData();

    Location();
    Location(int, int, char);
    Location(int, int, int);
};

#endif
