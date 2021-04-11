#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include <iostream>

#include "mainWindow.h"
#include "squadronsDialog.h"
#include "aircraftDialog.h"
#include "editDialog.h"
#include "transferDialog.h"

#include "./dependencies/headers/inventory.h"

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent){
  ui.setupUi(this);

  embarked = new Inventory;

  hangerScene = new QGraphicsScene(this);
  ui.hangerView->setScene(hangerScene);

  QObject::connect(hangerScene, SIGNAL(selectionChanged()), this, SLOT(setActiveHanger()));

  loadInventory();
}

void MainWindow::addAircraft(){

  AircraftDialog* dialog = new AircraftDialog;

  if(dialog->exec()){
    std::cout << "SAVE\n";
    std::cout << dialog->getAircraft()->getDetails() << std::endl;
    embarked->addAircraft(dialog->getAircraft());
    std::string serial = dialog->getAircraft()->getNumber();
    int type = dialog->getAircraft()->getTypeCode();
    bool stowed = dialog->getAircraft()->getStowed();
    addShape(dialog->getLocation(), type, stowed, serial);
  }

  delete dialog;
}

void MainWindow::editAircraft(){
  std::cout << "Edit\n";
  std::string serial = ui.serialNumber->text().toStdString();
  if(serial == "Serial Number"){
    std::cout << "No Active Aircraft" << std::endl;
    return;
  }
  EditDialog* dialog = new EditDialog;
  int activeIndex = embarked->searchAircraft(serial);
  Aircraft* activeAircraft = embarked->getAircraft(activeIndex);
  dialog->setData(serial, activeAircraft->getStowed(), activeAircraft->getTypeCode(), activeAircraft->getSquadronCode(), activeAircraft->getFuel());

  if(dialog->exec()){
    Aircraft* editedAircraft = dialog->getEdited();
    bool stowed = editedAircraft->getStowed();
    int type = editedAircraft->getTypeCode();
    int squadron = editedAircraft->getSquadronCode();
    int fuelLevel = editedAircraft->getFuel();
    embarked->editAircraft(activeIndex, stowed, type, squadron, fuelLevel);
    setActive(serial, editedAircraft->getType(), editedAircraft->getSquadron(), editedAircraft->getFuel());
  }
  delete dialog;
}

void MainWindow::removeAircraft(){
  std::cout << "Remove\n";
  std::string serial = ui.serialNumber->text().toStdString();
  if(serial == "Serial Number"){
    std::cout << "No Active Aircraft" << std::endl;
    return;
  }
  int activeIndex = embarked->searchAircraft(serial);
  int activeRegion = embarked->getAircraft(activeIndex)->getRegionCode();
  embarked->removeAircraft(activeIndex);

  switch (activeRegion){
    case 0:{
      std::cout << "Removed from Flight Deck" << '\n';
      return;
    }

    case 1:{
      std::cout << "Removed from Hanger" << '\n';
      hangerScene->removeItem(hangerScene->selectedItems().at(0));
      hangerScene->clearSelection();
      return;
    }

    case 2:{
      std::cout << "Removed from Mission" << '\n';
      delete ui.missionList->selectedItems().takeAt(0);
      ui.missionList->clearSelection();
      return;
    }

    case 3:{
      std::cout << "Removed from Transit" << '\n';
      delete ui.transitList->selectedItems().takeAt(0);
      ui.transitList->clearSelection();
      return;
    }
  }

}

