#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QObject>

#include "dependencies/headers/inventory.h"
#include "dependencies/headers/diagram.h"

#include "ui_mainWindow.h"

class MainWindow : public QMainWindow{
Q_OBJECT
public:
  explicit MainWindow(QMainWindow *parent = nullptr);
  void loadInventory();
  void addShape(int, int, bool, std::string, int=100, int=0);
  void setActive(std::string, std::string, int);
  ~MainWindow();
public slots:
  void addAircraft();
  void transferAircraft();
  void setActiveHanger();
  void setActiveTransit();
private:
  Ui::MainWindow ui;
  QGraphicsScene* hangerScene;
  QGraphicsRectItem* rectangle;

  std::vector<Diagram> hangerAircraft;

  Inventory* embarked;
};

#endif
