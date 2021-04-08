#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include <iostream>

#include "mainWindow.h"
#include "squadronsDialog.h"
#include "aircraftDialog.h"
#include "transferDialog.h"

#include "./dependencies/headers/inventory.h"

void viewSquadrons();

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent){
  ui.setupUi(this);
  connect(ui.actionviewSquadrons, &QAction::triggered, viewSquadrons);

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
    std::string tempString = dialog->getAircraft()->getNumber();
    int type = dialog->getAircraft()->getTypeCode();
    bool stowed = dialog->getAircraft()->getStowed();
    switch (dialog->getLocation()) {
      case 0:{
        addShape(0, type, stowed, tempString);
        break;
      }

      case 1:{
        addShape(1, type, stowed, tempString);
        break;
      }

      case 2:{
        QString missionAircraft = QString::fromUtf8(tempString.c_str());
        ui.missionList->addItem(missionAircraft);
        break;
      }

      case 3:{
        QString transitAircraft = QString::fromUtf8(tempString.c_str());
        ui.transitList->addItem(transitAircraft);
        break;
      }

      default:{
        std::cout << "Default" << '\n';
      }
    }
  }

  delete dialog;
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
      return;
    }
  }

  if(ui.transitList->selectedItems().size() != 0){
    std::string activeSerial = ui.transitList->selectedItems().at(0)->text().toStdString();
    std::cout << activeSerial << std::endl;
    int activeIndex = embarked->searchAircraft(activeSerial, 3);
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
    embarked->transferAircraft(activeIndex, 3, newRegion);
    delete ui.transitList->selectedItems().takeAt(0);
    addShape(newRegion, type, stowed, activeSerial);
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
      setActive(activeSerial, activeAircraft->getType(), activeAircraft->getFuel());
      return;
    }
  }
  setActive("Serial Number", "Type", 42);
}

void MainWindow::setActiveTransit(){
  std::cout << "Transit Select" << std::endl;
  if(ui.transitList->selectedItems().size() != 0){
    std::string activeSerial = ui.transitList->selectedItems().at(0)->text().toStdString();
    std::cout << activeSerial << std::endl;
    int activeIndex = embarked->searchAircraft(activeSerial, 3);
    Aircraft* activeAircraft = embarked->getAircraft(activeIndex);
    setActive(activeSerial, activeAircraft->getType(), activeAircraft->getFuel());
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
  int tempWidth = embarked->getShape(type)->getWidth(stowed);
  int tempHeight = embarked->getShape(type)->getHeight();

  newAircraft->resize(tempWidth, tempHeight);
  QGroupBox* layout = new QGroupBox(newAircraft);
  layout->resize(tempWidth, tempHeight);
  layout->setTitle(serial.c_str());

  switch (region) {
    case 0:{
      std::cout << "Flight Deck Graphic\n";
      break;
    }

    case 1:{
      QGraphicsWidget* const proxy = hangerScene->addWidget(newAircraft);
      proxy->setFlag(QGraphicsItem::ItemIsMovable, true);
      proxy->setObjectName(serial.c_str());

      QGraphicsRectItem* proxyControl = hangerScene->addRect(left, top, tempWidth, 10, QPen(Qt::black), QBrush(Qt::red));
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

void MainWindow::setActive(std::string serial, std::string type, int fuel){
  ui.serialNumber->setText(QString::fromUtf8(serial.c_str()));
  ui.type->setText(QString::fromUtf8(type.c_str()));
  ui.fuelLevel->setValue(fuel);
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

void viewSquadrons(){
  SquadronsDialog* dialog = new SquadronsDialog;
  dialog->show();
}