void MainWindow::transferAircraft(){
  std::cout << "In Transit" << std::endl;

  for(int i=1 ; i<hangerScene->items().size(); i+=2){
    if(hangerScene->items().at(i)->isSelected()){
      std::cout << "SELECTED\n";
      std::cout << hangerScene->items().at(i-1)->isWidget() << std::endl;

      QGraphicsObject* active = hangerScene->items().at(i-1)->toGraphicsObject();
      std::string activeSerial = active->objectName().toStdString();
      std::cout << activeSerial << std::endl;
      int aircraftIndex = embarked->searchAircraft(activeSerial, 1, 1);
      embarked->transferAircraft(aircraftIndex, 1);
      hangerScene->removeItem(hangerScene->items().at(i));
      ui.transitList->addItem(QString::fromUtf8(activeSerial.c_str()));
      hangerScene->clearSelection();
      return;
    }
  }

  if(ui.missionList->selectedItems().size() != 0){
    std::string activeSerial = ui.missionList->selectedItems().at(0)->text().toStdString();
    std::cout << activeSerial << std::endl;
    int activeIndex = embarked->searchAircraft(activeSerial, 2, 1);

    embarked->transferAircraft(activeIndex, 2);
    delete ui.missionList->selectedItems().takeAt(0);
    addShape(3, -1, 0, activeSerial);
    ui.missionList->clearSelection();
    return;
  }

  if(ui.transitList->selectedItems().size() != 0){
    std::string activeSerial = ui.transitList->selectedItems().at(0)->text().toStdString();
    std::cout << activeSerial << std::endl;
    int activeIndex = embarked->searchAircraft(activeSerial, 3);
    int activeTransfer = embarked->searchAircraft(activeSerial, 3, 1);
    Aircraft* activeAircraft = embarked->getAircraft(activeIndex);
    int newRegion{};
    int type = activeAircraft->getTypeCode();
    bool stowed = activeAircraft->getStowed();

    TransferDialog* dialog = new TransferDialog;
    if(dialog->exec()){
      std::cout << dialog->getNewRegion() << std::endl;
      newRegion = dialog->getNewRegion();
    }
    delete dialog;
    embarked->transferAircraft(activeTransfer, 3, newRegion);
    delete ui.transitList->selectedItems().takeAt(0);
    addShape(newRegion, type, stowed, activeSerial);
    ui.transitList->clearSelection();
    return;
  }
}

void MainWindow::setActiveHanger(){
  /*
  std::cout << region << std::endl;
  switch (region) {
    case 1:{
      std::cout << "Active" << std::endl;
      break;
    }
  }
  */
  std::cout << "Hanger Active" << std::endl;

  //QList<QGraphicsWidget*> allWidgets = hangerScene->findChildren<QGraphicsWidget*>();

  for(int i=1 ; i<hangerScene->items().size(); i+=2){
    if(hangerScene->items().at(i)->isSelected()){
      std::cout << "SELECTED\n";
      std::cout << hangerScene->items().at(i-1)->isWidget() << std::endl;

      QGraphicsObject* active = hangerScene->items().at(i-1)->toGraphicsObject();
      std::string activeSerial = active->objectName().toStdString();
      std::cout << activeSerial << std::endl;
      int activeIndex = embarked->searchAircraft(activeSerial, 1);
      Aircraft* activeAircraft = embarked->getAircraft(activeIndex);
      setActive(activeSerial, activeAircraft->getType(), activeAircraft->getSquadron(), activeAircraft->getFuel());
      ui.missionList->clearSelection();
      ui.transitList->clearSelection();
      return;
    }
  }
  setActive("Serial Number", "Type", "Squadron", 42);
}

void MainWindow::setActiveMission(){
  std::cout << "Mission Select" << std::endl;
  if(ui.missionList->selectedItems().size() != 0){
    std::string activeSerial = ui.missionList->selectedItems().at(0)->text().toStdString();
    std::cout << activeSerial << std::endl;
    int activeIndex = embarked->searchAircraft(activeSerial, 2);
    Aircraft* activeAircraft = embarked->getAircraft(activeIndex);
    setActive(activeSerial, activeAircraft->getType(), activeAircraft->getSquadron(), activeAircraft->getFuel());
    hangerScene->clearSelection();
    ui.transitList->clearSelection();
  }
}

void MainWindow::setActiveTransit(){
  std::cout << "Transit Select" << std::endl;
  if(ui.transitList->selectedItems().size() != 0){
    std::string activeSerial = ui.transitList->selectedItems().at(0)->text().toStdString();
    std::cout << activeSerial << std::endl;
    int activeIndex = embarked->searchAircraft(activeSerial, 3);
    Aircraft* activeAircraft = embarked->getAircraft(activeIndex);
    setActive(activeSerial, activeAircraft->getType(), activeAircraft->getSquadron(), activeAircraft->getFuel());
    hangerScene->clearSelection();
    ui.missionList->clearSelection();
  }
}

