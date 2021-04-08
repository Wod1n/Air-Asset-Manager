#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "headers/aircraft.h"

void Aircraft::setRegion(int newRegion){
  currentLocation.setRegion(newRegion);
}

void Aircraft::setLocation(int fly, std::string spot){
  currentLocation.setLocation(fly, spot);
}

void Aircraft::setLocation(int region, int left, int top){
  currentLocation.setLocation(region, left, top);
}

bool Aircraft::getStowed(){
  return stowed;
}

std::string Aircraft::getNumber(){
  return serialNumber;
}

int Aircraft::getRegionCode(){
  return currentLocation.getRegionCode();
}

int Aircraft::getTypeCode(){
  return type;
}

int Aircraft::getLeft(){
  return currentLocation.getLeft();
}

int Aircraft::getTop(){
  return currentLocation.getTop();
}

int Aircraft::getFuel(){
  return fuelLevel;
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
  std::ifstream typeFile;
  typeFile.open("./types.txt", std::ios::in);
  std::vector<std::string> aircraftTypes{};

  if(typeFile.is_open()){
    std::string readType{};

    while(typeFile){
      typeFile >> readType;
      if(readType == "-1"){
        break;
      }
      aircraftTypes.push_back(readType);
    }
  }

  typeFile.close();

  return aircraftTypes.at(type);
}

std::string Aircraft::getSquadron(){
  std::ifstream squadronFile;
  squadronFile.open("./squadrons.txt", std::ios::in);
  std::vector<std::string> embarkedSquadrons{};

  if(squadronFile.is_open()){
    std::string readSquadron{};

    while(squadronFile){
      squadronFile >> readSquadron;
      if(readSquadron == "-1"){
        break;
      }
      embarkedSquadrons.push_back(readSquadron);
    }
  }
  return embarkedSquadrons.at(squadron);
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
