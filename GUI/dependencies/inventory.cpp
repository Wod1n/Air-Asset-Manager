#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

#include "headers/aircraft.h"
#include "headers/inventory.h"
#include "headers/shape.h"

void Inventory::addAircraft(){
  Aircraft *new_aircraft = new Aircraft;
  embarkedAircraft.push_back(*new_aircraft);
  switch(new_aircraft->getRegionCode()) {
    case 1:{
      hangarlist.push_back(embarkedAircraft.size()-1);
      break;
    }

    case 2:{
      fdlist.push_back(embarkedAircraft.size()-1);
      break;
    }

    case 3:{
      onmission.push_back(embarkedAircraft.size()-1);
      break;
    }

    case 4:{
      transitlist.push_back(embarkedAircraft.size()-1);
      break;
    }
  }
}

void Inventory::addAircraft(Aircraft* new_aircraft){
  embarkedAircraft.push_back(*new_aircraft);
  switch(new_aircraft->getRegionCode()) {
    case 0:{
      fdlist.push_back(embarkedAircraft.size()-1);
      break;
    }

    case 1:{
      hangarlist.push_back(embarkedAircraft.size()-1);
      break;
    }

    case 2:{
      onmission.push_back(embarkedAircraft.size()-1);
      break;
    }

    case 3:{
      transitlist.push_back(embarkedAircraft.size()-1);
      break;
    }
  }
}

void Inventory::editAircraft(int i, bool stowed, int type, int squadron, int fuelLevel){
  embarkedAircraft.at(i).editAircraft(stowed, type, squadron, fuelLevel);
}

void Inventory::removeAircraft(int activeIndex){
  int forDeletion = -1;
  for(int i=0; i<fdlist.size(); i++){
    if(activeIndex == fdlist.at(i)){
      forDeletion = i;
    }
    else if(activeIndex < fdlist.at(i)){
      fdlist.at(i)--;
    }
  }
  if(forDeletion != -1){
    fdlist.erase(fdlist.begin()+forDeletion);
  }

  forDeletion = -1;
  for(int i=0; i<hangarlist.size(); i++){
    if(activeIndex == hangarlist.at(i)){
      forDeletion = i;
    }
    else if(activeIndex < hangarlist.at(i)){
      hangarlist.at(i)--;
    }
  }
  if(forDeletion != -1){
    hangarlist.erase(hangarlist.begin()+forDeletion);
  }

  forDeletion = -1;
  for(int i=0; i<onmission.size(); i++){
    if(activeIndex == onmission.at(i)){
      forDeletion = i;
    }
    else if(activeIndex < onmission.at(i)){
      onmission.at(i)--;
    }
  }
  if(forDeletion != -1){
    onmission.erase(onmission.begin()+forDeletion);
  }

  forDeletion = -1;
  for(int i=0; i<transitlist.size(); i++){
    if(activeIndex == transitlist.at(i)){
      forDeletion = i;
    }
    else if(activeIndex < transitlist.at(i)){
      transitlist.at(i)--;
    }
  }
  if(forDeletion != -1){
    transitlist.erase(transitlist.begin()+forDeletion);
  }

  embarkedAircraft.erase(embarkedAircraft.begin()+activeIndex);
}

std::string Inventory::viewSquadrons(){
  std::string outputstream{};
  for(int i=0; i<embarkedSquadrons.size(); i++){
    outputstream += embarkedSquadrons.at(i) +"\n";
  }
  return outputstream;
}

void Inventory::addSquadron(){
  std::string squadron{};
  std::cout << "Input Name of Squadron: ";
  std::cin >> squadron;
  embarkedSquadrons.push_back(squadron);
}

std::string Inventory::getSquadron(int i){
  return embarkedSquadrons.at(i);
}

int Inventory::getACNumber(){
  return embarkedAircraft.size();
}