void MainWindow::loadInventory(){
  std::cout << "Loading\n";
  if(embarked->getACNumber()!=0){
    for(int i=0; i<embarked->getACNumber(); i++){
      Aircraft* tempAircraft = embarked->getAircraft(i);
      switch (tempAircraft->getRegionCode()) {
        case 0:{
          std::cout << "Flight Deck\n";
          break;
        }

        case 1:{
          std::cout << "Hanger\n";
          int type = tempAircraft->getTypeCode();
          bool stowed = tempAircraft->getStowed();
          std::string serial = tempAircraft->getNumber();
          int left = tempAircraft->getLeft();
          int top = tempAircraft->getTop();
          addShape(1, type, stowed, serial, left, top);
          break;
        }

        case 2:{
          std::string tempString = tempAircraft->getNumber();
          QString missionAircraft = QString::fromUtf8(tempString.c_str());
          ui.missionList->addItem(missionAircraft);
          break;
        }

        case 3:{
          std::string tempString = tempAircraft->getNumber();
          QString transitAircraft = QString::fromUtf8(tempString.c_str());
          ui.transitList->addItem(transitAircraft);
          break;
        }

        default:{
          std::cout << "Unknown\n";
        }
      }
    }
  }
}

void MainWindow::addShape(int region, int type, bool stowed, std::string serial, int left, int top){
  QWidget* newAircraft = new QWidget;
  std::cout << serial << std::endl;
  int tempWidth{};
  int tempHeight{};
  if(type != -1){
    tempWidth = embarked->getShape(type)->getWidth(stowed);
    tempHeight = embarked->getShape(type)->getHeight();

    newAircraft->resize(tempWidth, tempHeight);
    QGroupBox* layout = new QGroupBox(newAircraft);
    layout->resize(tempWidth, tempHeight);
    layout->setTitle(serial.c_str());
  }
  switch (region) {
    case 0:{
      std::cout << "Flight Deck Graphic\n";
      break;
    }

    case 1:{
      QGraphicsWidget* const proxy = hangerScene->addWidget(newAircraft);
      proxy->setFlag(QGraphicsItem::ItemIsMovable, true);
      proxy->setObjectName(serial.c_str());

      QGraphicsRectItem* const proxyControl = hangerScene->addRect(left, top, tempWidth, 10, QPen(Qt::black), QBrush(Qt::red));
      std::cout << tempWidth << std::endl;
      proxyControl->setFlag(QGraphicsItem::ItemIsMovable, true);
      proxyControl->setFlag(QGraphicsItem::ItemIsSelectable, true);
      proxy->setPos(left, top+proxyControl->rect().height());
      proxy->setParentItem(proxyControl);
      //hangerAircraft.push_back(*proxyControl);
      break;
    }

    case 2:{
      QString missionAircraft = QString::fromUtf8(serial.c_str());
      ui.missionList->addItem(missionAircraft);
      break;
    }

    case 3:{
      QString transitAircraft = QString::fromUtf8(serial.c_str());
      ui.transitList->addItem(transitAircraft);
      break;
    }
  }
/*
  rectangle = hangerScene->addRect(100, 0, 80, 100, outlinePen, blueBrush);
  rectangle->setFlag(QGraphicsItem::ItemIsMovable);
  */
}

void MainWindow::setActive(std::string serial, std::string type, std::string squadron, int fuel){
  ui.serialNumber->setText(QString::fromUtf8(serial.c_str()));
  ui.type->setText(QString::fromUtf8(type.c_str()));
  ui.squadron->setText(QString::fromUtf8(squadron.c_str()));
  ui.fuelLevel->setValue(fuel);
}

void MainWindow::viewSquadrons(){
  SquadronsDialog* dialog = new SquadronsDialog;
  dialog->show();
}

MainWindow::~MainWindow(){
  for(int i=0 ; i<hangerScene->items().size(); i+=2){
    std::cout << hangerScene->items().at(i)->scenePos().x() << std::endl;
    std::cout << hangerScene->items().at(i)->scenePos().y() << std::endl;

    int left = hangerScene->items().at(i)->scenePos().x();
    int top = hangerScene->items().at(i)->scenePos().y();
    std::cout << (i-1)/2 << std::endl;

    embarked->setLocation(1, ((i-1)/2), left, top);
  }
  delete embarked;
}
