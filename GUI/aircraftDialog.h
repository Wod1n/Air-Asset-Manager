#ifndef AIRCRAFTDIALOG_H
#define AIRCRAFTDIALOG_H

#include <QObject>

#include "dependencies/headers/aircraft.h"

#include "ui_aircraftDialog.h"

class AircraftDialog : public QDialog{
Q_OBJECT
public:
  explicit AircraftDialog(QDialog *parent = nullptr);
  int getLocation();
  Aircraft* getAircraft();

public slots:
  void accept();

private:
  Ui::AircraftDialog ui;
  Aircraft* createdAircraft{};
  int location;
};

#endif