std::string Inventory::outputData(int selection){
  std::string outputstream{};

  switch (selection) {
    case 1:{
      for (int i=0; i<hangarlist.size(); i++){
        outputstream += embarkedAircraft.at(hangarlist.at(i)).getDetails();
      }
      return outputstream;
    }

    case 2:{
      for (int i=0; i<fdlist.size(); i++){
        outputstream += embarkedAircraft.at(fdlist.at(i)).getDetails();
      }
      return outputstream;
    }

    case 3:{
      for (int i=0; i<onmission.size(); i++){
        outputstream += embarkedAircraft.at(onmission.at(i)).getDetails();
      }
      return outputstream;
    }

    case 4:{
      for (int i=0; i<transitlist.size(); i++){
        outputstream += embarkedAircraft.at(transitlist.at(i)).getDetails();
      }
      return outputstream;
    }

    default:{
      for (int i=0; i<embarkedAircraft.size();  i++){
        outputstream += embarkedAircraft.at(i).getDetails();
      }
      return outputstream;
    }
  }
}

void Inventory::transferAircraft(int aircraftIndex, int initialRegion, int finalRegion){

  int transfer{};
  std::string message{};
  std::time_t now = std::time(0);
  std::tm* currentTime = std::localtime(&now);
  std::string time = std::to_string(currentTime->tm_hour) + ":" + std::to_string(currentTime->tm_min);

  switch(initialRegion){
    case 0:{
      transfer = fdlist.at(aircraftIndex);
      message += time + " Aircraft " + embarkedAircraft.at(transfer).getNumber() + " from Flight Deck ";
      fdlist.erase(fdlist.begin()+aircraftIndex);
      break;
    }

    case 1:{
      transfer = hangarlist.at(aircraftIndex);
      message += time + " Aircraft " + embarkedAircraft.at(transfer).getNumber() + " from Hangar ";
      hangarlist.erase(hangarlist.begin()+aircraftIndex);
      break;
    }

    case 2:{
      transfer = onmission.at(aircraftIndex);
      message += time + " Aircraft " + embarkedAircraft.at(transfer).getNumber() + " from Deployment ";
      onmission.erase(onmission.begin()+aircraftIndex);
      break;
    }

    case 3:{
      transfer = transitlist.at(aircraftIndex);
      message += time + " Aircraft " + embarkedAircraft.at(transfer).getNumber() + " from Transit ";
      transitlist.erase(transitlist.begin()+aircraftIndex);
      break;
    }
  }

  embarkedAircraft.at(transfer).setRegion(finalRegion);

  switch(finalRegion){

    case 0:{
      fdlist.push_back(transfer);
      message += "to Flight Deck";
      break;
    }

    case 1:{
      hangarlist.push_back(transfer);
      message += "to Hangar";
      break;
    }

    case 2:{
      onmission.push_back(transfer);
      message += "to Deployment";
      break;
    }

    case 3:{
      transitlist.push_back(transfer);
      message += "to Transit";
      break;
    }
  }

  std::cout << message << std::endl;
}

void Inventory::setLocation(int region, int index, int left, int top){
  switch (region) {
    case 0:{
      std::cout << "Flight Deck\n";
      break;
    }

    case 1:{
      embarkedAircraft.at(index).setLocation(region, left, top);
      break;
    }
  }
}

Aircraft* Inventory::getAircraft(int i){
  return &(embarkedAircraft.at(i));
}
/*
Aircraft* Inventory::searchAircraft(std::string searchNumber, int searchRegion){
  switch (searchRegion) {
    case 1:{
      for (int i=0; i < hangarlist.size(); i++){
        if(embarkedAircraft.at(hangarlist.at(i)).getNumber() == searchNumber){
          return &(embarkedAircraft.at(hangarlist.at(i)));
        }
      }
      break;
    }
  }
}
*/
int Inventory::searchAircraft(std::string searchNumber, int searchRegion, bool returnRegion){
  switch (searchRegion) {
    case 1:{
      std::cout << "Searching Hanger\n";
      for(int i=0; i < hangarlist.size(); i++){
        if(embarkedAircraft.at(hangarlist.at(i)).getNumber() == searchNumber){
          if(returnRegion){
            return i;
          }
          return hangarlist.at(i);
        }
      }
      break;
    }

    case 2:{
      std::cout << "Searching Mission\n";
      for(int i=0; i < onmission.size(); i++){
        if(embarkedAircraft.at(onmission.at(i)).getNumber() == searchNumber){
          if(returnRegion){
            return i;
          }
          return onmission.at(i);
        }
      }
      break;
    }

    case 3:{
      std::cout << "Searching Transit\n";
      for(int i=0; i < transitlist.size(); i++){
        if(embarkedAircraft.at(transitlist.at(i)).getNumber() == searchNumber){
          std::cout << transitlist.at(i) << std::endl;
          if(returnRegion){
            return i;
          }
          return transitlist.at(i);
        }
      }
      break;
    }

    default:{
      std::cout << "Searching All\n";
      for(int i=0; i < embarkedAircraft.size(); i++){
        if(embarkedAircraft.at(i).getNumber() == searchNumber){
          return i;
        }
      }
    }
  }
}

