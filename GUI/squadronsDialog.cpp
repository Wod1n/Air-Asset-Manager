#include <QMainWindow>
#include <fstream>

#include "squadronsDialog.h"


SquadronsDialog::SquadronsDialog(QDialog *parent) : QDialog(parent){
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
      QListWidgetItem *newItem = new QListWidgetItem;
      newItem->setText(newSquadron);
      ui.squadronsList->insertItem(ui.squadronsList->count(), newItem);
    }
  }

  else{
    std::ofstream new_squadrons("./squadrons.txt");
    new_squadrons.close();
  }
}

void SquadronsDialog::addItem(){
    ui.squadronsList->addItem(ui.lineEdit->text());
    ui.lineEdit->clear();
}

void SquadronsDialog::accept(){
  std::ofstream squadronFile;
  squadronFile.open("./squadrons.txt");

  if(squadronFile.is_open()){
    for(int i=0; i<ui.squadronsList->count(); i++){
      squadronFile << ui.squadronsList->item(i)->text().toStdString();
      squadronFile << "\n";
    }
    squadronFile << "-1";
    squadronFile.close();
  }

  this->close();
}
