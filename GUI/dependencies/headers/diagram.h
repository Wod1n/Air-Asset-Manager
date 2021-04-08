#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QGraphicsRectItem>
#include <QObject>

#include "aircraft.h"

class Diagram : public QGraphicsRectItem{
  private:
    Aircraft* assignedAircraft;
  public:
    explicit Diagram(QGraphicsRectItem *parent = nullptr);
    Aircraft* getAircraft();

    void setAircraft(Aircraft*);
};

#endif
