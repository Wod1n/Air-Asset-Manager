#include <fstream>

#include "editDialog.h"

EditDialog::EditDialog(QDialog *parent) : QDialog(parent){
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

void EditDialog::accept(){

  QString tempString = ui.activeSerial->text();
  std::string serial = tempString.toStdString();
  int type = ui.typeBox->currentIndex();
  bool stowed = ui.stowedBox->isChecked();
  int squadron = ui.squadronBox->currentIndex();
  int fuel = ui.fuelBox->value();

  editedAircraft = new Aircraft(type, serial, squadron, stowed, fuel);

  this->done(1);
}

void EditDialog::setData(std::string serial, bool stowed, int type, int squadron, int fuelLevel){
  ui.activeSerial->setText(QString::fromUtf8(serial.c_str()));
  ui.typeBox->setCurrentIndex(type);
  ui.stowedBox->setChecked(stowed);
  ui.squadronBox->setCurrentIndex(squadron);
  ui.fuelSlider->setValue(fuelLevel);
}

Aircraft* EditDialog::getEdited(){
  return editedAircraft;
}
