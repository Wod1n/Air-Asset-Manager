#include <iostream>
#include <string>

#include "headers/aircraft.h"

void Aircraft::setRegion(int newRegion){
  currentLocation.setRegion(newRegion);
}

void Aircraft::setLocation(int fly, std::string spot){
  currentLocation.setLocation(fly, spot);
}

std::string Aircraft::getNumber(){
  return serialNumber;
};

int Aircraft::getRegionCode(){
  return currentLocation.getRegionCode();
}

std::string Aircraft::getSpread(){
  if(stowed){
    return "Stowed";
  }

  else{
    return "Spread";
  }
}

std::string Aircraft::getRegion(){
  return currentLocation.getRegion();
}

std::string Aircraft::getType(){
  switch(type){
    case 0:{
      return "Merlin";
    }

    case 1:{
      return "Wildcat";
    }

    default:{
      return "Unlisted Type";
    }
  }
}

std::string Aircraft::getSquadron(){
  return "Flight 865";
}

std::string Aircraft::getDetails(bool ex){
  if(ex){
    std::string outputstream{};
    outputstream += std::to_string(type) + " ";
    outputstream += serialNumber + " ";
    outputstream += currentLocation.saveData() + " ";
    outputstream += std::to_string(squadron) + " ";
    outputstream += std::to_string(stowed) + " ";
    outputstream += std::to_string(fuelLevel) + "\n";

    return outputstream;
  }

  else{
    std::string outputstream{};
    outputstream += getType() += "\n";
    outputstream += serialNumber + "\n";
    outputstream += getRegion() += "\n";
    outputstream += getSquadron() += "\n";
    outputstream += getSpread() += "\n";
    outputstream += std::to_string(fuelLevel) + "%\n";
    outputstream += "-------------------------\n";

    return outputstream;
  }
}

Aircraft::Aircraft(){
  std::cout << "Input Serial Number: ";
  std::cin >> serialNumber;
  std::cout << "Input Type: ";
  std::cin >> type;
  Location *newLocation = new Location;
  currentLocation = *newLocation;
  std::cout << "Input Squadron: ";
  std::cin >> squadron;
  std::cout << "Input Fuel Level: ";
  std::cin >> fuelLevel;
};

Aircraft::Aircraft(int imType, std::string imSerialNumber, int imSquadron, bool imStowed, int imFuel, Location imLocation){
  type = imType;
  serialNumber = imSerialNumber;
  currentLocation = imLocation;
  squadron = imSquadron;
  stowed = imStowed;
  fuelLevel = imFuel;
}
