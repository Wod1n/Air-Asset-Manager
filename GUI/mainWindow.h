#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QObject>

#include "dependencies/headers/aircraft.h"
#include "dependencies/headers/inventory.h"
#include "dependencies/headers/diagram.h"

#include "ui_mainWindow.h"

class MainWindow : public QMainWindow{
Q_OBJECT
public:
  explicit MainWindow(QMainWindow *parent = nullptr);
  void loadInventory();
  void addShape(Aircraft*, int=0, int=0, int=0);
  void setActive(std::string, std::string, std::string, int);
  ~MainWindow();
public slots:
  void addAircraft();
  void arrived();
  void editAircraft();
  void removeAircraft();
  void rotateAircraft();
  void transferAircraft();
  void viewSquadrons();
  void setActiveFD();
  void setActiveHanger();
  void setActiveMission();
  void setActiveTransit();
private:
  Ui::MainWindow ui;
  QGraphicsScene* hangerScene;
  QGraphicsScene* fdScene;
  QGraphicsRectItem* rectangle;

  std::vector<Diagram> hangerAircraft;

  Inventory* embarked;
};

#endif
