#include <iostream>
#include <QGraphicsRectItem>

#include "headers/diagram.h"

void Diagram::setAircraft(Aircraft* input){
  assignedAircraft = input;
}

Aircraft* Diagram::getAircraft(){
  return assignedAircraft;
}

Diagram::Diagram(QGraphicsRectItem* parent) : QGraphicsRectItem(parent){
  std::cout << "Creator" << std::endl;
}
