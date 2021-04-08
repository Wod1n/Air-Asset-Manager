#include <QMainWindow>
#include <fstream>
#include <iostream>

#include "aircraftDialog.h"
#include "dependencies/headers/aircraft.h"

AircraftDialog::AircraftDialog(QDialog *parent) : QDialog(parent){
  ui.setupUi(this);

  std::ifstream squadronFile;
  squadronFile.open("./squadrons.txt", std::ios::in);

  if(squadronFile.is_open()){
    std::string squadron{};

    while(squadronFile){
      squadronFile >> squadron;
      if(squadron == "-1"){
        break;
      }
      QString newSquadron = QString::fromUtf8(squadron.c_str());
      ui.squadronBox->addItem(newSquadron);
    }
  }

  else{
    std::ofstream new_squadrons("./squadrons.txt");
    new_squadrons.close();
  }

  std::ifstream aircraftTypes;
  aircraftTypes.open("./types.txt", std::ios::in);

  if(aircraftTypes.is_open()){
    std::string type{};

    while(aircraftTypes){
      aircraftTypes >> type;
      if(type == "-1"){
        break;
      }
      QString newType = QString::fromUtf8(type.c_str());
      ui.typeBox->addItem(newType);
    }
  }

  else{
    //throw exception
  }
}

void AircraftDialog::accept(){

  QString tempString = ui.serialInput->text();
  std::string serial = tempString.toStdString();
  int type = ui.typeBox->currentIndex();
  bool stowed = ui.stowedBox->isChecked();
  int squadron = ui.squadronBox->currentIndex();
  int fuel = ui.fuelBox->value();
  location = ui.initialLocation->currentIndex();

  std::cout << typeid(tempString).name() << std::endl;

  Location* tempLocation = new Location(location, -1, 'Z');
  createdAircraft = new Aircraft(type, serial, squadron, stowed, fuel, *tempLocation);

  std::cout << createdAircraft->getDetails() << std::endl;

  this->done(1);
}

int AircraftDialog::getLocation(){
  return location;
}

Aircraft* AircraftDialog::getAircraft(){
  return createdAircraft;
}
