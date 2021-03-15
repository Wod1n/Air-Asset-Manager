#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "headers/aircraft.h"
#include "headers/inventory.h"

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

void Inventory::transferAircraft(){
  int initialRegion{0};
  int transfer{0};
  std::cout << "Transfer Aircraft from:" << std::endl;
  std::cout << "1. Hanger \n2. Flight Deck \n3. On Mission\n4. In Transit" << std::endl;
  std::cin >> initialRegion;

  std::cout << std::endl;

  int aircraftSelection{0};

  std::cout << "Which Aircraft to transfer" << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << outputData(initialRegion);
  std::cout << std::endl;
  std::cin >> aircraftSelection;

  aircraftSelection--;

  int finalRegion{0};

  std::cout << "Transfer Aircraft to:" << std::endl;
  std::cout << "1. Hanger \n2. Flight Deck \n3. On Mission\n4. In Transit" << std::endl;
  std::cin >> finalRegion;

  std::string message{};

  switch(initialRegion){
    case 1:{
      transfer = hangarlist.at(aircraftSelection);
      message += "<Time> Aircraft " + embarkedAircraft.at(transfer).getNumber() + " from Hangar ";
      hangarlist.erase(hangarlist.begin()+aircraftSelection);
      break;
    }

    case 2:{
      transfer = fdlist.at(aircraftSelection);
      message += "<Time> Aircraft " + embarkedAircraft.at(transfer).getNumber() + " from Flight Deck ";
      fdlist.erase(fdlist.begin()+aircraftSelection);
      break;
    }

    case 3:{
      transfer = onmission.at(aircraftSelection);
      message += "<Time> Aircraft " + embarkedAircraft.at(transfer).getNumber() + " from Deployment ";
      onmission.erase(onmission.begin()+aircraftSelection);
      break;
    }

    case 4:{
      transfer = transitlist.at(aircraftSelection);
      message += "<Time> Aircraft " + embarkedAircraft.at(transfer).getNumber() + " from Transit ";
      transitlist.erase(transitlist.begin()+aircraftSelection);
      break;
    }
  }

  embarkedAircraft.at(transfer).setRegion(finalRegion);

  switch(finalRegion){
    case 1:{
      hangarlist.push_back(transfer);
      message += "to Hangar";
      break;
    }

    case 2:{
      fdlist.push_back(transfer);
      int fly{};
      std::string spot{};
      std::cout << "Input Fly: ";
      std::cin >> fly;
      std::cout << "Input Spot: ";
      std::cin >> spot;
      embarkedAircraft.at(transfer).setLocation(fly, spot);
      message += "to " + embarkedAircraft.at(transfer).getRegion();
      break;
    }

    case 3:{
      onmission.push_back(transfer);
      message += "to Deployment";
      break;
    }

    case 4:{
      transitlist.push_back(transfer);
      message += "to Transit";
      break;
    }
  }

  std::cout << message << std::endl;
}

Inventory::Inventory(){
  std::ifstream aircraftFile;
  aircraftFile.open("./saved_inventory.txt", std::ios::in);

  if(aircraftFile.is_open()){
    int imType{};
    std::string serialNumber{};
    int imRegion{};
    int imFly{};
    char imSpot{};
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
      aircraftFile >> imFly;
      aircraftFile >> imSpot;
      aircraftFile >> imSquadron;
      aircraftFile >> imStowed;
      aircraftFile >> imFuel;
      Location *newLocation = new Location(imRegion, imFly, imSpot);
      Aircraft* new_aircraft = new Aircraft(imType, serialNumber, imSquadron, imStowed, imFuel, *newLocation);
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
    aircraftFile.close();
  }

  else{
    std::cout << "No saved inventory found" << std::endl;
    std::cout << "Creating new inventory" << std::endl;

    std::ofstream new_inventory("./saved_inventory.txt");
    new_inventory.close();
  }

  std::ifstream squadronFile;
  squadronFile.open("./saved_squadrons.txt", std::ios::in);

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

    std::ofstream new_squadrons("./saved_squadrons.txt");
    new_squadrons.close();
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

  std::ofstream squadronFile;
  squadronFile.open("./saved_squadrons.txt");

  if(squadronFile.is_open()){
    for(int i=0; i<embarkedSquadrons.size(); i++){
      squadronFile << embarkedSquadrons.at(i);

      squadronFile << "\n";
    }
    squadronFile << "-1";
    squadronFile.close();
  }
}
