#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "dependencies/headers/aircraft.h"

#include "ui_editDialog.h"

class EditDialog : public QDialog{

public:
    explicit EditDialog(QDialog *parent = nullptr);
    void setData(std::string, bool, int, int, int);
    Aircraft* getEdited();

public slots:
  void accept();

private:
    Ui::EditDialog ui;
    Aircraft* editedAircraft;
};

#endif
