#ifndef SQUADRONSDIALOG_H
#define SQUADRONSDIALOG_H

#include <QObject>

#include "ui_squadronsDialog.h"

class SquadronsDialog : public QDialog{
Q_OBJECT
public:
  explicit SquadronsDialog(QDialog *parent = nullptr);
  void accept();
  void discard();
public slots:
  void addItem();

private:
  Ui::SquadronsDialog ui;
};

#endif