Shape* Inventory::getShape(int i){
  return &(diagramShapes.at(i));
}

Inventory::Inventory(){
  std::ifstream aircraftFile;
  aircraftFile.open("./saved_inventory.txt", std::ios::in);

  if(aircraftFile.is_open()){
    int imType{};
    std::string serialNumber{};
    int imRegion{};
    int imLeft{};
    int imTop{};
    int imSquadron{};
    bool imStowed{};
    int imFuel{};

    while(aircraftFile){
      aircraftFile >> imType;
      if(imType == -1){
        break;
      }
      aircraftFile >> serialNumber;
      aircraftFile >> imRegion;
      aircraftFile >> imLeft;
      aircraftFile >> imTop;
      aircraftFile >> imSquadron;
      aircraftFile >> imStowed;
      aircraftFile >> imFuel;
      Location* newLocation = new Location(imRegion, imLeft, imTop);
      Aircraft* new_aircraft = new Aircraft(imType, serialNumber, imSquadron, imStowed, imFuel, *newLocation);
      embarkedAircraft.push_back(*new_aircraft);

      switch(new_aircraft->getRegionCode()) {
        case 0:{
          fdlist.push_back(embarkedAircraft.size()-1);
          break;
        }

        case 1:{
          hangarlist.push_back(embarkedAircraft.size()-1);
          break;
        }

        case 2:{
          onmission.push_back(embarkedAircraft.size()-1);
          break;
        }

        case 3:{
          transitlist.push_back(embarkedAircraft.size()-1);
          break;
        }
      }
    }
    aircraftFile.close();
  }

  else{
    std::cout << "No saved inventory found" << std::endl;
    std::cout << "Creating new inventory" << std::endl;

    std::ofstream new_inventory("./saved_inventory.txt");
    new_inventory.close();
  }

  std::ifstream squadronFile;
  squadronFile.open("./squadrons.txt", std::ios::in);

  if(squadronFile.is_open()){
    std::string squadron{};

    while(squadronFile){
      squadronFile >> squadron;
      if(squadron == "-1"){
        break;
      }
      embarkedSquadrons.push_back(squadron);
    }
  }

  else{
    std::cout << "No saved squadrons found" << std::endl;
    std::cout << "Creating new squadron list" << std::endl;

    std::ofstream new_squadrons("./squadrons.txt");
    new_squadrons.close();
  }

  std::ifstream shapesFile;
  shapesFile.open("./shapes.txt", std::ios::in);

  if(shapesFile.is_open()){
    int imWidth{};
    int imHeight{};
    int imStowedWidth{};

    while(shapesFile){
      shapesFile >> imWidth;
      if(imWidth == -1){
        break;
      }
      shapesFile >> imHeight;
      shapesFile >> imStowedWidth;

      Shape* importShape = new Shape(imWidth, imHeight, imStowedWidth);

      diagramShapes.push_back(*importShape);
    }
  }

  else{
    std::cout << "No saved shapes found" << std::endl;
    std::cout << "Creating new shapes list" << std::endl;

    std::ofstream new_shapes("./shapes.txt");
    new_shapes.close();
  }
}


Inventory::~Inventory(){
  std::ofstream aircraftFile;
  aircraftFile.open("./saved_inventory.txt");

  if(aircraftFile.is_open()){

    for(int i=0; i<embarkedAircraft.size(); i++){
      aircraftFile << embarkedAircraft.at(i).getDetails(1);
    }
    aircraftFile << "-1";
    aircraftFile.close();
  }
/*
  std::ofstream squadronFile;
  squadronFile.open("./squadrons.txt");

  if(squadronFile.is_open()){
    for(int i=0; i<embarkedSquadrons.size(); i++){
      squadronFile << embarkedSquadrons.at(i);

      squadronFile << "\n";
    }
    squadronFile << "-1";
    squadronFile.close();
  }
*/
}
