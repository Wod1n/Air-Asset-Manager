#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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

void Location::setRegion(int newRegion){
  region = newRegion;
}

void Location::setLocation(int newfly, std::string newspot){
  fly = newfly;
  spot = newspot;
}

std::string Location::getLocation(){

  std::string locationString{};

  switch (region) {
    case 2:{
      switch (fly) {
        case 0:{
          locationString += "Knuckle: ";
          break;
        }

        case 1:{
          locationString += "Fly 1: ";
          break;
        }

        case 2:{
          locationString += "Fly 2: ";
          break;
        }

        case 3:{
          locationString += "Fly 3: ";
          break;
        }

        case 4:{
          locationString += "Fly 4: ";
          break;
        }

        case 5:{
          locationString += "Graveyard: ";
          break;
        }

        case 6:{
          locationString += "Bridge Park: ";
          break;
        }

        case 7:{
          locationString += "FlyCo Park: ";
          break;
        }

        case 8:{
          locationString += "Fwd Lift: ";
          break;
        }

        case 9:{
          locationString += "Aft Lift: ";
          break;
        }
      }
      locationString += "Spot " + spot;
    }
  }

  return locationString;
}


std::string Location::getSpot(){
  return "Spot";
}

std::string Location::getRegion(){
  switch (region){

    case 1:{
      return "Hanger";
    }

    case 2:{
      return "Flight Deck " + getLocation();
    }

    case 3:{
      return "In Transit";
    }

    case 4:{
      return "On Mission";
    }

    default:{
      return "Unknown";
    }
  }
}

int Location::getRegionCode(){
  return region;
}

std::string Location::saveData(){
  std::string outputString{};
  outputString += std::to_string(region) + " " + std::to_string(fly) + " " + spot;
  return outputString;
}

Location::Location(){
  std::cout << "Input Region: ";
  std::cin >> region;
  std::cout << std::endl;

  switch (region) {
    case 2:{
      std::cout << "Input Fly: ";
      std::cin >> fly;

      std::cout << "Input Spot: ";
      std::cin >> spot;
      std::cout << std::endl;
      break;
    }

    default:{
      fly = -1;
      spot = 'Z';
      break;
    }
  }
}

Location::Location(int imRegion, int imFly, char imSpot){
  region = imRegion;
  fly = imFly;
  spot = imSpot;
}

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
    std::string getDetails(bool);
};

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

std::string Aircraft::getDetails(bool ex=0){
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

std::string Inventory::outputData(int selection=0){
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

class Shape{
  private:
    int width;
    int length;
    int orientation;
};

bool mainMenu(Inventory *embarked){
  int selection{};

  std::cout << "Main Menu\n" << "=========================" << std::endl;

  std::cout << "1. View Embarked Aircraft\n";
  std::cout << "2. View Aircraft in:\n";
  std::cout << "3. Add New Aircraft\n";
  std::cout << "4. Transfer Aircraft\n";
  std::cout << "5. Remove Aircraft\n";
  std::cout << "6. Edit Aircraft\n";
  std::cout << "7. Add Squadron\n";
  std::cout << "8. View Embarked Squadrons\n";
  std::cout << "0. Save and Quit" << std::endl;
  std::cout << "=========================" << std::endl;

  std::cout << "Select Option: ";

  std::cin >> selection;

  switch(selection){
    case 1:{
      std::cout << "-------------------------" << std::endl;
      std::cout << embarked->outputData() << std::endl;
      return 1;
    }

    case 2:{
      int newSelection{};
      std::cout << "1. Hanger \n2. Flight Deck \n3. On Mission\n4. In Transit" << std::endl;
      std::cout << "-------------------------" << std::endl;
      std::cout << "Select Option: ";
      std::cin >> newSelection;

      std::cout << embarked->outputData(newSelection) << std::endl;
      return 1;
    }

    case 3:{
      embarked->addAircraft();
      return 1;
    }

    case 4:{
      embarked->transferAircraft();
      return 1;
    }

    case 7:{
      embarked->addSquadron();
      return 1;
    }

    case 8:{
      std::cout << embarked->viewSquadrons() << std::endl;
      return 1;
    }

    case 0:{
      return 0;
    }

    default:{
      std::cout << "Invalid Selection" << std::endl;
      return 1;
    }
  }
}

int main(){

  Inventory *embarked = new Inventory;

  while(mainMenu(embarked));

  delete embarked;

  return 1;
}
