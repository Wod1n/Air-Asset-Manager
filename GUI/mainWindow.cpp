#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include <QImage>
#include <QPixmap>
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
  hangerScene->setSceneRect(0,0,550,180);

  fdScene = new QGraphicsScene(this);
  fdScene->setSceneRect(0,0,920,260);
  ui.fdView->setScene(fdScene);
  //hangerScene->setSceneRect(0,0,200,80);
  ui.hangerView->setScene(hangerScene);
  //ui.hangerView->scale(0.7, 0.7);

  QImage hangerbackground("./dependencies/images/hanger.jpg");
  QBrush backgroundBrush;
  backgroundBrush.setTextureImage(hangerbackground);
  hangerScene->setBackgroundBrush(backgroundBrush);

  QImage fdbackground("./dependencies/images/QEFD.jpg");
  backgroundBrush.setTextureImage(fdbackground);
  fdScene->setBackgroundBrush(backgroundBrush);

  QObject::connect(fdScene, SIGNAL(selectionChanged()), this, SLOT(setActiveFD()));
  QObject::connect(hangerScene, SIGNAL(selectionChanged()), this, SLOT(setActiveHanger()));

  loadInventory();
}

void MainWindow::addAircraft(){

  AircraftDialog* dialog = new AircraftDialog;

  if(dialog->exec()){
    std::cout << "SAVE\n";
    std::cout << dialog->getAircraft()->getDetails() << std::endl;
    Aircraft* newAircraft = dialog->getAircraft();
    embarked->addAircraft(newAircraft);
    /*
    std::string serial = dialog->getAircraft()->getNumber();
    int type = dialog->getAircraft()->getTypeCode();
    bool stowed = dialog->getAircraft()->getStowed();
    */
    addShape(newAircraft);
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

void MainWindow::rotateAircraft(){
  std::cout << "Rotate" << std::endl;

  std::string serial = ui.serialNumber->text().toStdString();

  if(serial == "Serial Number"){
    std::cout << "No Active Aircraft" << std::endl;
    return;
  }
  int activeIndex = embarked->searchAircraft(serial);
  int activeRegion = embarked->getAircraft(activeIndex)->getRegionCode();

  switch (activeRegion) {
    case 0:{
      std::cout << "Flight Deck" << std::endl;
      for(int i=1 ; i<fdScene->items().size(); i+=2){
        if(fdScene->items().at(i)->isSelected()){
          std::cout << "SELECTED\n";
          std::cout << fdScene->items().at(i-1)->isWidget() << std::endl;
          QGraphicsObject* active = fdScene->items().at(i-1)->toGraphicsObject();
          std::string activeSerial = active->objectName().toStdString();
          std::cout << activeSerial << std::endl;
          int aircraftIndex = embarked->searchAircraft(activeSerial, 0, 1);
          std::cout << fdScene->items().at(i-1)->x() << " " << fdScene->items().at(i-1)->y() << std::endl;
          fdScene->items().at(i)->setTransformOriginPoint(fdScene->items().at(i-1)->x(), fdScene->items().at(i-1)->y());
          qreal angle = hangerScene->items().at(i)->rotation();
          angle += 90;
          fdScene->items().at(i)->setRotation(angle);
          embarked->setAircraftRotation(aircraftIndex, angle);
          return;
        }
      }
      break;
    }

    case 1:{
      std::cout << "Hanger" << std::endl;
      for(int i=1 ; i<hangerScene->items().size(); i+=2){
        if(hangerScene->items().at(i)->isSelected()){
          std::cout << "SELECTED\n";
          std::cout << hangerScene->items().at(i-1)->isWidget() << std::endl;
          QGraphicsObject* active = hangerScene->items().at(i-1)->toGraphicsObject();
          std::string activeSerial = active->objectName().toStdString();
          std::cout << activeSerial << std::endl;
          int aircraftIndex = embarked->searchAircraft(activeSerial, 1, 1);
          std::cout << hangerScene->items().at(i-1)->x() << " " << hangerScene->items().at(i-1)->y() << std::endl;
          hangerScene->items().at(i)->setTransformOriginPoint(hangerScene->items().at(i-1)->x(), hangerScene->items().at(i-1)->y());
          qreal angle = hangerScene->items().at(i)->rotation();
          angle += 90;
          hangerScene->items().at(i)->setRotation(angle);
          embarked->setAircraftRotation(aircraftIndex, angle);
          return;
        }
      }
      break;
    }

    default:{
      std::cout << "Nothing to rotate" << std::endl;
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
    Aircraft* transitAircraft = embarked->getAircraft(activeIndex);
    delete ui.missionList->selectedItems().takeAt(0);
    addShape(transitAircraft);
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
    /*
    int type = activeAircraft->getTypeCode();
    bool stowed = activeAircraft->getStowed();
    */
    TransferDialog* dialog = new TransferDialog;
    if(dialog->exec()){
      std::cout << dialog->getNewRegion() << std::endl;
      newRegion = dialog->getNewRegion();
      embarked->transferAircraft(activeTransfer, 3, newRegion);
      delete ui.transitList->selectedItems().takeAt(0);
      addShape(activeAircraft);
      ui.transitList->clearSelection();
    }
    delete dialog;
    return;
  }
}

void MainWindow::setActiveFD(){
  /*
  std::cout << region << std::endl;
  switch (region) {
    case 1:{
      std::cout << "Active" << std::endl;
      break;
    }
  }
  */
  std::cout << "Flight Deck Active" << std::endl;

  //QList<QGraphicsWidget*> allWidgets = hangerScene->findChildren<QGraphicsWidget*>();

  for(int i=0; i<fdScene->items().size(); i++){
    std::cout << fdScene->items().at(i)->scenePos().x() << " " << fdScene->items().at(i)->scenePos().y() << std::endl;
    std::cout << fdScene->items().at(i)->isWidget() << std::endl;
  }

  for(int i=1; i<fdScene->items().size(); i+=2){
    if(fdScene->items().at(i)->isSelected()){
      std::cout << "SELECTED\n";
      std::cout << fdScene->items().at(i-1)->isWidget() << std::endl;

      QGraphicsObject* active = fdScene->items().at(i-1)->toGraphicsObject();
      std::string activeSerial = active->objectName().toStdString();
      std::cout << activeSerial << std::endl;
      int activeIndex = embarked->searchAircraft(activeSerial, 0);
      Aircraft* activeAircraft = embarked->getAircraft(activeIndex);
      setActive(activeSerial, activeAircraft->getType(), activeAircraft->getSquadron(), activeAircraft->getFuel());
      std::cout << fdScene->items().at(i)->scenePos().x() << std::endl;
      std::cout << fdScene->items().at(i)->scenePos().y() << std::endl;
      ui.missionList->clearSelection();
      ui.transitList->clearSelection();
      return;
    }
  }
  setActive("Serial Number", "Type", "Squadron", 42);
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

  for(int i=0; i<hangerScene->items().size(); i++){
    std::cout << hangerScene->items().at(i)->scenePos().x() << " " << hangerScene->items().at(i)->scenePos().y() << std::endl;
    std::cout << hangerScene->items().at(i)->isWidget() << std::endl;
  }

  for(int i=1; i<hangerScene->items().size(); i+=2){
    if(hangerScene->items().at(i)->isSelected()){
      std::cout << "SELECTED\n";
      std::cout << hangerScene->items().at(i-1)->isWidget() << std::endl;

      QGraphicsObject* active = hangerScene->items().at(i-1)->toGraphicsObject();
      std::string activeSerial = active->objectName().toStdString();
      std::cout << activeSerial << std::endl;
      int activeIndex = embarked->searchAircraft(activeSerial, 1);
      Aircraft* activeAircraft = embarked->getAircraft(activeIndex);
      setActive(activeSerial, activeAircraft->getType(), activeAircraft->getSquadron(), activeAircraft->getFuel());
      std::cout << hangerScene->items().at(i)->scenePos().x() << std::endl;
      std::cout << hangerScene->items().at(i)->scenePos().y() << std::endl;
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
          int left = tempAircraft->getLeft();
          int top = tempAircraft->getTop();
          int rotation = tempAircraft->getRotation();
          addShape(tempAircraft, left, top, rotation);
          break;
        }

        case 1:{
          std::cout << "Hanger\n";
          /*
          int type = tempAircraft->getTypeCode();
          bool stowed = tempAircraft->getStowed();
          std::string serial = tempAircraft->getNumber();
          */
          int left = tempAircraft->getLeft();
          int top = tempAircraft->getTop();
          int rotation = tempAircraft->getRotation();
          addShape(tempAircraft, left, top, rotation);
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

void MainWindow::addShape(Aircraft* createdAircraft, int left, int top, int rotation){
  QWidget* newAircraft = new QWidget;
  std::cout << createdAircraft->getNumber() << std::endl;
  int tempWidth{};
  int tempHeight{};
  if(createdAircraft->getTypeCode() != -1){
    tempWidth = embarked->getShape(createdAircraft->getTypeCode())->getWidth(createdAircraft->getStowed());
    tempHeight = embarked->getShape(createdAircraft->getTypeCode())->getHeight();
    newAircraft->resize(tempWidth, tempHeight);
    QGroupBox* layout = new QGroupBox(newAircraft);
    layout->resize(tempWidth, tempHeight);
    layout->setTitle(createdAircraft->getNumber().c_str());
  }
  std::string imagePath{"./dependencies/images/"};
  imagePath += createdAircraft->getType();
  if (createdAircraft->getStowed()) {
    imagePath += "(S)";
  }
  imagePath += ".jpg";
  QPixmap aircraftimage(imagePath.c_str());
  QPixmap scaled = aircraftimage.scaled(tempWidth, tempHeight, Qt::IgnoreAspectRatio, Qt::FastTransformation);
  QLabel* label = new QLabel(newAircraft);
  label->setPixmap(scaled);
  switch (createdAircraft->getRegionCode()) {
    case 0:{
      QGraphicsWidget* const proxy = fdScene->addWidget(newAircraft);
      proxy->setFlag(QGraphicsItem::ItemIsMovable, true);
      proxy->setObjectName(createdAircraft->getNumber().c_str());

      QGraphicsRectItem* const proxyControl = fdScene->addRect(left-5, top-5, tempWidth + 10 , tempHeight + 10, QPen(Qt::black), QBrush(Qt::red));
      std::cout << left << " " << top << std::endl;
      proxyControl->setFlag(QGraphicsItem::ItemIsMovable, true);
      proxyControl->setFlag(QGraphicsItem::ItemIsSelectable, true);
      proxy->setPos(left, top);
      proxy->setParentItem(proxyControl);
      proxyControl->setTransformOriginPoint(proxy->x(), proxy->y());
      proxyControl->setRotation(rotation);
      break;
    }

    case 1:{
      QGraphicsWidget* const proxy = hangerScene->addWidget(newAircraft);
      proxy->setFlag(QGraphicsItem::ItemIsMovable, true);
      proxy->setObjectName(createdAircraft->getNumber().c_str());

      QGraphicsRectItem* const proxyControl = hangerScene->addRect(left-5, top-5, tempWidth + 10 , tempHeight + 10, QPen(Qt::black), QBrush(Qt::red));
      std::cout << left << " " << top << std::endl;
      proxyControl->setFlag(QGraphicsItem::ItemIsMovable, true);
      proxyControl->setFlag(QGraphicsItem::ItemIsSelectable, true);
      proxy->setPos(left, top);
      proxy->setParentItem(proxyControl);
      proxyControl->setTransformOriginPoint(proxy->x(), proxy->y());
      proxyControl->setRotation(rotation);
      //hangerAircraft.push_back(*proxyControl);
      break;
    }

    case 2:{
      QString missionAircraft = QString::fromUtf8(createdAircraft->getNumber().c_str());
      ui.missionList->addItem(missionAircraft);
      break;
    }

    case 3:{
      QString transitAircraft = QString::fromUtf8(createdAircraft->getNumber().c_str());
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

void MainWindow::arrived(){
  std::cout << "Arrived" << '\n';
}

void MainWindow::viewSquadrons(){
  SquadronsDialog* dialog = new SquadronsDialog;
  dialog->show();
}

MainWindow::~MainWindow(){
  for(int i=0 ; i<hangerScene->items().size(); i+=2){
    QGraphicsObject* active = hangerScene->items().at(i)->toGraphicsObject();
    std::string activeSerial = active->objectName().toStdString();
    std::cout << activeSerial << std::endl;
    int activeIndex = embarked->searchAircraft(activeSerial, 1);
    std::cout << hangerScene->items().at(i)->scenePos().x() << std::endl;
    std::cout << hangerScene->items().at(i)->scenePos().y() << std::endl;

    int left = hangerScene->items().at(i)->scenePos().x();
    int top = hangerScene->items().at(i)->scenePos().y();
    std::cout << (i-1)/2 << std::endl;

    embarked->setLocation(1, activeIndex, left, top);
  }

  for(int i=0 ; i<fdScene->items().size(); i+=2){
    QGraphicsObject* active = fdScene->items().at(i)->toGraphicsObject();
    std::string activeSerial = active->objectName().toStdString();
    std::cout << activeSerial << std::endl;
    int activeIndex = embarked->searchAircraft(activeSerial, 0);
    std::cout << fdScene->items().at(i)->scenePos().x() << std::endl;
    std::cout << fdScene->items().at(i)->scenePos().y() << std::endl;

    int left = fdScene->items().at(i)->scenePos().x();
    int top = fdScene->items().at(i)->scenePos().y();
    std::cout << (i-1)/2 << std::endl;

    embarked->setLocation(0, activeIndex, left, top);
  }
  delete embarked;
}
